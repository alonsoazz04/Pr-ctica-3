#include "servidor_busqueda.hpp"
#include <iostream>
#include <chrono>

ServidorBusqueda::ServidorBusqueda(unsigned int maxClientesConcurrente) : maxClientesConcurrente(maxClientesConcurrente), clientesActivos(0), pararPeticion(false),
contadorTurnoPremium(0), contadorTurnoGratis(0) {}

bool ServidorBusqueda::hayClientesPendientes() const {
    return !colaGratis.empty() || !colaPremium.empty();
}

bool ServidorBusqueda::escogerPremium() const {
    if (colaPremium.empty()) return false;
    if (colaGratis.empty()) return true;

    if (contadorTurnoPremium < CUOTA_PREMIUM) {
        return true;
    }
    return false;
}

void ServidorBusqueda::actualizarContadoresTurno(bool escogerPremium) {
    if (escogerPremium) {
        ++contadorTurnoPremium;
        if (contadorTurnoPremium >= CUOTA_PREMIUM && contadorTurnoGratis >= CUOTA_GRATIS) {
            contadorTurnoPremium = 0;
            contadorTurnoGratis = 0;
        }
    } else {
        ++contadorTurnoGratis;
        if (contadorTurnoPremium >= CUOTA_PREMIUM && contadorTurnoGratis >= CUOTA_GRATIS) {
            contadorTurnoPremium = 0;
            contadorTurnoGratis = 0;
        }
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

Cliente ServidorBusqueda::getSiguienteCliente() {
    Cliente cliente(0, "", TipoCliente::GRATIS);

    if (!colaPremium.empty()) {
        cliente = colaPremium.front();
        colaPremium.pop_front();
    } else if (!colaGratis.empty()) {
        cliente = colaGratis.front();
        colaGratis.pop_front();
    }

    return cliente;

}

void ServidorBusqueda::procesarCliente(Cliente cliente) {

}

void ServidorBusqueda::ejecutar() {
    while(true) {
        Cliente cliente(0, "", TipoCliente::GRATIS);

        {
            std::unique_lock<std::mutex> lock(colaMutex);
            cola_cv.wait(lock, [this] { return pararPeticion || hayClientesPendientes();});

            if (pararPeticion && !hayClientesPendientes()) {
                break;
            }

            active_cv.wait(lock, [this] { return clientesActivos < maxClientesConcurrente;});

            if (!colaPremium.empty()) {
                cliente = colaPremium.front();
                colaPremium.pop_front();
            } else if (!colaGratis.empty()) {
                cliente = colaGratis.front();
                colaGratis.pop_front();
            }

            ++clientesActivos;
        }

        std::thread([this, cliente]() {
            procesarCliente(cliente);

            {
                std::lock_guard<std::mutex> lock(activeMutex);
                if (clientesActivos > 0) {
                    --clientesActivos;
                }
            }

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