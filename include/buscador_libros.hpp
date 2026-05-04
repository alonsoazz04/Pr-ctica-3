#pragma once

#include <vector>
#include <string>
#include <atomic>
#include "cliente.hpp"
#include "resultado_busqueda_cliente.hpp"

struct Libro {
    std::string nombre;
    std::string contenido;
};

class BuscadorLibros {
private:
    const std::vector<Libro>& libros;

public:
    explicit BuscadorLibros(const std::vector<Libro>& libros);

    ResultadoBusquedaCliente buscar(const Cliente& cliente) const;
};