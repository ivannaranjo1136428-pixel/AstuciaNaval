#ifndef A_TABLERO_CASILLA_H
#define A_TABLERO_CASILLA_H

#include <string>
#include <fstream>

class Casilla {
private:
    bool ocupado;
    bool disparado;
    bool hundido;
    int idBarco;

public:
    Casilla();
    void ColocarBarco(int id);
    bool EstaOcupado() const;
    void Disparar();
    bool FueDisparado() const;
    int GetBarcoID() const;
    void SetBarcoID(int id);
    void MarcarHundido();
    bool EstaHundido() const;
    void Mostrar(bool propio) const;
    void MostrarGuia() const;
    std::string ToString(bool propio) const;
    std::string ToStringGuia() const;

    // Guardar / cargar en archivo
    void Serializar(std::ofstream &out) const;
    void Deserializar(std::ifstream &in);
};

#endif
