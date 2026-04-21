#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class LectorFichero {
    public:
        static std::vector<std::string> leerLineas(const std::string& nombreFichero);
};