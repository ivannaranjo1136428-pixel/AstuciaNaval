#include <iostream>
#include "Tablero.h"

int main() {
    Tablero tablero;

    std::cout << "Prueba: colocar un barco horizontal en (0,0)...\n";
    bool colocado = tablero.ColocarBarco(0, 0, true, 1, 3);
    std::cout << (colocado ? "Barco colocado correctamente" : "Error al colocar barco") << std::endl;

    std::cout << "\nEstado del tablero propio:\n";
    tablero.Mostrar(true);

    int idImpactado = -1;
    bool yaDisparado = false;

    std::cout << "\nPrueba: disparar a (0,0)...\n";
    bool acierto = tablero.Disparar(0, 0, idImpactado, yaDisparado);
    std::cout << (acierto ? "IMPACTO" : "AGUA") << " en barco id " << idImpactado << std::endl;

    std::cout << "\nGuia de disparos:\n";
    tablero.MostrarGuia();

    return 0;
}
