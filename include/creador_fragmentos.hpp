#include <vector>
#include <string>
#include "estructura.hpp"

class CreadorFragmentos {
public:
    static std::vector<Buscador> crear(int totalLineas, int numHilos, const std::string& palabra);
};