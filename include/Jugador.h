#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador {
public:
    Jugador();
    bool dispararAJugador(Jugador &enemigo, int fila, int col, int &idImpactado);
};

#endif // JUGADOR_H
