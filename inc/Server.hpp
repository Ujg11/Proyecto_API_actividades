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

        //Usuaris
        std::string createUsuari(const std::string &jsonData);
		std::string updateUsuari(int id, const std::string &jsonData);
		std::string getUsuari(int id);
		std::string deleteUsuari(int id);

		//Activitats
		std::string createActivitat(const std::string &jsonData);
		std::string getActivitat(int id);
		std::string deleteActivitat(int id);

		//Inscripcions
		std::string apuntarActivitat(int idUsr, int idAct);
		std::string desapuntarActivitat(int idUsr, int idAct);

		//Importació/Exportació de JSON
		std::string exportarActivitats();
		std::string importarActivitats(const std::string &jsonData);
};


