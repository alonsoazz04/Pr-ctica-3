#pragma once

#include "cliente.hpp"
#include "diccionario.hpp"

class CreadorClientes {
private:
    int siguienteId;
    int limitePalabrasDefecto;
    int saldoPremiumDefecto;

public:
    CreadorClientes(int defaultFreeWord = 5, int defaultPremiumBalance = 10);

    Cliente crearClienteGratis(const Diccionario& diccionario);
    Cliente crearClientePremiumLimitado(const Diccionario& diccionario);
    Cliente crearClientePremiumIlimitado(const Diccionario& diccionario);
};