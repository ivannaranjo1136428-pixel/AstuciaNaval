#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
using namespace std;



class Barco {
    int id;
    int size;
    int impactos;
public:
    Barco(int i = 0, int s = 0) : id(i), size(s), impactos(0) {}
    void Impactar() { impactos++; }
    bool Hundido() const { return impactos >= size; }
};

class Tablero {
public:
    static int GetTAM() { return 20; }
    bool ColocarBarco(int, int, bool, int) { return true; }
    bool Disparar(int, int, int& idImpactado, bool& yaDisparado) {
        yaDisparado = false;
        idImpactado = 0;
        return (rand() % 2 == 0); // aleatorio acierto/fallo
    }
    void Mostrar(bool) {}
    void SetBarcoID(int, int, int) {}
    void MarcarBarcoHundido(int) {}
    string ToStringGuia() const { return "[Guia dummy]\n"; }
};

//  Clase Jugador 
class Jugador {
public:
    string nombre;
    Tablero propio;
    Tablero oponente;
    vector<Barco> barcos;

    Jugador(string n = "") : nombre(n) {}

    int LeerCoordenada(const string& msg) {
        int valor;
        while (true) {
            cout << msg;
            cin >> valor;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR - Debe ingresar un numero.\n";
                continue;
            }
            if (valor < 0 || valor >= Tablero::GetTAM()) {
                cout << "ERROR - Valor fuera de rango (0-19).\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
    }

    void ColocarBarcos() {
        cout << "\n>>> " << nombre << " coloca barcos (dummy)\n";
        for (int b = 0; b < 3; ++b) {
            barcos.push_back(Barco(b, 4));
        }
    }

    int BarcosRestantes() const {
        int vivos = 0;
        for (const Barco& b : barcos) if (!b.Hundido()) vivos++;
        return vivos;
    }

    bool Disparar(Jugador& enemigo, vector<string>& historial) {
        cout << "\n" << nombre << " dispara a " << enemigo.nombre << "...\n";
        int x = LeerCoordenada(" x: ");
        int y = LeerCoordenada(" y: ");
        int idImpactado = -1;
        bool yaDisparado = false;
        bool acierto = enemigo.propio.Disparar(x, y, idImpactado, yaDisparado);

        if (acierto) {
            cout << "IMPACTO!\n";
            enemigo.barcos[idImpactado].Impactar();
        }
        else {
            cout << "AGUA...\n";
        }
        return acierto;
    }

    bool HaPerdido() const {
        for (const Barco& b : barcos) if (!b.Hundido()) return false;
        return true;
    }
};

//  MAIN DE PRUEBA 
int main() {
    Jugador jugadorA("Jugador A");
    Jugador jugadorB("Jugador B");

    jugadorA.ColocarBarcos();
    jugadorB.ColocarBarcos();

    vector<string> historial;

    jugadorA.Disparar(jugadorB, historial);
    jugadorB.Disparar(jugadorA, historial);

    cout << "\nFin de prueba. Barcos A restantes: " << jugadorA.BarcosRestantes() << "\n";
    cout << "Barcos B restantes: " << jugadorB.BarcosRestantes() << "\n";

    return 0;
}




