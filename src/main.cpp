#include <iostream>
#include <vector>
#include <thread>
#include <string>

#include "lector_fichero.hpp"
#include "creador_fragmentos.hpp"
#include "worker_buscador.hpp"
#include "estructura.hpp"
#include "cliente.hpp"
#include "diccionario.hpp"
#include "creador_clientes.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <num_clientes> <libro1> [libro2] [libro3] ...\n";
        return 1;
    }

    int numClientes = std::stoi(argv[1]);
    if (numClientes <= 0) {
        std::cerr << "Error: el número de clientes debe ser mayor que 0\n";
        return 1;
    }

    std::vector<std::string> nombresLibros;
    for (int i = 2; i < argc; ++i) {
        nombresLibros.push_back(argv[i]);
    }

    if (nombresLibros.empty()) {
        std::cerr << "Error: debes indicar al menos un libro\n";
        return 1;
    }

    std::vector<std::vector<std::string>> librosLineas;
    for (const auto& nombreLibro : nombresLibros) {
        librosLineas.push_back(LectorFichero::leerLineas(nombreLibro));
    }

    Diccionario diccionario;
    diccionario.aniadirPalabra("amor");
    diccionario.aniadirPalabra("guerra");
    diccionario.aniadirPalabra("rey");
    diccionario.aniadirPalabra("castillo");
    diccionario.aniadirPalabra("muerte");

    CreadorClientes creador(5, 10);
    std::vector<Cliente> clientes;

    for (int i = 0; i < numClientes; ++i) {
        if (i % 3 == 0) {
            clientes.push_back(creador.crearClienteGratis(diccionario));
        } else if (i % 3 == 1) {
            clientes.push_back(creador.crearClientePremiumLimitado(diccionario));
        } else {
            clientes.push_back(creador.crearClientePremiumIlimitado(diccionario));
        }
    }

    for (const auto& cliente : clientes) {
        std::cout << "Cliente " << cliente.getId() << " [" << tipoClienteToString(cliente.getTipoCliente()) << "]" << " buscando palabra: " << cliente.getPalabra() << "\n";

        std::vector<Buscador> buscador = CreadorFragmentos::crear(cliente.getId(), nombresLibros, cliente.getPalabra());

        ResultadosGlobales globales(static_cast<int>(buscador.size()));
        std::vector<std::thread> hilos;

        for (std::size_t i = 0; i < buscador.size(); ++i) {
            hilos.emplace_back(
                WorkerBuscador::ejecutar,
                buscador[i],
                std::cref(librosLineas[i]),
                std::ref(globales)
            );
        }

        for (auto& h : hilos) {
            h.join();
        }

        printOrdered(globales, static_cast<int>(buscador.size()));

        std::cout << "Finalizado cliente " << cliente.getId() << "\n\n";
    }

    return 0;
}