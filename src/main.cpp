#include <iostream>
#include "C_juego/Partida.h"

int main() {
    Partida p;
    p.nuevo();
    std::cout << "Partida inicializada (demo).\n";
    std::cout << "Turno inicial: " << p.turno() << "\n";

    // Ejemplo rápido: intentar disparo (0,0)
    int id;
    bool impacto = p.turnoActualDispara(0, 0, id);
    std::cout << "Disparo en (0,0): " << (impacto ? "Impacto, id=" + std::to_string(id) : "Agua") << "\n";
    std::cout << "Turno ahora: " << p.turno() << "\n";

    return 0;
}
