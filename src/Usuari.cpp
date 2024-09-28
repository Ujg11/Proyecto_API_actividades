#include "../inc/Usuari.hpp"

Usuari::Usuari(string n, string c, string dni, int e, string email)
{
	this->nom = n;
	this->cognoms = c;
	this->dni = dni;
	this->edat = e;
	this->email = email;
}

Usuari::~Usuari()
{
}

void Usuari::setNom(std::string nom)
{
	this->nom = nom;
}

void Usuari::setCognoms(std::string cognoms)
{
	this->cognoms = cognoms;
}

void Usuari::setDni(std::string dni)
{
	this->dni = dni;
}

void Usuari::setEdat(int edat)
{
	this->edat = edat;
}

void Usuari::setEmail(std::string email)
{
	this->email = email;
}

std::string Usuari::getNom() const
{
	return (this->nom);
}

std::string Usuari::getCognoms() const
{
	return (this->cognoms);
}

std::string Usuari::getDni() const
{
	return (this->dni);
}

int Usuari::getEdat() const
{
	return (this->edat);
}

std::string Usuari::getEmail() const
{
	return (this->email);
}
