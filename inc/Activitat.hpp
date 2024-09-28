#pragma once

# include <iostream>
# include <string>
# include <cctype>

using namespace std;

class Activitat
{
	private:
		int id;
		string  nom;
		string  descripcio;
		int     capacitat_maxima;

	public:
		Activitat();
		Activitat(string nom, string desc, int capacitat);
		~Activitat();

		void setId(int id);
		void setNom(string nom);
		void setDescripcio(string desc);
		void setCapacitatMaxima(int cm);
		int getId() const;
		string getNom() const;
		string getDescripcio() const;
		int getCapacitatMaxima() const;
};

