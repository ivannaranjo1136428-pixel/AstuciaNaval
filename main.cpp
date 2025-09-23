#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;

// ==================== Clase Casilla ====================
class Casilla {
    bool ocupado;
    bool disparado;
    bool hundido;
    int idBarco;
public:
    Casilla() : ocupado(false), disparado(false), hundido(false), idBarco(-1) {}

    void ColocarBarco(int id) { ocupado = true; idBarco = id; }
    bool EstaOcupado() const { return ocupado; }

    void Disparar() { disparado = true; }
    bool FueDisparado() const { return disparado; }

    int GetBarcoID() const { return idBarco; }
    void SetBarcoID(int id) { idBarco = id; ocupado = (id != -1); }

    void MarcarHundido() { hundido = true; }
    bool EstaHundido() const { return hundido; }

    // Mostrar en consola (usa secuencias ANSI)
    void Mostrar(bool propio) const {
        cout << ToString(propio);
    }

    void MostrarGuia() const {
        cout << ToStringGuia();
    }

    // Devuelve la representacion en string (con coloration ANSI si aplica)
    string ToString(bool propio) const {
        if (hundido) {
            return string("\033[45m  \033[0m");
        } else if (disparado) {
            if (ocupado) return string("\033[41m  \033[0m");
            else return string("\033[44m  \033[0m");
        } else {
            if (propio && ocupado) return string("\033[42m  \033[0m");
            else return string("\033[47m  \033[0m");
        }
    }

    // Guia: si no fue disparado, se muestra como no disparado (gris)
    string ToStringGuia() const {
        if (hundido) return string("\033[45m  \033[0m");
        else if (disparado) {
            if (ocupado) return string("\033[41m  \033[0m");
            else return string("\033[44m  \033[0m");
        } else {
            return string("\033[47m  \033[0m");
        }
    }

    // Serializacion binaria
    void Serializar(ofstream &out) const {
        out.write((char*)&ocupado, sizeof(ocupado));
        out.write((char*)&disparado, sizeof(disparado));
        out.write((char*)&hundido, sizeof(hundido));
        out.write((char*)&idBarco, sizeof(idBarco));
    }

    void Deserializar(ifstream &in) {
        in.read((char*)&ocupado, sizeof(ocupado));
        in.read((char*)&disparado, sizeof(disparado));
        in.read((char*)&hundido, sizeof(hundido));
        in.read((char*)&idBarco, sizeof(idBarco));
    }
};

// ==================== Clase Tablero ====================
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
            for (int i = 0; i < tam; ++i)
                if (!PuedeColocar(x + i, y)) return false;
            for (int i = 0; i < tam; ++i) celdas[y][x + i].ColocarBarco(idBarco);
        } else {
            if (y + tam > TAM) return false;
            for (int i = 0; i < tam; ++i)
                if (!PuedeColocar(x, y + i)) return false;
            for (int i = 0; i < tam; ++i) celdas[y + i][x].ColocarBarco(idBarco);
        }
        return true;
    }

    bool Disparar(int x, int y, int &idImpactado, bool &yaDisparado) {
        if (x < 0 || x >= TAM || y < 0 || y >= TAM) return false;
        if (celdas[y][x].FueDisparado()) {
            yaDisparado = true;
            return false;
        }
        celdas[y][x].Disparar();
        if (celdas[y][x].EstaOcupado()) {
            idImpactado = celdas[y][x].GetBarcoID();
            return true;
        }
        return false;
    }

    void MarcarBarcoHundido(int idBarco) {
        for (int y = 0; y < TAM; ++y)
            for (int x = 0; x < TAM; ++x)
                if (celdas[y][x].GetBarcoID() == idBarco)
                    celdas[y][x].MarcarHundido();
    }

    void SetBarcoID(int x, int y, int id) { celdas[y][x].SetBarcoID(id); }

    // Impresion directa a consola
    void Mostrar(bool propio) const {
        cout << ToString(propio);
    }

    void MostrarGuia() const {
        cout << ToStringGuia();
    }

    // Convierte el tablero a string (para historial)
    string ToString(bool propio) const {
        ostringstream ss;
        ss << "    ";
        for (int i = 0; i < TAM; ++i) ss << setw(2) << i << " ";
        ss << "\n   +" << string(TAM * 3, '-') << "+\n";
        for (int y = 0; y < TAM; ++y) {
            ss << setw(2) << y << " |";
            for (int x = 0; x < TAM; ++x) {
                ss << celdas[y][x].ToString(propio) << "|";
            }
            ss << "\n   +" << string(TAM * 3, '-') << "+\n";
        }
        return ss.str();
    }

    string ToStringGuia() const {
        ostringstream ss;
        ss << "    ";
        for (int i = 0; i < TAM; ++i) ss << setw(2) << i << " ";
        ss << "\n   +" << string(TAM * 3, '-') << "+\n";
        for (int y = 0; y < TAM; ++y) {
            ss << setw(2) << y << " |";
            for (int x = 0; x < TAM; ++x) {
                ss << celdas[y][x].ToStringGuia() << "|";
            }
            ss << "\n   +" << string(TAM * 3, '-') << "+\n";
        }
        return ss.str();
    }

    void Serializar(ofstream &out) const {
        for (int y = 0; y < TAM; ++y)
            for (int x = 0; x < TAM; ++x)
                celdas[y][x].Serializar(out);
    }

    void Deserializar(ifstream &in) {
        for (int y = 0; y < TAM; ++y)
            for (int x = 0; x < TAM; ++x)
                celdas[y][x].Deserializar(in);
    }
};

// ==================== Clase Barco ====================
class Barco {
public:
    int id;
    int tam;
    int impactos;
    Barco(int i = 0, int t = 4) : id(i), tam(t), impactos(0) {}
    void Impactar() { impactos++; }
    bool Hundido() const { return impactos >= tam; }

    void Serializar(ofstream &out) const {
        out.write((char*)&id, sizeof(id));
        out.write((char*)&tam, sizeof(tam));
        out.write((char*)&impactos, sizeof(impactos));
    }
    void Deserializar(ifstream &in) {
        in.read((char*)&id, sizeof(id));
        in.read((char*)&tam, sizeof(tam));
        in.read((char*)&impactos, sizeof(impactos));
    }
};

// Forward decl Partida (se define despues)
class Partida;

// ==================== Clase Jugador ====================
class Jugador {
public:
    string nombre;
    Tablero propio;
    Tablero oponente;
    vector<Barco> barcos;

    Jugador(string n = "") : nombre(n) {}

    // Lectura segura de coordenadas
    int LeerCoordenada(const string &msg) {
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
                cout << "ERROR - Valor fuera de rango. Ingrese un numero entre 0 y 19.\n";
                continue;
            }
            // limpiar resto de linea
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
    }

    void ColocarBarcos() {
        cout << "\n>>> Turno de " << nombre << " para colocar sus barcos.\n";
        for (int b = 0; b < 3; ++b) {
            cout << "\nBarco #" << (b + 1) << " de " << nombre << ":\n";
            while (true) {
                int x = LeerCoordenada("  x (0-19): ");
                int y = LeerCoordenada("  y (0-19): ");
                char orient;
                do {
                    cout << "Orientacion (H=Horizontal, V=Vertical): ";
                    cin >> orient;
                    if (!(orient == 'H' || orient == 'h' || orient == 'V' || orient == 'v')) {
                        cout << "ERROR - Opcion no valida. Solo se acepta H o V.\n";
                    }
                } while (!(orient == 'H' || orient == 'h' || orient == 'V' || orient == 'v'));

                // limpiar resto de linea para la siguiente lectura segura
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                bool horizontal = (orient == 'H' || orient == 'h');
                int idBarco = static_cast<int>(barcos.size());
                Barco nuevo(idBarco, 4);

                if (propio.ColocarBarco(x, y, horizontal, idBarco)) {
                    cout << "OK - Barco colocado con exito.\n";
                    barcos.push_back(nuevo);
                    propio.Mostrar(true);
                    break;
                } else {
                    cout << "ERROR - Posicion no valida. Elija otra.\n";
                }
            }
        }

        cout << "\n--- Fin del turno de " << nombre << " ---\n";
        cout << "Pase el control al siguiente jugador y presione ENTER...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    int BarcosRestantes() const {
        int vivos = 0;
        for (const Barco &b : barcos)
            if (!b.Hundido()) ++vivos;
        return vivos;
    }

    // Disparar: retorna true si acerto
    bool Disparar(Jugador &enemigo, vector<string> &historial) {
        while (true) {
            cout << "\nGuia de disparos contra " << enemigo.nombre << ":\n";
            cout << oponente.ToStringGuia();

            int x = LeerCoordenada(" x (0-19): ");
            int y = LeerCoordenada(" y (0-19): ");

            int idImpactado = -1;
            bool yaDisparado = false;

            bool acierto = enemigo.propio.Disparar(x, y, idImpactado, yaDisparado);

            if (yaDisparado) {
                cout << "ERROR - Ya disparaste en (" << x << "," << y << "). Intente otra posicion.\n";
                continue; // pedir otras coordenadas
            }

            // marcar disparo en el tablero de referencia del jugador
            int dummyId = -1; bool dummyBool = false;
            oponente.Disparar(x, y, dummyId, dummyBool);

            if (acierto) {
                oponente.SetBarcoID(x, y, idImpactado);
                cout << "IMPACTO - Disparo acertado!\n";
                enemigo.barcos[idImpactado].Impactar();
                if (enemigo.barcos[idImpactado].Hundido()) {
                    cout << "HUNDIDO - Barco de " << enemigo.nombre << " destruido!\n";
                    enemigo.propio.MarcarBarcoHundido(idImpactado);
                    oponente.MarcarBarcoHundido(idImpactado);
                }
            } else {
                cout << "AGUA - No acertaste...\n";
            }

            cout << "> A " << enemigo.nombre << " le quedan "
                 << enemigo.BarcosRestantes() << " barcos por destruir.\n";

            // Guardar solo la guía de disparos en historial
            ostringstream ss;
            ss << "Disparo de " << nombre << " a (" << x << "," << y << ")\n\n";
            ss << "Guia de disparos de " << nombre << ":\n" << oponente.ToStringGuia() << "\n";
            historial.push_back(ss.str());

            return acierto;
        }
    }

    bool HaPerdido() const {
        for (const Barco &b : barcos)
            if (!b.Hundido()) return false;
        return true;
    }

    // Serializar jugador completo (nombre + tableros + barcos)
    void Serializar(ofstream &out) const {
        size_t len = nombre.size();
        out.write((char*)&len, sizeof(len));
        out.write(nombre.c_str(), len);
        propio.Serializar(out);
        oponente.Serializar(out);
        size_t n = barcos.size();
        out.write((char*)&n, sizeof(n));
        for (const Barco &b : barcos) b.Serializar(out);
    }

    void Deserializar(ifstream &in) {
        size_t len;
        in.read((char*)&len, sizeof(len));
        nombre.resize(len);
        in.read(&nombre[0], len);
        propio.Deserializar(in);
        oponente.Deserializar(in);
        size_t n;
        in.read((char*)&n, sizeof(n));
        barcos.resize(n);
        for (size_t i = 0; i < n; ++i) barcos[i].Deserializar(in);
    }
};

// ==================== Clase Partida ====================
class Partida {
public:
    static void Guardar(const Jugador &j1, const Jugador &j2, const string &turno, const vector<string> &historial) {
        ofstream out("partida.dat", ios::binary);
        if (!out) {
            cerr << "ERROR - No se pudo abrir archivo para guardar.\n";
            return;
        }
        j1.Serializar(out);
        j2.Serializar(out);
        size_t len = turno.size();
        out.write((char*)&len, sizeof(len));
        out.write(turno.c_str(), len);
        size_t h = historial.size();
        out.write((char*)&h, sizeof(h));
        for (const string &s : historial) {
            size_t l = s.size();
            out.write((char*)&l, sizeof(l));
            out.write(s.c_str(), l);
        }
    }

    static bool Cargar(Jugador &j1, Jugador &j2, string &turno, vector<string> &historial) {
        ifstream in("partida.dat", ios::binary);
        if (!in) return false;
        j1.Deserializar(in);
        j2.Deserializar(in);
        size_t len;
        in.read((char*)&len, sizeof(len));
        turno.resize(len);
        in.read(&turno[0], len);
        size_t h;
        in.read((char*)&h, sizeof(h));
        historial.resize(h);
        for (size_t i = 0; i < h; ++i) {
            size_t l;
            in.read((char*)&l, sizeof(l));
            historial[i].resize(l);
            in.read(&historial[i][0], l);
        }
        return true;
    }
};

// ==================== Clase Juego ====================
class Juego {
public:
    Jugador j1, j2;
    string turno;
    vector<string> historial;

    Juego() {}
    Juego(const string &n1, const string &n2) : j1(n1), j2(n2), turno(n1) {}
    Juego(const Jugador &p1, const Jugador &p2, const string &turn, const vector<string> &hist)
        : j1(p1), j2(p2), turno(turn), historial(hist) {}

    void Iniciar() {
        cout << "=== ASTUCIA NAVAL ===\n";

        // Si los jugadores no tienen barcos colocados (barcos vacio), pedir colocacion.
        if (j1.barcos.empty()) j1.ColocarBarcos();
        if (j2.barcos.empty()) j2.ColocarBarcos();

        Jugador *turnoPtr = (turno == j1.nombre) ? &j1 : &j2;
        Jugador *enemigoPtr = (turnoPtr == &j1) ? &j2 : &j1;

        while (true) {
            cout << "\nTurno de " << turnoPtr->nombre << "\n";

            bool acierto;
            do {
                acierto = turnoPtr->Disparar(*enemigoPtr, historial);

                // Guardar estado despues de cada disparo
                Partida::Guardar(j1, j2, turnoPtr->nombre, historial);

                if (enemigoPtr->HaPerdido()) {
                    cout << "\nTABLERO FINAL - Estado del tablero de " << enemigoPtr->nombre << ":\n";
                    enemigoPtr->propio.Mostrar(true);

                    cout << "\nTABLERO FINAL - Estado del tablero de " << turnoPtr->nombre << ":\n";
                    turnoPtr->propio.Mostrar(true);

                    cout << "\nVICTORIA - " << turnoPtr->nombre << " ha ganado la partida!\n";

                    cout << "\nPresione ENTER para salir...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();

                    // borrar archivo de guardado si lo desea (opcional)
                    // remove("partida.dat");

                    return;
                }

                if (acierto) {
                    cout << "REPITE - " << turnoPtr->nombre << " acerto y repite disparo.\n";
                }

            } while (acierto);

            // Cambiar turno
            if (turnoPtr == &j1) {
                turnoPtr = &j2;
                enemigoPtr = &j1;
            } else {
                turnoPtr = &j1;
                enemigoPtr = &j2;
            }
            turno = turnoPtr->nombre;
        }
    }

    static void MostrarHistorial(const vector<string> &h) {
        cout << "\n=== HISTORIAL DE PARTIDA ===\n";
        for (size_t i = 0; i < h.size(); ++i) {
            cout << "---- Jugada " << (i + 1) << " ----\n";
            cout << h[i] << "\n";
        }
    }
};

// ==================== MAIN ====================
int main() {
    // Verificar si existe archivo de partida guardada
    ifstream test("partida.dat", ios::binary);
    if (test) {
        char op;
        cout << "Se encontro una partida guardada. Desea cargarla? (S/N): ";
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (op == 'S' || op == 's') {
            Jugador j1, j2;
            string turno;
            vector<string> historial;
            bool cargada = Partida::Cargar(j1, j2, turno, historial);
            if (!cargada) {
                cout << "ERROR - No se pudo cargar la partida.\n";
            } else {
                                cout << "Partida cargada con exito.\n";

                char h;
                cout << "Desea ver el historial de jugadas anteriores? (S/N): ";
                cin >> h;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (h == 'S' || h == 's') {
                    Juego::MostrarHistorial(historial);
                }

                char r;
                cout << "Desea reanudar la partida cargada? (S/N): ";
                cin >> r;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (r == 'S' || r == 's') {
                    Juego juegoCargado(j1, j2, turno, historial);
                    juegoCargado.Iniciar();
                    return 0;
                } else {
                    cout << "La partida ha sido mostrada. Saliendo.\n";
                    return 0;
                }
            }
        }
    }

    // Si no cargo, iniciar nueva partida
    string n1, n2;
    cout << "Nombre del Jugador 1: ";
    getline(cin, n1);
    if (n1.empty()) { // si el usuario presiono enter antes
        cout << "Nombre del Jugador 1 (no vacio): ";
        getline(cin, n1);
    }
    cout << "Nombre del Jugador 2: ";
    getline(cin, n2);
    if (n2.empty()) {
        cout << "Nombre del Jugador 2 (no vacio): ";
        getline(cin, n2);
    }

    Juego juego(n1, n2);
    juego.Iniciar();

    return 0;
}
