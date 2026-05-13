#include "resultado_busqueda_cliente.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

extern std::mutex coutMutex;

ResultadoBusquedaCliente::ResultadoBusquedaCliente() {}

void ResultadoBusquedaCliente::aniadirBusqueda(const std::string& nombreLibro, int numeroLinea, int posicionPalabra) {
    busquedas.emplace_back(nombreLibro, numeroLinea, posicionPalabra);
}

void ResultadoBusquedaCliente::setTiempoTotal(std::chrono::milliseconds tiempo) {
    tiempoTotal = tiempo;
}

const std::vector<Busqueda>& ResultadoBusquedaCliente::getBusquedas() const {
    return busquedas;
}

std::chrono::milliseconds ResultadoBusquedaCliente::getTiempoTotal() const {
    return tiempoTotal;
}

void ResultadoBusquedaCliente::print() const {
    std::lock_guard<std::mutex> lock(coutMutex);

    std::cout << "Resultados encontrados: " << busquedas.size() << std::endl;
    std::cout << "Tiempo total: " << tiempoTotal.count() << " ms" << std::endl;

    if (!busquedas.empty()) {
        std::cout << "Libros encontrados: " << std::endl;
        for (const auto& busqueda : busquedas) {
            std::cout << busqueda.nombreLibro << " (línea " << busqueda.numeroLinea << ", posicion " << busqueda.posicionPalabra << ")" << std::endl;
        }
    }
}

void ResultadoBusquedaCliente::guardarArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "Error: no se pudo crear el archivo " << nombreArchivo << std::endl;
        return;
    }

    archivo << "RESULTADOS DE BUSQUEDA\n";
    archivo << "Palabras encontradas: " << busquedas.size() << "\n";
    archivo << "Tiempo total: " << tiempoTotal.count() << " ms\n\n";

    for (const auto& busqueda : busquedas) {
        archivo << busqueda.nombreLibro << " (línea " << busqueda.numeroLinea << ", posicion " << busqueda.posicionPalabra <<  ")" << std::endl;
    }

    std::cout << "Resultados guardados en " << nombreArchivo << "\n";
}