#ifndef PARTIDA_H
#define PARTIDA_H

#include "B_jugador/Jugador.h"

class Partida {
    Jugador player1_;
    Jugador player2_;
    int turno_; // 1 = player1, 2 = player2
public:
    Partida();
    void nuevo(); // inicializa partida
    // El jugador del turno actual dispara a (fila,col).
    // Devuelve true si hubo impacto; idImpactado contiene id del barco o -1.
    bool turnoActualDispara(int fila, int col, int &idImpactado);

    // accesores simples (útiles para pruebas)
    int turno() const { return turno_; }
    const Jugador& jugador1() const { return player1_; }
    const Jugador& jugador2() const { return player2_; }
};
#endif // PARTIDA_H
