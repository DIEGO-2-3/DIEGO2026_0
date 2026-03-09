#ifndef USUARIOS_H
#define USUARIOS_H

#include "Sistema.h"
#include <memory>

class Usuario {
protected:
    string username;
    string password;
    Rol rol;

public:
    Usuario(string u, string p, Rol r) : username(u), password(p), rol(r) {}
    virtual ~Usuario() = default;

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    Rol getRol() const { return rol; }

    virtual void mostrarMenu(SistemaAlmacen& sistema) = 0; 
};

class Administrador : public Usuario {
public:
    Administrador(string u, string p);
    void mostrarMenu(SistemaAlmacen& sistema) override;
};

class Empleado : public Usuario {
public:
    Empleado(string u, string p);
    void mostrarMenu(SistemaAlmacen& sistema) override;
};

class GestorSesion {
private:
    vector<unique_ptr<Usuario>> usuarios;
    void cargarUsuarios();
    void guardarUsuariosDefault();

public:
    GestorSesion();
    Usuario* login();
};

#endif