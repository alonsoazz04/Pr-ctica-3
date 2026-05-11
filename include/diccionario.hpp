#pragma once

#include <string>
#include <vector>

class Diccionario {
    private:
        std::vector<std::string> palabras;
    
    public:
        Diccionario();

        const std::vector<std::string>& getPalabras() const;

        bool vacio() const;
        void aniadirPalabra(const std::string& palabra);
        std::string getPalabraAleatoria() const;

};