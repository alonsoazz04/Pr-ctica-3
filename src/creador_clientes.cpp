#include "creador_clientes.hpp"

CreadorClientes::CreadorClientes(int limitePalabrasDefecto, int saldoPremiumDefecto) : 
    siguienteId(1), limitePalabrasDefecto(limitePalabrasDefecto), saldoPremiumDefecto(saldoPremiumDefecto) {}

Cliente CreadorClientes::crearClienteGratis(const Diccionario& diccionario) {
    Cliente cliente(siguienteId, diccionario.getPalabraAleatoria(), TipoCliente::GRATIS, 0, limitePalabrasDefecto);
    ++siguienteId;
    return cliente;
}

Cliente CreadorClientes::crearClientePremiumLimitado(const Diccionario& diccionario) {
    Cliente cliente(siguienteId, diccionario.getPalabraAleatoria(), TipoCliente::PREMIUM_LIMITE, saldoPremiumDefecto, 0);
    ++siguienteId;
    return cliente;
}

Cliente CreadorClientes::crearClientePremiumIlimitado(const Diccionario& diccionario) {
    Cliente cliente(siguienteId, diccionario.getPalabraAleatoria(), TipoCliente::PREMIUM_ILIMITADO, 0, 0);
    ++siguienteId;
    return cliente;
}