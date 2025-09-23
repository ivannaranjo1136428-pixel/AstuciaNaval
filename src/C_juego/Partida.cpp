#include "C_juego/Partida.h"

Partida::Partida(): player1_(), player2_(), turno_(1) {}

void Partida::nuevo() {
    turno_ = 1;
    // Si quieren: aquí inicializar barcos por defecto para pruebas.
}

bool Partida::turnoActualDispara(int fila, int col, int &idImpactado) {
    bool res = false;
    if (turno_ == 1) {
        res = player1_.dispararAJugador(const_cast<Jugador&>(player2_), fila, col, idImpactado);
        turno_ = 2;
    } else {
        res = player2_.dispararAJugador(const_cast<Jugador&>(player1_), fila, col, idImpactado);
        turno_ = 1;
    }
    return res;
}
