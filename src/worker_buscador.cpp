#include "worker_buscador.hpp"
#include "procesador_linea.hpp"

void WorkerBuscador::ejecutar(Buscador local, const std::vector<std::string>& lineas, ResultadosGlobales& globales) {
    for (std::size_t i = 0; i < lineas.size(); ++i) {
        std::vector<ResultadoBusqueda> encontrados =
            ProcesadorLinea::buscarEnLinea(lineas[i], local.palabra, static_cast<int>(i + 1));

        for (auto& resultado : encontrados) {
            resultado.libro = local.libro;
            local.resultados.push_back(resultado);
        }
    }

    publish(globales, std::move(local));
}