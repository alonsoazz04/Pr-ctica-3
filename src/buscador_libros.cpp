#include "buscador_libros.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

BuscadorLibros::BuscadorLibros(const std::vector<Libro>& libros) : libros(libros) {}

ResultadoBusquedaCliente BuscadorLibros::buscar(Cliente& cliente, ServicioPago& servicioPago) const {
    ResultadoBusquedaCliente resultado;
    auto inicio = std::chrono::steady_clock::now();

    std::mutex resultadoMutex;
    std::mutex clienteMutex;
    std::atomic<int> contadorPalabras{0};
    std::atomic<bool> pararBusqueda{false};

    std::vector<std::thread> hilos;

    for (const auto& libro : libros) {
        hilos.emplace_back([&, libro]() {
            std::size_t pos = libro.contenido.find(cliente.getPalabra());

            while (pos != std::string::npos && !pararBusqueda.load()) {
                {
                    std::lock_guard<std::mutex> lock(resultadoMutex);
                    resultado.aniadirBusqueda(libro.nombre, 0, static_cast<int>(pos));
                }

                if (cliente.esGratis()) {
                    int actual = ++contadorPalabras;
                    if (actual >= cliente.getLimitePalabras()) {
                        pararBusqueda = true;
                        break;
                    }
                } else if (cliente.esPremiumLimite()) {
                    std::lock_guard<std::mutex> lock(clienteMutex);

                    if (!cliente.hasSaldo()) {
                        servicioPago.recargar(cliente, 10);
                    }

                    cliente.consumirCredito();
                }

                pos = libro.contenido.find(cliente.getPalabra(), pos + 1);
            }
        });
    }

    for (auto& hilo : hilos) {
        if (hilo.joinable()) {
            hilo.join();
        }
    }

    auto fin = std::chrono::steady_clock::now();
    resultado.setTiempoTotal(
        std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio)
    );

    return resultado;
}