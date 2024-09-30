#pragma once

# include <string>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <sstream>
#include <fstream>
# include "SqlManager.hpp"
# include "../lib/json.hpp"

using json = nlohmann::json; 

class Server
{
	private:
		int port;
		int servSocket;
		struct sockaddr_in direccio;
		SqlManager &sqlManager;
		
	public:
		Server(SqlManager &sqlManager);
		~Server();
		
		void iniciar();
		void processarSolicitud(int clientSock);
		void sendResponse(std::string &s, int clientSock, int msg);

		//Usuaris
		std::string createUsuari(const std::string &jsonData, int *msg);
		std::string updateUsuari(int id, const std::string &jsonData, int *msg);
		std::string getUsuari(int id, int *msg);
		std::string deleteUsuari(int id, int *msg);

		//Activitats
		std::string createActivitat(const std::string &jsonData, int *msg);
		std::string getActivitat(int id, int *msg);
		std::string deleteActivitat(int id, int *msg);

		//Inscripcions
		std::string apuntarActivitat(int idUsr, int idAct, int *msg);
		std::string desapuntarActivitat(int idUsr, int idAct, int *msg);

		//Importació/Exportació de JSON
		std::string exportarActivitats();
		std::string importarActivitats(const std::string &jsonData, int *msg);
};


