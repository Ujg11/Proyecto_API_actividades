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
	
	if (order == "POST" && startsWith(path, "/appActivitats/user"))
	{
		json jsonData = json::parse(body);
		response = createUsuari(jsonData.dump());
	}
	else if (order == "PUT" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		json jsonData = json::parse(body);
		response = updateUsuari(id, jsonData.dump());
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		response = getUsuari(id);
	}
	else if (order == "DELETE" && startsWith(path, "/appActivitats/users/"))
	{
		int id = std::stoi(path.substr(21));
		response = deleteUsuari(id);
	}
	else if (order == "POST" && path == "/appActivitats/activitats")
	{
		json jsonData = json::parse(body);
		response = createActivitat(jsonData.dump());
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/activitats/") && path.find("exportar") == std::string::npos)
	{
		int id = std::stoi(path.substr(26));
		response = getActivitat(id);
	}
	else if (order == "DELETE" && startsWith(path, "/appActivitats/activitats/"))
	{
		int id = std::stoi(path.substr(26));
		response = deleteActivitat(id);
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
		response = apuntarActivitat(idUsr, idAct);
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
		response = desapuntarActivitat(idUsr, idAct);
	}
	else if (order == "GET" && startsWith(path, "/appActivitats/activitats/exportar"))
	{
		response = exportarActivitats();
	}
	else if (order == "POST" && startsWith(path, "/appActivitats/activitats/importar"))
	{
		json jsonData = json::parse(body);
		response = importarActivitats(jsonData.dump());
	}
	else
		response = "{\"message\": \"Opció no trobada\"}";
	sendResponse(response, clientSock);
}

void Server::sendResponse(std::string &s, int clientSock)
{
	std::string response = "HTTP/1.1 200 OK\r\n"
						   "Content-Type: application/json\r\n"
						   "Content-Length: " + std::to_string(s.size()) + "\r\n"
						   "\r\n" + s + "\n";
	write(clientSock, response.c_str(), response.length());
	close(clientSock);
}

std::string Server::createUsuari(const std::string &jsonData)
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
				return ("{\"message\": \"Usuari creat\"}");
			else
				return ("{\"message\": \"Error en crear l'usuari\"}");
		}
		else
			return ("{\"message\": \"Error, falten camps en l'usuario\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::updateUsuari(int id, const std::string &jsonData)
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
				return ("{\"message\": \"Usuari actualitzat\"}");
			else
				return ("{\"message\": \"Error, usuari no trobat\"}");
		}
		else
			return ("{\"message\": \"Error, falten camps en l'usuario\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::getUsuari(int id)
{
	try
	{
		Usuari u = sqlManager.getUsuari(id);
		if (u.getId() == -1)
			return ("{\"message\": \"Usuari no trobat\"}");
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
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::deleteUsuari(int id)
{
	try
	{
		if (sqlManager.removeUsuari(id))
			return ("{\"message\": \"Usuari eliminat\"}");
		else
			return ("{\"message\": \"Usuari no trobat\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::createActivitat(const std::string &jsonData)
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
				return ("{\"message\": \"Activitat creada\"}");
			else
				return ("{\"message\": \"Error al crear l'activitat\"}");
		}
		else
			return ("{\"message\": \"Error, falten camps en l'activitat\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::getActivitat(int id)
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
		return output.dump();
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::deleteActivitat(int id)
{
	try
	{
		if (sqlManager.removeActivitat(id)) {
			return ("{\"message\": \"Activitat eliminada correctament\"}");
		} else {
			return ("{\"message\": \"Error, activitat no trobada\"}");
		}
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::apuntarActivitat(int idUsr, int idAct)
{
	try
	{
		if (sqlManager.apuntarseActivitat(idUsr, idAct))
			return ("{\"message\": \"Usuari apuntat a l'activitat\"}");
		else
			return ("{\"message\": \"Error al apuntar a l'usuari\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}

std::string Server::desapuntarActivitat(int idUsr, int idAct)
{
	try
	{
		if (sqlManager.desapuntarseActivitat(idUsr, idAct))
			return ("{\"message\": \"Usuari desapuntat a l'activitat\"}");
		else
			return ("{\"message\": \"Error al desapuntar a l'usuari\"}");
	}
	catch(json::parse_error &e)
	{
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

std::string Server::importarActivitats(const std::string &jsonData)
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
				return ("{\"message\": \"Error en l'import, falten camps en alguna activitat\"}");
		}
		return ("{\"message\": \"L'import ha estat unèxit\"}");
	}
	catch(json::parse_error &e)
	{
		return ("{\"message\": \"Error al parsejar el JSON\"}");
	}
}
