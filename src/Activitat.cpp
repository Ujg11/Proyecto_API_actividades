#include "../inc/Activitat.hpp"

Activitat::Activitat(string nom, string desc, int capacitat)
{
	this->nom = nom;
	this->descripcio = desc;
	this->capacitat_maxima = capacitat;
}

Activitat::~Activitat()
{}

void Activitat::setNom(string nom)
{
	this->nom = nom;
}

void Activitat::setDescripcio(string desc)
{
	this->descripcio = desc;
}

void Activitat::setCapacitatMaxima(int cm)
{
	this->capacitat_maxima = cm;
}

string Activitat::getNom() const
{
	return (this->nom);
}

string Activitat::getDescripcio() const
{
	return (this->descripcio);
}

int Activitat::getCapacitatMaxima() const
{
	return (this->capacitat_maxima);
}

