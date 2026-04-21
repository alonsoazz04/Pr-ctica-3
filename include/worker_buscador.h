#pragma once
#include <vector>
#include <string>
#include "estructura.h"

class WorkerBuscador {
    public:
        static void ejecutar(Buscador local, const std::vector<std::string>& lineas, ResultadosGlobales& globales);
};