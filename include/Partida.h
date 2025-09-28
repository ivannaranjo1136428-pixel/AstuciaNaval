#ifndef PARTIDA_H
#define PARTIDA_H

#include <string>
#include "Jugador.h"

class Partida {
private:
    Jugador player1_;
    Jugador player2_;
    int turno_;
public:
    Partida();
    void nuevo();
    bool turnoActualDispara(int fila, int col, int &idImpactado);
    int turno() const { return turno_; }
};

#endif // PARTIDA_H
