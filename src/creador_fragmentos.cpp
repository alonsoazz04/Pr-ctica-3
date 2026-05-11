#include "creador_fragmentos.hpp"

std::vector<Buscador> CreadorFragmentos::crear(
    int idCliente,
    const std::vector<std::string>& nombresLibros,
    const std::string& palabra
) {
    std::vector<Buscador> tareas;

    for (std::size_t i = 0; i < nombresLibros.size(); ++i) {
        Buscador tarea;
        tarea.idCliente = idCliente;
        tarea.idWorker = static_cast<int>(i + 1);
        tarea.libro = nombresLibros[i];
        tarea.palabra = palabra;

        tareas.push_back(tarea);
    }

    return tareas;
}