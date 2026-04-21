#include <string>
#include <vector>
#include <sstream>
#include "estructura.hpp"

class ProcesadorLinea {
    public:
        static std::vector<std::string> tokenizar(const std::string& linea);

        static std::vector<ResultadoBusqueda> buscarEnLinea(const std::string& textoLinea, const std::string& palabraBuscada, int numLinea);
};