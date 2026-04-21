#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>

struct ResultadoBusqueda {
    int linea;
    std::string palabraAnterior;
    std::string palabra;
    std::string palabraSiguiente;
};

struct Buscador {
    int id;
    int inicio;
    int fin;
    std::string palabra;
    std::vector<ResultadoBusqueda> resultados;
};

struct ResultadosGlobales {
    std::vector<Buscador> porHilo;
    std::vector<bool> listo;

    std::mutex mtx;
    std::condition_variable cv;

    explicit ResultadosGlobales(int n) : porHilo(n), listo(n, false) {}
};

inline void publish(ResultadosGlobales& s, Buscador local) {
    std::lock_guard<std::mutex> lk(s.mtx);
    int idx = local.id - 1;
    s.porHilo[idx] = std::move(local);
    s.listo[idx] = true;

    s.cv.notify_all();
}

inline void printOrdered(ResultadosGlobales& s, int n) {
    for (int tid = 0; tid < n; ++tid) {
        std::unique_lock<std::mutex> lk(s.mtx);
        s.cv.wait(lk, [&]{ return s.listo[tid]; });

        Buscador b = std::move(s.porHilo[tid]);
        lk.unlock();

        for (const auto& r : b.resultados) {
            std::cout << "Hilo " << b.id
                      << " inicio " << b.inicio
                        << " fin " << b.fin
                        << " línea " << r.linea << " "
                        << r.palabraAnterior << " "
                        << r.palabra << " "
                        << r.palabraSiguiente << std::endl;
        }
    }
}