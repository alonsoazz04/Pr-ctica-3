#include "cliente.h"
#include <iostream>

Cliente::Cliente(int id, const std::string& palabra, TipoCliente tipo, int saldo, int limitePalabras)
    : id(id), palabra(palabra), tipo(tipo), saldo(saldo), limitePalabras(limitePalabras) {}

int Cliente::getId() const {
    return id;
}

const std::string& Cliente::getPalabra() const {
    return palabra;
}

int Cliente::getSaldo() const {
    return saldo;
}

int Cliente::getLimitePalabras() const {
    return limitePalabras;
}

bool Cliente::esGratis() const {
    return tipo == TipoCliente::GRATIS;
}

bool Cliente::esPremiumLimite() const {
    return tipo == TipoCliente::PREMIUM_LIMITE;
}

bool Cliente::esPremiumIlimitado() const {
    return tipo == TipoCliente::PREMIUM_ILIMITADO;
}

void Cliente::setSaldo(int nuevoSaldo) {
    saldo = nuevoSaldo;
}

void Cliente::incrementarSaldo(int cantidad) {
    saldo += cantidad;
}

void Cliente::consumirCredito() {
    if (saldo <= 0) {
        std::cerr << "El cliente no tiene saldo." << std::endl;
    }

    --saldo;
}