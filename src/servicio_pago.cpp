#include "servicio_pago.hpp"
#include <iostream>

ServicioPago::ServicioPago() : ocupado(false) {}

void ServicioPago::recargar(Cliente& cliente, int cantidad) {
    std::unique_lock<std::mutex> lock(servicioMutex);

    servicio_cv.wait(lock, [this] {
        return !ocupado;
    });

    ocupado = true;

    std::cout << "Procesando pago del cliente " << cliente.getId() << std::endl;

    cliente.incrementarSaldo(cantidad);

    std::cout << "Saldo actualizado para el cliente " << cliente.getId() << std::endl;

    ocupado = false;
    lock.unlock();
    servicio_cv.notify_one();
}