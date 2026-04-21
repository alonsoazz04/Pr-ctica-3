#include "worker_buscador.hpp"
#include "procesador_linea.hpp"


void WorkerBuscador::ejecutar(Buscador local, const std::vector<std::string>& lineas, ResultadosGlobales& globales) {
    for (int i = local.inicio; i <= local.fin; ++i) {
        std::vector<ResultadoBusqueda> encontrados = ProcesadorLinea::buscarEnLinea(lineas[i], local.palabra, i+1);
        local.resultados.insert(local.resultados.end(), encontrados.begin(), encontrados.end());
    }

    publish(globales, std::move(local));
}  
