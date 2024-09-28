#pragma once

# include <iostream>
# include <string>
# include <cctype>

using namespace std;

class Activitat
{
	private:
		string  nom;
		string  descripcio;
		int     capacitat_maxima;

	public:
		Activitat(string nom, string desc, int capacitat);
		~Activitat();

		void setNom(string nom);
		void setDescripcio(string desc);
		void setCapacitatMaxima(int cm);
		string getNom() const;
		string getDescripcio() const;
		int getCapacitatMaxima() const;
};

