#ifndef PARTIDA_H
#define PARTIDA_H

#include <string>
#include <vector>

class Jugador; // forward

class Partida {
public:
    static void Guardar(const Jugador& j1, const Jugador& j2, const std::string& turno, const std::vector<std::string>& historial);
    static bool Cargar(Jugador& j1, Jugador& j2, std::string& turno, std::vector<std::string>& historial);
};

#endif // PARTIDA_H
#include "Partida.h"
#include "Jugador.h"
#include <fstream>
#include <iostream>

void Partida::Guardar(const Jugador& j1, const Jugador& j2, const std::string& turno, const std::vector<std::string>& historial) {
    std::ofstream out("partida.dat", std::ios::binary);
    if (!out) {
        std::cerr << "ERROR - No se pudo abrir archivo para guardar.\n";
        return;
    }
    j1.Serializar(out);
    j2.Serializar(out);

    size_t len = turno.size();
    out.write((char*)&len, sizeof(len));
    out.write(turno.c_str(), len);

    size_t h = historial.size();
    out.write((char*)&h, sizeof(h));
    for (const std::string& s : historial) {
        size_t l = s.size();
        out.write((char*)&l, sizeof(l));
        out.write(s.c_str(), l);
    }
}

bool Partida::Cargar(Jugador& j1, Jugador& j2, std::string& turno, std::vector<std::string>& historial) {
    std::ifstream in("partida.dat", std::ios::binary);
    if (!in) return false;

    j1.Deserializar(in);
    j2.Deserializar(in);

    size_t len;
    in.read((char*)&len, sizeof(len));
    turno.resize(len);
    in.read(&turno[0], len);

    size_t h;
    in.read((char*)&h, sizeof(h));
    historial.resize(h);
    for (size_t i = 0; i < h; ++i) {
        size_t l;
        in.read((char*)&l, sizeof(l));
        historial[i].resize(l);
        in.read(&historial[i][0], l);
    }
    return true;
}
#ifndef JUEGO_H
#define JUEGO_H

#include <string>
#include <vector>
#include "Jugador.h"

class Juego {
public:
    Jugador j1, j2;
    std::string turno;
    std::vector<std::string> historial;

    Juego();
    Juego(const std::string& n1, const std::string& n2);
    Juego(const Jugador& p1, const Jugador& p2, const std::string& turn, const std::vector<std::string>& hist);

    void Iniciar();
    static void MostrarHistorial(const std::vector<std::string>& h);
};

#endif // JUEGO_H
#include "Juego.h"
#include "Partida.h"
#include <iostream>
#include <limits>

Juego::Juego() {}

Juego::Juego(const std::string& n1, const std::string& n2) : j1(n1), j2(n2), turno(n1) {}

Juego::Juego(const Jugador& p1, const Jugador& p2, const std::string& turn, const std::vector<std::string>& hist)
    : j1(p1), j2(p2), turno(turn), historial(hist) {
}

void Juego::MostrarHistorial(const std::vector<std::string>& h) {
    std::cout << "\n=== HISTORIAL DE PARTIDA ===\n";
    for (size_t i = 0; i < h.size(); ++i) {
        std::cout << "---- Jugada " << (i + 1) << " ----\n";
        std::cout << h[i] << "\n";
    }
}

void Juego::Iniciar() {
    std::cout << "=== ASTUCIA NAVAL ===\n";

    if (j1.barcos.empty()) j1.ColocarBarcos();
    if (j2.barcos.empty()) j2.ColocarBarcos();

    Jugador* turnoPtr = (turno == j1.nombre) ? &j1 : &j2;
    Jugador* enemigoPtr = (turnoPtr == &j1) ? &j2 : &j1;

    while (true) {
        std::cout << "\nTurno de " << turnoPtr->nombre << "\n";

        bool acierto;
        do {
            acierto = turnoPtr->Disparar(*enemigoPtr, historial);
            Partida::Guardar(j1, j2, turnoPtr->nombre, historial);

            if (enemigoPtr->HaPerdido()) {
                std::cout << "\nVICTORIA - " << turnoPtr->nombre << " ha ganado la partida!\n";
                return;
            }

            if (acierto) {
                std::cout << "REPITE - " << turnoPtr->nombre << " acerto y repite disparo.\n";
            }

        } while (acierto);

        if (turnoPtr == &j1) {
            turnoPtr = &j2;
            enemigoPtr = &j1;
        }
        else {
            turnoPtr = &j1;
            enemigoPtr = &j2;
        }
        turno = turnoPtr->nombre;
    }
}
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <vector>

#include "Jugador.h"
#include "Juego.h"
#include "Partida.h"

int main() {
    std::ifstream test("partida.dat", std::ios::binary);
    if (test) {
        char op;
        std::cout << "Se encontro una partida guardada. Desea cargarla? (S/N): ";
        std::cin >> op;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (op == 'S' || op == 's') {
            Jugador j1, j2;
            std::string turno;
            std::vector<std::string> historial;
            if (Partida::Cargar(j1, j2, turno, historial)) {
                char h;
                std::cout << "Desea ver el historial? (S/N): ";
                std::cin >> h;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (h == 'S' || h == 's') {
                    Juego::MostrarHistorial(historial);
                }
                Juego juegoCargado(j1, j2, turno, historial);
                juegoCargado.Iniciar();
                return 0;
            }
            else {
                std::cout << "No se pudo cargar la partida.\n";
            }
        }
    }

    std::string n1, n2;
    std::cout << "Nombre del Jugador 1: ";
    std::getline(std::cin, n1);
    if (n1.empty()) { std::getline(std::cin, n1); }

    std::cout << "Nombre del Jugador 2: ";
    std::getline(std::cin, n2);
    if (n2.empty()) { std::getline(std::cin, n2); }

    Juego juego(n1, n2);
    juego.Iniciar();
    return 0;
}
