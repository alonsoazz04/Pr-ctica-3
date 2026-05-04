#pragma once

#include <mutex>
#include <condition_variable>
#include "cliente.hpp"

class ServicioPago {
private:
    std::mutex servicioMutex;
    std::condition_variable servicio_cv;
    bool ocupado;

public:
    ServicioPago();

    void recargar(Cliente& cliente, int cantidad);
};