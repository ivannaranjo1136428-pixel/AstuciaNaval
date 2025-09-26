#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;

// ==================== Clase Jugador ====================
class Jugador {
public:
    string nombre;
    Tablero propio;
    Tablero guia;
    vector<Barco> barcos;

    Jugador(string n = "") : nombre(n) {}

    // 🔹 Leer coordenadas con validación
    int LeerNumero(string msg) {
        int num;
        while (true) {
            cout << msg;
            if (cin >> num && num >= 0 && num < Tablero::GetTAM()) {
                return num;
            }
            else {
                cout << "⚠️ Entrada inválida. Ingresa un número entre 0 y " << Tablero::GetTAM() - 1 << ".\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // 🔹 Leer orientación con validación
    char LeerOrientacion() {
        char orient;
        while (true) {
            cout << "Orientacion (H=Horizontal, V=Vertical): ";
            if (cin >> orient) {
                if (orient == 'H' || orient == 'h' || orient == 'V' || orient == 'v') {
                    return orient;
                }
            }
            cout << "⚠️ Entrada inválida. Solo puedes ingresar H o V.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void ColocarBarcos() {
        cout << "\n>>> Turno de " << nombre << " para colocar sus barcos.\n";
        for (int b = 0; b < 3; b++) {
            cout << "\nBarco #" << (b + 1) << " de " << nombre << ":\n";

            while (true) {
                int x = LeerNumero("  x: ");
                int y = LeerNumero("  y: ");
                char orient = LeerOrientacion();

                bool horizontal = (orient == 'H' || orient == 'h');
                int idBarco = barcos.size();
                Barco nuevo(idBarco, 4);

                if (propio.ColocarBarco(x, y, horizontal, idBarco)) {
                    cout << "✅ Barco colocado con éxito.\n";
                    barcos.push_back(nuevo);
                    propio.Mostrar(true);
                    break;
                }
                else {
                    cout << "⚠️ Posición no válida. Elige otra.\n";
                }
            }
        }

        // 🔹 Ocultar tablero después de ubicar barcos
        cout << "\n✅ " << nombre << " ha terminado de ubicar todos sus barcos.\n";
        cout << "🔒 Ocultando el tablero...\n";
        cout << "👉 Presiona ENTER y entrega el turno al siguiente jugador.\n";
        cin.ignore();
        cin.get();

        // Simular limpiar pantalla (imprimir muchas líneas)
        for (int i = 0; i < 50; i++) cout << "\n";
    }

    int BarcosRestantes() const {
        int vivos = 0;
        for (const Barco& b : barcos)
            if (!b.Hundido()) vivos++;
        return vivos;
    }

    bool Disparar(Jugador& enemigo) {
        while (true) {
            cout << "\nGuía de disparos contra " << enemigo.nombre << ":\n";
            guia.MostrarGuia();

            int x = LeerNumero(" x: ");
            int y = LeerNumero(" y: ");

            int idImpactado = -1;
            bool yaDisparado = false;
            bool acierto = enemigo.propio.Disparar(x, y, idImpactado, yaDisparado);

            if (yaDisparado) {
                cout << "⚠️ Ya disparaste en (" << x << "," << y << "). Elige otra posición.\n";
                continue;
            }

            guia.RegistrarDisparo(x, y, acierto, idImpactado);

            if (acierto) {
                cout << "🎯 Impacto!\n";
                enemigo.barcos[idImpactado].Impactar();
                if (enemigo.barcos[idImpactado].Hundido()) {
                    cout << "💥 Barco de " << enemigo.nombre << " destruido!\n";
                }
            }
            else {
                cout << "🌊 Agua...\n";
            }

            cout << "👉 A " << enemigo.nombre << " le quedan "
                << enemigo.BarcosRestantes() << " barcos por destruir.\n";

            return acierto;
        }
    }

    bool HaPerdido() const {
        for (const Barco& b : barcos)
            if (!b.Hundido()) return false;
        return true;
    }
};



