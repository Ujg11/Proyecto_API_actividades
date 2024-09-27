#include "../inc/Usuari.hpp"

Usuari::Usuari(string n, string c, string dni, int e, string email)
{
	
}

Usuari::~Usuari()
{
}

void Usuari::setNom(std::string nom)
{
	for(size_t i = 0; i < nom.size(); i++)
	{
		if ()
	}
}

void Usuari::setCognoms(std::string cognoms)
{

}

void Usuari::setDni(std::string dni)
{

}

void Usuari::setEdat(int edat)
{

}

void Usuari::setEmail(std::string email)
{

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
