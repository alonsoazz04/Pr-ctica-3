#pragma once

#include <string>
#include <vector>
#include <chrono>

struct Busqueda {
    std::string nombreLibro;
    int numeroLinea;
    int posicionPalabra;
};

class ResultadoBusquedaCliente {
private:
    std::vector<Busqueda> busquedas;
    std::chrono::milliseconds tiempoTotal;

public:
    ResultadoBusquedaCliente();

    void aniadirBusqueda(const std::string& nombreLibro, int numeroLinea, int posicionPalabra);
    void setTiempoTotal(std::chrono::milliseconds tiempo);

    const std::vector<Busqueda>& getBusquedas() const;
    std::chrono::milliseconds getTiempoTotal() const;

    void print() const;
    void guardarArchivo(const std::string& nombreArchivo) const;
    
};