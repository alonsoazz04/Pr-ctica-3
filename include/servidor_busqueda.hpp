#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cstddef>

#include "cliente.hpp"
#include "resultado_busqueda_cliente.hpp"
#include "diccionario.hpp"

class ServidorBusqueda {
private:
    std::deque<Cliente> colaPremium;
    std::deque<Cliente> colaGratis;
    std::mutex colaMutex;
    std::condition_variable cola_cv;
    std::mutex activeMutex;
    std::condition_variable active_cv;
    unsigned int maxClientesConcurrente;
    unsigned int clientesActivos;
    bool pararPeticion;
    int contadorTurnoPremium;
    int contadorTurnoGratis;
    static constexpr int CUOTA_PREMIUM = 8;
    static constexpr int CUOTA_GRATIS = 2;

private:
    bool hayClientesPendientes() const;
    Cliente getSiguienteCliente();
    void procesarCliente(Cliente cliente);
    bool escogerPremium() const;
    void actualizarContadoresTurno(bool escogerPremium);

public:
    explicit ServidorBusqueda(unsigned int maxClientesConcurrente);

    void enviarCliente(const Cliente& cliente);
    void ejecutar();
    void parar();
};