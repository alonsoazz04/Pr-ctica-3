#include "servicio_pago.hpp"
#include <iostream>
#include <mutex>

std::mutex coutMutex;

ServicioPago::ServicioPago() : ocupado(false) {}

void ServicioPago::recargar(Cliente& cliente, int cantidad) {
    std::unique_lock<std::mutex> lock(servicioMutex);

    servicio_cv.wait(lock, [this] {
        return !ocupado;
    });

    ocupado = true;

    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "Procesando pago del cliente " << cliente.getId() << std::endl;
    }

    cliente.incrementarSaldo(cantidad);

    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "Saldo actualizado para el cliente " << cliente.getId() << ". Nuevo saldo: " << cliente.getSaldo() << std::endl;
    }

    ocupado = false;
    lock.unlock();
    servicio_cv.notify_one();
}