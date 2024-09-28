#pragma once

# include "../lib/crow_all.h"
# include "SqlManager.hpp"
# include "Usuari.hpp"
# include "Activitat.hpp"

class ApiController
{
	private:
		SqlManager &sqlManager;

	public:
		ApiController(SqlManager &sqlManager);
		~ApiController();

		void registerRoutes(crow::SimpleApp &app);

		//Usuaris
		crow::response createUsuari(const crow::request &req);
		crow::response updateUsuari(int id, const crow::request &req);
		crow::response getUsuari(int id);
		crow::response deleteUsuari(int id);

		//Activitats
		crow::response createActivitat(const crow::request &req);
		crow::response getActivitat(int id);
		crow::response deleteActivitat(int id);

		//Inscripcions
		crow::response apuntarActivitat(int idUsr, int idAct);
		crow::response desapuntarActivitat(int idUsr, int idAct);

		//Importació/Exportació de JSON
		crow::response exportarActivitats();
		crow::response importarActivitats(const crow::request &req);
};

