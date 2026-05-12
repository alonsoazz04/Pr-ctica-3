#pragma once

enum class TipoCliente {
    GRATIS,
    PREMIUM_LIMITE,
    PREMIUM_ILIMITADO 
};

inline std::string tipoClienteToString(TipoCliente tipo) {
    switch (tipo) {
        case TipoCliente::GRATIS:  return "GRATIS";
        case TipoCliente::PREMIUM_LIMITE: return "PREMIUM_LIMITE";
        case TipoCliente::PREMIUM_ILIMITADO: return "PREMIUM_ILIMITADO";
        default:                   return "DESCONOCIDO";
    }
}