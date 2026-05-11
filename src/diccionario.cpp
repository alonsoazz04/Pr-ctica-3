#include "diccionario.hpp"
#include <iostream>
#include <random>

Diccionario::Diccionario() {}

const std::vector<std::string>& Diccionario::getPalabras() const {
    return palabras;
}

bool Diccionario::vacio() const {
    return palabras.empty();
}

void Diccionario::aniadirPalabra(const std::string& palabra) {
    palabras.push_back(palabra);

}

std::string Diccionario::getPalabraAleatoria() const {
    if (palabras.empty()) {
        std::cerr << "El diccionario está vacío." << std::endl;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, palabras.size() - 1);

    return palabras[dist(gen)];
}