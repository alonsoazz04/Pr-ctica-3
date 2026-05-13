#include "servidor_busqueda.hpp"
#include <iostream>
#include <chrono>

extern std::mutex coutMutex;

ServidorBusqueda::ServidorBusqueda(unsigned int maxClientesConcurrente, const std::vector<Libro>& libros) : maxClientesConcurrente(maxClientesConcurrente), clientesActivos(0), pararPeticion(false),
contadorTurnoPremium(0), contadorTurnoGratis(0), buscador(libros) {}

bool ServidorBusqueda::hayClientesPendientes() const {
    return !colaGratis.empty() || !colaPremium.empty();
}

bool ServidorBusqueda::escogerPremium() const {
    if (colaPremium.empty()) return false;
    if (colaGratis.empty()) return true;

    if (contadorTurnoPremium < CUOTA_PREMIUM) {
        return true;
    }
    if (contadorTurnoGratis < CUOTA_GRATIS) {
        return false;
    }
    return false;
}

void ServidorBusqueda::actualizarContadoresTurno(bool premium) {
    if (premium) {
        ++contadorTurnoPremium;
    } else {
        ++contadorTurnoGratis;
    }

    if (contadorTurnoPremium >= CUOTA_PREMIUM &&
        contadorTurnoGratis >= CUOTA_GRATIS) {
        contadorTurnoPremium = 0;
        contadorTurnoGratis = 0;
    }
}

void ServidorBusqueda::enviarCliente(const Cliente& cliente) {
    std::lock_guard<std::mutex> lock(colaMutex);

    if (cliente.esGratis()) {
        colaGratis.push_back(cliente);
    } else {
        colaPremium.push_back(cliente);
    }

    cola_cv.notify_one();
}

void ServidorBusqueda::procesarCliente(Cliente cliente) {
    auto inicio = std::chrono::steady_clock::now();

    ResultadoBusquedaCliente resultado = buscador.buscar(cliente, servicioPago);

    auto fin = std::chrono::steady_clock::now();
    resultado.setTiempoTotal(
        std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio)
    );

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Cliente " << cliente.getId()
                  << " buscando palabra: " << cliente.getPalabra() << "\n";
    }

    resultado.print();

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Finalizado cliente " << cliente.getId() << "\n";
    }
}

void ServidorBusqueda::ejecutar() {
    while (true) {
        Cliente cliente(0, "", TipoCliente::GRATIS);

        std::unique_lock<std::mutex> lock(colaMutex);
        cola_cv.wait(lock, [this] {
            return pararPeticion || hayClientesPendientes();
        });

        if (pararPeticion && !hayClientesPendientes()) {
            break;
        }

        active_cv.wait(lock, [this] {
            return clientesActivos < maxClientesConcurrente;
        });

        bool premium = escogerPremium();

        if (premium) {
            cliente = colaPremium.front();
            colaPremium.pop_front();
        } else {
            cliente = colaGratis.front();
            colaGratis.pop_front();
        }

        actualizarContadoresTurno(premium);
        ++clientesActivos;

        lock.unlock();

        std::thread([this, cliente]() mutable {
            procesarCliente(cliente);

            std::lock_guard<std::mutex> guard(colaMutex);
            --clientesActivos;
            active_cv.notify_one();
            cola_cv.notify_one();
        }).detach();
    }
}

void ServidorBusqueda::parar() {
    {
        std::lock_guard<std::mutex> lock(colaMutex);
        pararPeticion = true;
    }

    cola_cv.notify_all();

}