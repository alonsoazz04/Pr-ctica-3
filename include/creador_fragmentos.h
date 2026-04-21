#include <vector>
#include <string>
#include "estructura.h"

class CreadorFragmentos {
public:
    static std::vector<Buscador> crear(int totalLineas, int numHilos, const std::string& palabra);
};