#include "../inc/SqlManager.hpp"

SqlManager::SqlManager()
{
	this->driver = sql::mysql::get_mysql_driver_instance();
	this->connection.reset(driver->connect("tcp://127.0.0.1:3306", "ojimenez", "xxxx"));
	this->connection->setSchema("API_Activitats_db");
}

SqlManager::SqlManager(string sqlUser, string sqlPassword, string sqlDataBase)
{
	this->driver = sql::mysql::get_mysql_driver_instance();
	this->connection.reset(driver->connect("tcp://127.0.0.1:3306", sqlUser, sqlPassword));
	this->connection->setSchema(sqlDataBase);
}

SqlManager::~SqlManager()
{
	connection->close();
}

bool SqlManager::addUsuari(Usuari &u)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"INSERT INTO users  (nom, cognoms, dni, edat, email) VALUES (?, ?, ?, ?, ?)"
		));

		statement->setString(1, u.getNom());
		statement->setString(2, u.getCognoms());
		statement->setString(3, u.getDni());
		statement->setInt(4, u.getEdat());
		statement->setString(5, u.getEmail());

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
		{
			std::unique_ptr<sql::PreparedStatement> statementId(connection->prepareStatement("SELECT LAST_INSERT_ID()"));
			std::unique_ptr<sql::ResultSet> result(statementId->executeQuery());

			if (result->next())
			{
				int id = result->getInt(1);
				std::cout << "Usuari afegit amb ID: " << id << std::endl;
				u.setId(id);
			}
		}
		else
		{
			std::cerr << "L'usuari no s'ha pogut afegir" << std::endl;
			return (false);
		}
			
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool SqlManager::removeUsuari(int id)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"DELETE FROM users WHERE id = ?"
		));

		statement->setInt(1, id);
		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "Usuari amb ID: " << id << " eliminat de la base de dades" << std::endl;
		else
		{
			std::cerr << "No s'ha trobat cap usuari amb Id: " << id << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

Usuari SqlManager::getUsuari(int id)
{
	Usuari user;
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"SELECT nom, cognoms, dni, edat, email FROM users WHERE id = ?"
		));

		statement->setInt(1, id);
		std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
		if (result->next())
		{
			user.setId(id);
			user.setNom(result->getString("nom"));
			user.setCognoms(result->getString("cognoms"));
			user.setDni(result->getString("dni"));
			user.setEdat(result->getInt("edat"));
			user.setEmail(result->getString("email"));
		}
		else
		{
			std::cerr << "No s'ha trobat cap usuari amb ID: " << id << std::endl;
			user.setId(-1);
		}
			
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
	}
	return (user);
}

bool SqlManager::updateUsuari(int id, const Usuari &u)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"UPDATE users SET nom = ?, cognoms = ?, dni = ?, edat = ?, email = ? WHERE id = ?"
		));

		statement->setString(1, u.getNom());
		statement->setString(2, u.getCognoms());
		statement->setString(3, u.getDni());
		statement->setInt(4, u.getEdat());
		statement->setString(5, u.getEmail());
		statement->setInt(6, id);

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "Usuari amb Id: " << id << " actualitzat" << std::endl;
		else
		{
			std::cerr << "Usuari amb Id: " << id << " no trobat" << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool SqlManager::addActivitat(Activitat &a)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"INSERT INTO activitats (nom, descripcio, capacitat_maxima) VALUES (?, ?, ?)"
		));

		statement->setString(1, a.getNom());
		statement->setString(2, a.getDescripcio());
		statement->setInt(3, a.getCapacitatMaxima());

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
		{
			std::unique_ptr<sql::PreparedStatement> statementId(connection->prepareStatement("SELECT LAST_INSERT_ID()"));
			std::unique_ptr<sql::ResultSet> result(statementId->executeQuery());

			if (result->next())
			{
				int id = result->getInt(1);
				std::cout << "Activitat afegida amb ID: " << id << std::endl;
				a.setId(id);
			}
		}
		else
		{
			std::cerr << "L'activitat no s'ha pogut afegir" << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool SqlManager::removeActivitat(int id)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"DELETE FROM activitats WHERE id = ?"
		));

		statement->setInt(1, id);
		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "Activitat amb ID: " << id << " eliminada de la base de dades" << std::endl;
		else
		{
			std::cerr << "No s'ha trobat cap activitat amb Id: " << id << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

Activitat SqlManager::getActivitat(int id)
{
	Activitat activitat;
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"SELECT nom, descripcio, capacitat_maxima FROM activitats WHERE id = ?"
		));

		statement->setInt(1, id);
		std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
		if (result->next())
		{
			activitat.setId(id);
			activitat.setNom(result->getString("nom"));
			activitat.setDescripcio(result->getString("descripcio"));
			activitat.setCapacitatMaxima(result->getInt("capacitat_maxima"));
		}
		else
		{
			std::cerr << "No s'ha trobat cap activitat amb ID: " << id << std::endl;
			activitat.setId(-1);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
	}
	return (activitat);
}

bool SqlManager::updateActivitat(int id, const Activitat &a)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"UPDATE activitats SET nom = ?, descripcio = ?, capacitat_maxima = ? WHERE id = ?"
		));

		statement->setString(1, a.getNom());
		statement->setString(2, a.getDescripcio());
		statement->setInt(3, a.getCapacitatMaxima());
		statement->setInt(4, id);

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "Activitat amb Id: " << id << " actualitzada" << std::endl;
		else
		{
			std::cerr << "Activitat amb Id: " << id << " no trobada" << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool SqlManager::apuntarseActivitat(int usuariID, int activitatId)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"INSERT INTO inscripcions (usuari_id, activitat_id) VALUES (?, ?)"
		));

		statement->setInt(1, usuariID);
		statement->setInt(2, activitatId);

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "Usuari inscrit a l'activitat correctament" << std::endl;
		else
		{
			std::cerr << "L'usuari no s'ha pogut apuntar a l'activitat" << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool SqlManager::desapuntarseActivitat(int usuariID, int activitatId)
{
	try
	{
		std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(
			"DELETE FROM inscripcions WHERE usuari_id = ? AND activitat_id = ?"
		));

		statement->setInt(1, usuariID);
		statement->setInt(2, activitatId);

		int affectedRows = statement->executeUpdate();
		if (affectedRows == 1)
			std::cout << "L'usuari s'ha desinscrit de l'activitat correctament" << std::endl;
		else
		{
			std::cerr << "L'usuari no s'ha pogut desinscriure's de l'activitat" << std::endl;
			return (false);
		}
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "Error de SQL" << e.what() << std::endl;
		return (false);
	}
	return (true);
}

//executeUpdate---> INSERT, UPDATE, DELETE. Retorna el num de files modificades
//executeQuery----> SELECT. Retorna un objecte ResultSet amb les files de dades