#include "Jugador.h"  
 
Jugador::Jugador() {} 
 
bool Jugador::dispararAJugador(Jugador &/*enemigo*/, int /*fila*/, int /*col*/, int &idImpactado) { 
    idImpactado = -1; 
    return false; // stub: sin impacto por ahora 
}
