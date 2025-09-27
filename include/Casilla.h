#ifndef CASILLA_H
#define CASILLA_H

#include <fstream>
#include <string>

class Casilla {
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

    void Serializar(std::ofstream &out) const;
    void Deserializar(std::ifstream &in);
};

#endif // CASILLA_H
