#include "inc/Server.hpp"
#include "inc/SqlManager.hpp"

int main(int argc, char *argv[]) {
	if (argc != 3)
	{
		std::cout << "Executa com ./api <sql_user> <sql_password>" << std::endl;
		return (1);
	}
	SqlManager sqlManager(argv[1], argv[2]);
	Server server(sqlManager);
	server.iniciar();
	return 0;
}

