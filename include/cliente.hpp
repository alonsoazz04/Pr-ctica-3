#pragma once 

#include <string>
#include "tipo_cliente.hpp"

class Cliente {
    private:
        int id;
        std::string palabra;
        TipoCliente tipo;
        int saldo;
        int limitePalabras;
    
    public:
        Cliente(int id, const std::string& palabra, TipoCliente tipo, int saldo = 0, int limitePalabras = 0);

        int getId() const;
        const std::string& getPalabra() const;
        int getSaldo() const;
        int getLimitePalabras() const;

        bool esGratis() const;
        bool esPremiumLimite() const;
        bool esPremiumIlimitado() const;

        bool hasSaldo() const;

        void setPalabra(const std::string& nuevaPalabra);
        void setSaldo(int nuevoSaldo);
        void setLimitePalabras(int nuevoLimite);
        
        void incrementarSaldo(int cantidad);
        void consumirCredito();

};