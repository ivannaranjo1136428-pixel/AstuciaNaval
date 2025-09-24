#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;

class Barco {
public:
    Barco();
    int coordenadas[2][4]; // [x/y][indice] validar ubicacion
};

Barco::Barco() {
    cout << "Ingrese las coordenadas del barco (4 celdas)\n";
    for (int i = 0; i < 4; i++) {
        cout << "  Posicion " << i << ":\n";
        cout << "   x: "; cin >> coordenadas[0][i];
        cout << "   y: "; cin >> coordenadas[1][i];
    }
}


