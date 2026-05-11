#include <vector>
#include <string>
#include "estructura.hpp"

class CreadorFragmentos {
public:
    static std::vector<Buscador> crear(int idCliente, const std::vector<std::string>& nombreLibros, const std::string& palabra);
};