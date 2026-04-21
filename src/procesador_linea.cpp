#include <string>
#include <vector>
#include <sstream>
#include "procesador_linea.h"

std::vector<std::string> ProcesadorLinea::tokenizar(const std::string& linea) {
    std::istringstream iss(linea);
    std::vector<std::string> palabras;
    std::string token;

    while (iss >> token) {
        palabras.push_back(token);
    }

    return palabras;
}

std::vector<ResultadoBusqueda> ProcesadorLinea::buscarEnLinea(const std::string& textoLinea, const std::string& palabraBuscada, int numLinea) {
    std::vector<ResultadoBusqueda> encontrados;
    std::vector<std::string> palabras = tokenizar(textoLinea);

    for (size_t i = 0; i < palabras.size(); ++i) {
        if (palabras[i] == palabraBuscada) {
            ResultadoBusqueda r;
            r.linea = numLinea;
            r.palabraAnterior = (i > 0) ? palabras[i - 1] : "...";
            r.palabra = palabras[i];
            r.palabraSiguiente = (i + 1 < palabras.size()) ? palabras[i + 1] : "...";
            encontrados.push_back(r);
        }
    }

    return encontrados;
}