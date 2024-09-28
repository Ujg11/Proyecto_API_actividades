#include "../inc/ApiController.hpp"
#include "../inc/Usuari.hpp"
#include "../lib/json.hpp"

using json = nlohmann::json;

ApiController::ApiController(SqlManager &sqlManager): sqlManager(sqlManager)
{
}

ApiController::~ApiController()
{
}

void ApiController::registerRoutes(crow::SimpleApp &app)
{
	CROW_ROUTE(app, "/appActivitats/user").methods("POST"_method)([&](const crow::request &req) {
		return createUsuari(req);
	});
	CROW_ROUTE(app, "/appActivitats/users/<int>").methods("PUT"_method)([&](int id, const crow::request &req) {
		return updateUsuari(id, req);
	});
	CROW_ROUTE(app, "/appActivitats/users/<int>").methods("GET"_method)([&](int id) {
		return getUsuari(id);
	});
	CROW_ROUTE(app, "/appActivitats/users/<int>").methods("DELETE"_method)([&](int id) {
		return deleteUsuari(id);
	});

	CROW_ROUTE(app, "/appActivitats/activitats/exportar").methods("GET"_method)([&]() {
		return exportarActivitats();
	});
	CROW_ROUTE(app, "/appActivitats/importar").methods("POST"_method)([&](const crow::request &req) {
		return importarActivitats(req);
	});
}

crow::response ApiController::createUsuari(const crow::request &req)
{
	json input = json::parse(req.body);
	Usuari usuari;
	usuari.setNom(input["nom"]);

}

crow::response ApiController::updateUsuari(int id, const crow::request &req)
{

}

crow::response ApiController::getUsuari(int id)
{

}

crow::response ApiController::deleteUsuari(int id)
{

}

crow::response createActivitat(const crow::request &req)
{

}

crow::response getActivitat(int id)
{

}

crow::response deleteActivitat(int id)
{

}

crow::response apuntarActivitat(int idUsr, int idAct)
{

}

crow::response desapuntarActivitat(int idUsr, int idAct)
{
	
}


crow::response ApiController::exportarActivitats()
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
	//Convertim a cadena JSON
	std::string jsonString = output.dump();
	//Creem un .json amb les activitats per veure el format
	std::ofstream outFile("activitats.json");
	if (outFile.is_open())
	{
		outFile << jsonString;
		outFile.close();
	}
	return (crow::response{jsonString});
}

crow::response ApiController::importarActivitats(const crow::request &req)
{
	json input;
	try
	{
		input = json::parse(req.body);
	}
	catch(json::parse_error &e)
	{
		return crow::response(400);//Bad Request
	}
	for (const auto &actJson : input)
	{
		Activitat novaAct;
		novaAct.setNom(actJson["nom"]);
		novaAct.setDescripcio(actJson["descripcio"]);
		novaAct.setCapacitatMaxima(actJson["capacitat_maxima"]);
		sqlManager.addActivitat(novaAct);
	}
	return crow::response(201);//Created
}
