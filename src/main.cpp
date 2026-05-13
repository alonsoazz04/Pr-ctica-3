#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>

#include "lector_fichero.hpp"
#include "diccionario.hpp"
#include "creador_clientes.hpp"
#include "servidor_busqueda.hpp"
#include "tipo_cliente.hpp"
#include "buscador_libros.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Error en el número de argumentos" << std::endl;
        return 1;
    }

    int numClientes = std::stoi(argv[1]);
    int maxConcurrentes = std::stoi(argv[2]);

    if (numClientes <= 0 || maxConcurrentes <= 0) {
        std::cerr << "Error: numClientes y maxConcurrentes deben ser mayores que 0\n";
        return 1;
    }

    std::vector<std::string> nombresLibros;
    for (int i = 3; i < argc; ++i) {
        nombresLibros.push_back(argv[i]);
    }

    if (nombresLibros.empty()) {
        std::cerr << "Error: debes indicar al menos un libro\n";
        return 1;
    }

    std::vector<Libro> libros;
    for (const auto& nombreLibro : nombresLibros) {
        std::vector<std::string> lineas = LectorFichero::leerLineas(nombreLibro);

        std::string contenido;
        for (const auto& linea : lineas) {
            contenido += linea + "\n";
        }

        libros.push_back({nombreLibro, contenido});
    }

    Diccionario diccionario;
    diccionario.aniadirPalabra("amor");
    diccionario.aniadirPalabra("guerra");
    diccionario.aniadirPalabra("rey");
    diccionario.aniadirPalabra("castillo");
    diccionario.aniadirPalabra("muerte");

    CreadorClientes creador(5, 10);
    ServidorBusqueda servidor(static_cast<unsigned int>(maxConcurrentes), libros);

    std::thread hiloServidor(&ServidorBusqueda::ejecutar, &servidor);

    for (int i = 0; i < numClientes; ++i) {
        Cliente cliente(0, "", TipoCliente::GRATIS);

        if (i % 3 == 0) {
            cliente = creador.crearClienteGratis(diccionario);
        } else if (i % 3 == 1) {
            cliente = creador.crearClientePremiumLimitado(diccionario);
        } else {
            cliente = creador.crearClientePremiumIlimitado(diccionario);
        }

        std::cout << "Cliente " << cliente.getId()
                  << " [" << tipoClienteToString(cliente.getTipoCliente()) << "] "
                  << "buscando palabra: " << cliente.getPalabra() << '\n';

        servidor.enviarCliente(cliente);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    servidor.parar();
    hiloServidor.join();

    return 0;
}