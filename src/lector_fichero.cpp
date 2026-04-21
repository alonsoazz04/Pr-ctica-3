#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "lector_fichero.h"


std::vector<std::string> LectorFichero::leerLineas(const std::string& nombreFichero) {
    std::ifstream fichero(nombreFichero);
    if (!fichero.is_open()) {
        std::cerr << "No se puede abrir el archivo " << nombreFichero << std::endl;
    }

    std::vector<std::string> lineas;
    std::string linea;

    while (std::getline(fichero, linea)) {
        lineas.push_back(linea);
    }

    return lineas;
}
