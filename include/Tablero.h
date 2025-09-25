#ifndef TABLERO_H
#define TABLERO_H

#include "Casilla.h"
#include <string>
#include <fstream>

class Tablero {
    static const int TAM = 20;
    Casilla celdas[TAM][TAM];
public:
    static int GetTAM();
    bool PuedeColocar(int x, int y) const;
    bool ColocarBarco(int x, int y, bool horizontal, int idBarco, int tam = 4);
    bool Disparar(int x, int y, int &idImpactado, bool &yaDisparado);
    void MarcarBarcoHundido(int idBarco);
    void SetBarcoID(int x, int y, int id);

    void Mostrar(bool propio) const;
    void MostrarGuia() const;

    std::string ToString(bool propio) const;
    std::string ToStringGuia() const;

    void Serializar(std::ofstream &out) const;
    void Deserializar(std::ifstream &in);
};

#endif // TABLERO_H
