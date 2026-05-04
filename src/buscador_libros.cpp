#include "buscador_libros.hpp"
#include <thread>
#include <chrono>
#include <mutex>

BuscadorLibros::BuscadorLibros(const std::vector<Libro>& libros) : libros(libros) {}

ResultadoBusquedaCliente BuscadorLibros::buscar(const Cliente& cliente) const {
    ResultadoBusquedaCliente resultado;

    auto inicio = std::chrono::steady_clock::now();

    std::mutex resultadoMutex;
    std::atomic<int> contadorPalabras{0};
    std::atomic<int> saldoRestante(cliente.getSaldo());
    std::atomic<bool> pararBusqueda{false};

    std::vector<std::thread> hilos;

    for (const auto& libro : libros) {
        hilos.emplace_back([&]() {
            if (pararBusqueda.load()) {
                return;
            }

            std::size_t pos = libro.contenido.find(cliente.getPalabra());
            while (pos != std::string::npos && !pararBusqueda.load()) {
                {
                    std::lock_guard<std::mutex> lock(resultadoMutex);
                    resultado.aniadirBusqueda(libro.nombre, 0, static_cast<int>(pos));
                }

                if (cliente.esGratis()) {
                    int current = ++contadorPalabras;
                    if (current >= cliente.getLimitePalabras()) {
                        pararBusqueda = true;
                        break;
                    }
                } else if (cliente.esPremiumLimite()) {
                    int nuevoSaldo = --saldoRestante;
                    if (nuevoSaldo <= 0) {
                        pararBusqueda = true;
                        break;
                    }
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

    resultado.setTiempoTotal(std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio));

    return resultado;
}