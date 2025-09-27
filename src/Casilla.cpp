#include "Casilla.h"
#include <sstream>
#include <iostream>

Casilla::Casilla() : ocupado(false), disparado(false), hundido(false), idBarco(-1) {}

void Casilla::ColocarBarco(int id) { ocupado = true; idBarco = id; }
bool Casilla::EstaOcupado() const { return ocupado; }

void Casilla::Disparar() { disparado = true; }
bool Casilla::FueDisparado() const { return disparado; }

int Casilla::GetBarcoID() const { return idBarco; }
void Casilla::SetBarcoID(int id) { idBarco = id; ocupado = (id != -1); }

void Casilla::MarcarHundido() { hundido = true; }
bool Casilla::EstaHundido() const { return hundido; }

void Casilla::Mostrar(bool propio) const {
    std::cout << ToString(propio);
}

void Casilla::MostrarGuia() const {
    std::cout << ToStringGuia();
}

std::string Casilla::ToString(bool propio) const {
    if (hundido) {
        return std::string("\033[45m  \033[0m");
    } else if (disparado) {
        if (ocupado) return std::string("\033[41m  \033[0m");
        else return std::string("\033[44m  \033[0m");
    } else {
        if (propio && ocupado) return std::string("\033[42m  \033[0m");
        else return std::string("\033[47m  \033[0m");
    }
}

std::string Casilla::ToStringGuia() const {
    if (hundido) return std::string("\033[45m  \033[0m");
    else if (disparado) {
        if (ocupado) return std::string("\033[41m  \033[0m");
        else return std::string("\033[44m  \033[0m");
    } else {
        return std::string("\033[47m  \033[0m");
    }
}

void Casilla::Serializar(std::ofstream &out) const {
    out.write((char*)&ocupado, sizeof(ocupado));
    out.write((char*)&disparado, sizeof(disparado));
    out.write((char*)&hundido, sizeof(hundido));
    out.write((char*)&idBarco, sizeof(idBarco));
}

void Casilla::Deserializar(std::ifstream &in) {
    in.read((char*)&ocupado, sizeof(ocupado));
    in.read((char*)&disparado, sizeof(disparado));
    in.read((char*)&hundido, sizeof(hundido));
    in.read((char*)&idBarco, sizeof(idBarco));
}
