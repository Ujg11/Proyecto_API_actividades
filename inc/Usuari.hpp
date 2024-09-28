#pragma once

# include <iostream>
# include <string>
# include <cctype>

using namespace std;

class Usuari
{
	private:
		int		id;
		string	nom;
		string	cognoms;
		string	dni;
		int		edat;
		string	email;
		
	public:
		Usuari(string n, string c, string dni, int e, string email);
		Usuari();
		~Usuari();

		void setId(int id);
		void setNom(std::string nom);
		void setCognoms(std::string cognoms);
		void setDni(std::string dni);
		void setEdat(int edat);
		void setEmail(std::string email);

		int getId() const;
		std::string getNom() const;
		std::string getCognoms() const;
		std::string getDni() const;
		int getEdat() const;
		std::string getEmail() const;

};

