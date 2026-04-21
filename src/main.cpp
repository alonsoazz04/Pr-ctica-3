#include <iostream>
#include <vector>
#include <thread>

#include "lector_fichero.h"
#include "creador_fragmentos.h"
#include "worker_buscador.h"
#include "estructura.h"

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cerr << "Error en el número de argumentos" << std::endl;
        return 1;
    }

    std::string nombreFichero = argv[1];
    std::string palabra = argv[2];
    int numHilos = std::stoi(argv[3]);

    if (numHilos <= 0) {
        std::cerr << "Error: el número de hilos debe ser mayor que 0" << std::endl;
        return 1;
    }

    std::vector<std::string> lineas = LectorFichero::leerLineas(nombreFichero);

    std::vector<Buscador> fragmentos = CreadorFragmentos::crear(lineas.size(), numHilos, palabra);

    ResultadosGlobales globales(numHilos);
    std::vector<std::thread> hilos;

    for (const auto& buscador : fragmentos) {
        hilos.emplace_back(WorkerBuscador::ejecutar, buscador, std::cref(lineas), std::ref(globales));
    }

    for (auto& h : hilos) {
        h.join();
    }

    printOrdered(globales, numHilos);
}