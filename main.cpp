#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
using namespace std;

// =Clase Jugador (Versión Final Rama B) 
class Jugador {
public:
    string nombre;
    Tablero propio;
    Tablero oponente;
    vector<Barco> barcos;

    Jugador(string n = "") : nombre(n) {}

    // Lectura segura de coordenadas
    int LeerCoordenada(const string& msg) {
        int valor;
        while (true) {
            cout << msg;
            cin >> valor;
            if (cin.fail() || valor < 0 || valor >= Tablero::GetTAM()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR - valor inválido.\n";
                continue;
            }
            return valor;
        }
    }

    // Colocación de barcos
    void ColocarBarcos() {
        cout << "\nColocando barcos para " << nombre << "...\n";
        for (int b = 0; b < 2; b++) {
            int x = LeerCoordenada("  x: ");
            int y = LeerCoordenada("  y: ");
            char orient;
            cout << "  Orientacion (H/V): ";
            cin >> orient;
            bool horizontal = (orient == 'H' || orient == 'h');
            int idBarco = barcos.size();
            Barco nuevo(idBarco, 2);
            if (propio.ColocarBarco(x, y, horizontal, idBarco)) {
                barcos.push_back(nuevo);
                cout << "Barco colocado.\n";
            }
            else {
                cout << "ERROR - No se pudo colocar.\n";
                b--; // repetir intento
            }
        }
    }

    // Acción de disparar contra enemigo
    bool Disparar(Jugador& enemigo, vector<string>& historial) {
        cout << "\nGuia de disparos contra " << enemigo.nombre << ":\n";
        enemigo.oponente.MostrarGuia();

        int x = LeerCoordenada("x: ");
        int y = LeerCoordenada("y: ");

        int id = -1;
        bool ya = false;
        bool acierto = enemigo.propio.Disparar(x, y, id, ya);

        if (ya) {
            cout << "Ya disparaste aquí.\n";
            return false;
        }

        if (acierto) {
            cout << "IMPACTO!\n";
            enemigo.barcos[id].Impactar();
        }
        else {
            cout << "AGUA...\n";
        }

        ostringstream ss;
        ss << "Disparo " << nombre << " a (" << x << "," << y << ")";
        historial.push_back(ss.str());

        return acierto;
    }

    // Validar derrota
    bool HaPerdido() const {
        for (const Barco& b : barcos)
            if (!b.Hundido()) return false;
        return true;
    }
};
