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
