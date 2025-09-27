#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// ==================== Colores ANSI ====================
#define RESET   "\033[0m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AZUL    "\033[34m"
#define BLANCO  "\033[37m"

// ==================== Clases mínimas de apoyo ====================
class Barco {
public:
    int id;
    int tam;
    int impactos;

    Barco(int i = 0, int t = 4) : id(i), tam(t), impactos(0) {}
    void Impactar() { impactos++; }
    bool Hundido() const { return impactos >= tam; }
};

class Casilla {
    bool ocupado;
    bool disparado;
    int idBarco;
public:
    Casilla() : ocupado(false), disparado(false), idBarco(-1) {}
    void ColocarBarco(int id) { ocupado = true; idBarco = id; }
    bool EstaOcupado() const { return ocupado; }
    void Disparar() { disparado = true; }
    bool FueDisparado() const { return disparado; }
    int GetBarcoID() const { return idBarco; }
    void MostrarGuia() const {
        if (disparado) {
            if (ocupado) cout << ROJO << "■ " << RESET;
            else cout << AZUL << "■ " << RESET;
        }
        else {
            cout << BLANCO << ". " << RESET;
        }
    }
    void Mostrar(bool propio) const {
        if (disparado) {
            if (ocupado) cout << ROJO << "■ " << RESET;
            else cout << AZUL << "■ " << RESET;
        }
        else {
            if (propio) {
                if (ocupado) cout << VERDE << "■ " << RESET;
                else cout << BLANCO << ". " << RESET;
            }
            else {
                cout << BLANCO << ". " << RESET;
            }
        }
    }
};

class Tablero {
    static const int TAM = 20;
    Casilla celdas[TAM][TAM];
public:
    static int GetTAM() { return TAM; }
    bool PuedeColocar(int x, int y) const {
        return x >= 0 && x < TAM && y >= 0 && y < TAM && !celdas[y][x].EstaOcupado();
    }
    bool ColocarBarco(int x, int y, bool horizontal, int idBarco, int tam = 4) {
        if (horizontal) {
            if (x + tam > TAM) return false;
            for (int i = 0; i < tam; i++) if (!PuedeColocar(x + i, y)) return false;
            for (int i = 0; i < tam; i++) celdas[y][x + i].ColocarBarco(idBarco);
        }
        else {
            if (y + tam > TAM) return false;
            for (int i = 0; i < tam; i++) if (!PuedeColocar(x, y + i)) return false;
            for (int i = 0; i < tam; i++) celdas[y + i][x].ColocarBarco(idBarco);
        }
        return true;
    }
    bool Disparar(int x, int y, int& idImpactado, bool& yaDisparado) {
        if (x < 0 || x >= TAM || y < 0 || y >= TAM) return false;
        if (celdas[y][x].FueDisparado()) { yaDisparado = true; return false; }
        celdas[y][x].Disparar();
        if (celdas[y][x].EstaOcupado()) { idImpactado = celdas[y][x].GetBarcoID(); return true; }
        return false;
    }
    void RegistrarDisparo(int x, int y, bool acierto, int idBarco = -1) {
        celdas[y][x].Disparar();
        if (acierto) celdas[y][x].ColocarBarco(idBarco);
    }
    void Mostrar(bool propio) const {
        cout << "   ";
        for (int i = 0; i < TAM; i++) cout << setw(2) << i;
        cout << "\n";
        for (int y = 0; y < TAM; y++) {
            cout << setw(2) << y << " ";
            for (int x = 0; x < TAM; x++) celdas[y][x].Mostrar(propio);
            cout << "\n";
        }
    }
    void MostrarGuia() const {
        cout << "   ";
        for (int i = 0; i < TAM; i++) cout << setw(2) << i;
        cout << "\n";
        for (int y = 0; y < TAM; y++) {
            cout << setw(2) << y << " ";
            for (int x = 0; x < TAM; x++) celdas[y][x].MostrarGuia();
            cout << "\n";
        }
    }
};

// ==================== Clase Jugador ====================
class Jugador {
public:
    string nombre;
    Tablero propio;
    Tablero guia;
    vector<Barco> barcos;

    Jugador(string n = "") : nombre(n) {}

    int LeerNumero(string msg) {
        int num;
        while (true) {
            cout << msg;
            if (cin >> num && num >= 0 && num < Tablero::GetTAM()) return num;
            cout << "⚠️ Número inválido. Intenta de nuevo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    char LeerOrientacion() {
        char orient;
        while (true) {
            cout << "Orientacion (H=Horizontal, V=Vertical): ";
            if (cin >> orient && (orient == 'H' || orient == 'h' || orient == 'V' || orient == 'v')) return orient;
            cout << "⚠️ Entrada inválida. Ingresa H o V.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void ColocarBarcos() {
        cout << "\n>>> Turno de " << nombre << " para colocar sus barcos.\n";
        for (int b = 0; b < 3; b++) {
            cout << "\nBarco #" << (b + 1) << ":\n";
            while (true) {
                int x = LeerNumero(" x: ");
                int y = LeerNumero(" y: ");
                char orient = LeerOrientacion();
                bool horizontal = (orient == 'H' || orient == 'h');
                int idBarco = barcos.size();
                Barco nuevo(idBarco, 4);
                if (propio.ColocarBarco(x, y, horizontal, idBarco)) {
                    barcos.push_back(nuevo);
                    propio.Mostrar(true);
                    break;
                }
                else cout << "⚠️ Posición inválida.\n";
            }
        }
    }

    int BarcosRestantes() const {
        int vivos = 0;
        for (const Barco& b : barcos) if (!b.Hundido()) vivos++;
        return vivos;
    }

    bool Disparar(Jugador& enemigo) {
        while (true) {
            cout << "\nGuía de disparos contra " << enemigo.nombre << ":\n";
            guia.MostrarGuia();
            int x = LeerNumero(" x: ");
            int y = LeerNumero(" y: ");
            int idImpactado = -1; bool yaDisparado = false;
            bool acierto = enemigo.propio.Disparar(x, y, idImpactado, yaDisparado);
            if (yaDisparado) { cout << "⚠️ Ya disparaste ahí.\n"; continue; }
            guia.RegistrarDisparo(x, y, acierto, idImpactado);
            if (acierto) {
                enemigo.barcos[idImpactado].Impactar();
                if (enemigo.barcos[idImpactado].Hundido()) cout << "💥 Barco destruido!\n";
                return true;
            }
            else { cout << "🌊 Agua...\n"; return false; }
        }
    }

    bool HaPerdido() const {
        for (const Barco& b : barcos) if (!b.Hundido()) return false;
        return true;
    }
};

// ==================== MAIN DE PRUEBA ====================
int main() {
    Jugador j1("JugadorB");
    Jugador j2("Enemigo");
    j1.ColocarBarcos();
    j2.ColocarBarcos();
    j1.Disparar(j2);
    return 0;
}




