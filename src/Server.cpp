#include "../inc/Server.hpp"

//Creem el Socket i fem que estigui escoltant conexions al port especificat
Server::Server(SqlManager &sqlManager): port(8080), sqlManager(sqlManager)
{
	this->servSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (servSocket == 0)
	{
		std::cerr << "Error al crear el socket del servidor." << std::endl;
		exit(EXIT_FAILURE);
	}
	direccio.sin_family = AF_INET;
	direccio.sin_addr.s_addr = INADDR_ANY;
	direccio.sin_port = htons(port);
	if (bind(servSocket, (struct sockaddr*)&direccio, sizeof(direccio)) < 0) {
		std::cerr << "Error en el bind." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(servSocket, 100) < 0) {
		std::cerr << "Error en el listen." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Servidor iniciat i escoltant el port " << port << "..." << std::endl;
}

Server::~Server()
{
	close(servSocket);
}


void Server::iniciar()
{
	int clientSocket;
	struct sockaddr_in direccioClient;
	int longitudCliente = sizeof(direccioClient);

	while (true)
	{
		clientSocket = accept(servSocket, (struct sockaddr*)&direccioClient, (socklen_t*)&longitudCliente);
		if (clientSocket < 0) {
			std::cerr << "Error en el accept." << std::endl;
			continue;
		}
		processarSolicitud(clientSocket);
	}
}

static bool startsWith(const std::string& str, const std::string& prefix)
{
	if (str.length() < prefix.length()) {
		return false;
	}
	return (str.compare(0, prefix.length(), prefix) == 0);
}

/* Ens arriba:
POST /appActivitats/user HTTP/1.1
Host: example.com
Content-Type: application/json
Content-Length: 123
{"nom": "xxx", "cognoms": "askdguhas"}...*/
void Server::processarSolicitud(int clientSock)
{
	char buffer[4096] = {0};
	std::string request = "";
	int bytesRead;

	while ((bytesRead = read(clientSock, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		request += buffer;
		if (request.find("\r\n\r\n") != std::string::npos) {
			break;
		}
	}
	size_t headerEnd = request.find("\r\n\r\n");
	std::string headers = request.substr(0, headerEnd + 4);
	std::string body;
	if (headerEnd + 4 < request.length())
		body = request.substr(headerEnd + 4);

	std::istringstream stream(headers);
	std::string order, path, httpVersion;
	stream >> order >> path >> httpVersion;

	std::string line;
	size_t contentLength = 0;
	while (std::getline(stream, line) && !line.empty())
		if (line.find("Content-Length:") != std::string::npos)
			contentLength = std::stoul(line.substr(line.find(":") + 1));
	if (contentLength > 0)
	{
		size_t totalRead = body.length();
		while (totalRead < contentLength)
		{
			int bytesToRead = contentLength - totalRead;
			char newBuff[bytesToRead + 1];
			int bytesRead = read(clientSock, newBuff, sizeof(newBuff));
			if (bytesRead <= 0)
				break;
			newBuff[bytesRead] = '\0';
			body.append(newBuff, bytesRead);
			totalRead += bytesRead;
		}
	}
	std::string response;
	int msg = 200;
	
	if (order == "POST" && startsWith(path, "/appActivitats/user"))
	{
		json jsonData = json::parse(body);
		response = createUsuari(jsonData.dump(), &msg);
	}
	else if (order == "PUT" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		json jsonData = json::parse(body);
		response = updateUsuari(id, jsonData.dump(), &msg);
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		response = getUsuari(id, &msg);
	}
	else if (order == "DELETE" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		response = deleteUsuari(id, &msg);
	}
	else if (order == "POST" && path == "/appActivitats/activitats")
	{
		json jsonData = json::parse(body);
		response = createActivitat(jsonData.dump(), &msg);
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/activitats/") && path.find("exportar") == std::string::npos)
	{
		int id = std::stoi(path.substr(26));
		response = getActivitat(id, &msg);
	}
	else if (order == "DELETE" && startsWith(path, "/appActivitats/activitats/"))
	{
		int id = std::stoi(path.substr(26));
		response = deleteActivitat(id, &msg);
	}
	else if (order == "POST" && path.find("/appActivitats/activitats/") != std::string::npos && path.find("/apuntar/") != std::string::npos
		&& path.find("/desapuntar/") == std::string::npos)
	{
		size_t startId1 = path.find("/activitats/") + strlen("/activitats/");
		size_t endId1 = path.find("/apuntar/");
		std::string id1Str = path.substr(startId1, endId1 - startId1);
		size_t startId2 = endId1 + strlen("/apuntar/");
		std::string id2Str = path.substr(startId2);
		int idUsr = std::stoi(id1Str);
		int idAct = std::stoi(id2Str);
		response = apuntarActivitat(idUsr, idAct, &msg);
	}
	else if (order == "POST" && path.find("/appActivitats/activitats/") != std::string::npos && path.find("/desapuntar/") != std::string::npos)
	{
		size_t startId1 = path.find("/activitats/") + strlen("/activitats/");
		size_t endId1 = path.find("/desapuntar/");
		std::string id1Str = path.substr(startId1, endId1 - startId1);
		size_t startId2 = endId1 + strlen("/desapuntar/");
		std::string id2Str = path.substr(startId2);
		int idUsr = std::stoi(id1Str);
		int idAct = std::stoi(id2Str);
		response = desapuntarActivitat(idUsr, idAct, &msg);
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/activitats/exportar"))
	{
		response = exportarActivitats();
		msg = 200;
	}
	else if (order == "POST" && startsWith(path, "/appActivitats/activitats/importar"))
	{
		json jsonData = json::parse(body);
		response = importarActivitats(jsonData.dump(), &msg);
	}
	else
		response = "{\"message\": \"Opció no trobada\"}";
	
	sendResponse(response, clientSock, msg);
}

void Server::sendResponse(std::string &s, int clientSock, int cod)
{
	std::string msg;
	switch (cod)
	{
		case 200: msg = "200 OK";
			break;
		case 201: msg = "201 Created";
			break;
		case 202: msg = "202 Accepted";
			break;
		case 204: msg = "204 No content";
			break;
		case 400: msg = "400 Bad Request";
			break;
		case 404: msg = "404 Not Found";
			break;
		case 422: msg = "422 Unprocessable Entity";
			break;
		default:
			break;
	}
	std::string response = "HTTP/1.1 " + msg + "\r\n"
						   "Content-Type: application/json\r\n"
						   "Content-Length: " + std::to_string(s.size()) + "\r\n"
						   "\r\n" + s + "\r\n\r\n";
	write(clientSock, response.c_str(), response.length());
	close(clientSock);
}

std::string Server::createUsuari(const std::string &jsonData, int *msg)
{
	try
	{
		json input = json::parse(jsonData);
		Usuari usuari;
		if (input.contains("nom") && input.contains("cognoms") && input.contains("dni") && input.contains("edat") && input.contains("email"))
		{
			usuari.setNom(input["nom"]);
			usuari.setCognoms(input["cognoms"]);
			usuari.setDni(input["dni"]);
			usuari.setEdat(input["edat"]);
			usuari.setEmail(input["email"]);
			if (sqlManager.addUsuari(usuari))
			{
				*msg = 200;
				return ("{\"message\": \"Usuari creat\"}");
			}
			else
			{
				*msg = 400;
				return ("{\"message\": \"Error en crear l'usuari\"}");
			}
		}
		else
		{
			*msg = 422;
			return ("{\"message\": \"Error, falten camps en l'usuario\"}");
		}
			
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::updateUsuari(int id, const std::string &jsonData, int *msg)
{
	try
	{
		json input = json::parse(jsonData);
		Usuari usuari;
		if (input.contains("nom") && input.contains("cognoms") && input.contains("dni") && input.contains("edat") && input.contains("email"))
		{
			usuari.setNom(input["nom"]);
			usuari.setCognoms(input["cognoms"]);
			usuari.setDni(input["dni"]);
			usuari.setEdat(input["edat"]);
			usuari.setEmail(input["email"]);
			if (sqlManager.updateUsuari(id, usuari))
			{
				*msg = 200;
				return ("{\"message\": \"Usuari actualitzat\"}");
			}
			else
			{
				*msg = 404;
				return ("{\"message\": \"Error, usuari no trobat\"}");
			}
		}
		else
		{
			*msg = 422;
			return ("{\"message\": \"Error, falten camps en l'usuario\"}");
		}
			
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::getUsuari(int id, int *msg)
{
	try
	{
		Usuari u = sqlManager.getUsuari(id);
		if (u.getId() == -1)
		{
			*msg = 404;
			return ("{\"message\": \"Usuari no trobat\"}");
		}
		json output;
		output["nom"] = u.getNom();
		output["cognoms"] = u.getCognoms();
		output["dni"] = u.getDni();
		output["edat"] = u.getEdat();
		output["email"] = u.getEmail();
		return output.dump();
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::deleteUsuari(int id, int *msg)
{
	try
	{
		if (sqlManager.removeUsuari(id))
		{
			*msg = 200;
			return ("{\"message\": \"Usuari eliminat\"}");
		}
		else
		{
			*msg = 404;
			return ("{\"message\": \"Usuari no trobat\"}");
		}
			
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::createActivitat(const std::string &jsonData, int *msg)
{
	try
	{
		json input = json::parse(jsonData);
		Activitat novaAct;
		if (input.contains("nom") && input.contains("descripcio") && input.contains("capacitat_maxima"))
		{
			novaAct.setNom(input["nom"]);
			novaAct.setDescripcio(input["descripcio"]);
			novaAct.setCapacitatMaxima(input["capacitat_maxima"]);
			if (sqlManager.addActivitat(novaAct))
			{
				*msg = 201;
				return ("{\"message\": \"Activitat creada\"}");
			}
			else
			{
				*msg = 422;
				return ("{\"message\": \"Error al crear l'activitat\"}");
			}
		}
		else
		{
			*msg = 422;
			return ("{\"message\": \"Error, falten camps en l'activitat\"}");
		}
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::getActivitat(int id, int *msg)
{
	try
	{
		Activitat act = sqlManager.getActivitat(id);
		if (act.getId() == -1)
			return ("{\"message\": \"Error, activitat no trobada\"}");
		json output;
		output["nom"] = act.getNom();
		output["descripcio"] = act.getDescripcio();
		output["capacitat_maxima"] = act.getCapacitatMaxima();
		*msg = 200;
		return output.dump();
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::deleteActivitat(int id, int *msg)
{
	try
	{
		if (sqlManager.removeActivitat(id))
		{
			*msg = 200;
			return ("{\"message\": \"Activitat eliminada correctament\"}");
		}
		else
		{
			*msg = 404;
			return ("{\"message\": \"Error, activitat no trobada\"}");
		}
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::apuntarActivitat(int idUsr, int idAct, int *msg)
{
	try
	{
		if (sqlManager.apuntarseActivitat(idUsr, idAct))
		{
			*msg = 200;
			return ("{\"message\": \"Usuari apuntat a l'activitat\"}");
		}
		else
		{
			*msg = 400;
			return ("{\"message\": \"Error al apuntar a l'usuari\"}");
		}
			
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::desapuntarActivitat(int idUsr, int idAct, int *msg)
{
	try
	{
		if (sqlManager.desapuntarseActivitat(idUsr, idAct))
		{
			*msg = 200;
			return ("{\"message\": \"Usuari desapuntat a l'activitat\"}");
		}
		else
		{
			*msg = 400;
			return ("{\"message\": \"Error al desapuntar a l'usuari\"}");
		}
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::exportarActivitats()
{
	std::vector<Activitat> activitats = sqlManager.getAllActivitats();
	json output = json::array();

	for (const Activitat& activitat : activitats)
	{
		json actJson;
		actJson["nom"] = activitat.getNom();
		actJson["descripcio"] = activitat.getDescripcio();
		actJson["capacitat_maxima"] = activitat.getCapacitatMaxima();
		output.push_back(actJson);
	}
	std::string jsonString = output.dump();
	/*std::ofstream outFile("activitats.json");
	if (outFile.is_open())
	{
		outFile << jsonString;
		outFile.close();
	}*/
	return (jsonString);
}

std::string Server::importarActivitats(const std::string &jsonData, int *msg)
{
	json input;
	try
	{
		input = json::parse(jsonData);
		for (const auto &actJson : input)
		{
			Activitat novaAct;
			if (actJson.contains("nom") && actJson.contains("descripcio") && actJson.contains("capacitat_maxima"))
			{
				novaAct.setNom(actJson["nom"]);
				novaAct.setDescripcio(actJson["descripcio"]);
				novaAct.setCapacitatMaxima(actJson["capacitat_maxima"]);
				sqlManager.addActivitat(novaAct);
			}
			else
			{
				*msg = 422;
				return ("{\"message\": \"Error en l'import, falten camps en alguna activitat\"}");
			}
				
		}
		*msg = 200;
		return ("{\"message\": \"L'import ha estat unèxit\"}");
	}
	catch(json::parse_error &e)
	{
		*msg = 400;
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}
