#include "creador_fragmentos.h"


std::vector<Buscador> CreadorFragmentos::crear(int totalLineas, int numHilos, const std::string& palabra) {
    std::vector<Buscador> fragmentos;

    int tamBase = totalLineas / numHilos;
    int resto = totalLineas % numHilos;

    int inicio = 0;

    for (int i = 0; i < numHilos; ++i) {
        int tam = tamBase;
        if (i == numHilos - 1) {
            tam += resto;
        }

        int fin = inicio + tam - 1;

        Buscador b;
        b.id = i + 1;
        b.inicio = inicio;
        b.fin = fin;
        b.palabra = palabra;

        fragmentos.push_back(b);

        inicio = fin + 1;
    }

    return fragmentos;
}