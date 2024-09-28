#pragma once

# include <mysql_driver.h>
# include <mysql_connection.h>
# include <string>
# include <vector>
# include <memory>
# include <cppconn/prepared_statement.h>
# include <cppconn/resultset.h>
# include <cppconn/exception.h>
# include "Usuari.hpp"
# include "Activitat.hpp"

class SqlManager
{
	private:
		sql::mysql::MySQL_Driver			*driver;
		std::shared_ptr<sql::Connection>	connection;
		string sqlUser;
		string sqlPassword;
		string sqlDataBase;

	public:
		SqlManager();
		SqlManager(string sqlUser, string sqlPassword, string sqlDataBase);
		~SqlManager();

		//Usuaris
		bool addUsuari(Usuari &u);
		bool removeUsuari(int id);
		Usuari getUsuari(int id);
		bool updateUsuari(int id, const Usuari &u);
	
		//Activitats
		bool addActivitat(Activitat &a);
		bool removeActivitat(int id);
		Activitat getActivitat(int id);
		bool updateActivitat(int id, const Activitat &a);
		std::vector<Activitat> getAllActivitats();

		//Inscripcions
		bool apuntarseActivitat(int usuariID, int activitatId);
		bool desapuntarseActivitat(int usuariID, int activitatId);
};

