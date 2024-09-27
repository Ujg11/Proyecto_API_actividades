#pragma once

# include <iostream>
# include <string>
# include <cctype>

using namespace std;

class Usuari
{
	private:
		std::string	nom;
		std::string	cognoms;
		std::string dni;
		int			edat;
		std::string	email;
		
	public:
		Usuari(string n, string c, string dni, int e, string email);
		~Usuari();

		void setNom(std::string nom);
		void setCognoms(std::string cognoms);
		void setDni(std::string dni);
		void setEdat(int edat);
		void setEmail(std::string email);

		std::string getNom() const;
		std::string getCognoms() const;
		std::string getDni() const;
		int getEdat() const;
		std::string getEmail() const;

};

