#ifndef JUGADOR_H  
#define JUGADOR_H 
 
class Jugador { 
public: 
    Jugador(); 
    bool dispararAJugador(Jugador &enemigo, int fila, int col, int &idImpactado); 
}; 
 
#endif // JUGADOR_H
