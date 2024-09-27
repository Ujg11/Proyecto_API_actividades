#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>

int main() {
    try {
        // Crea un objecte de controlador de MySQL
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // Inicialitza el controlador
        driver = sql::mysql::get_mysql_driver_instance();

        // Intenta establir una connexió (canvia 'user', 'password', 'db' amb les teves dades)
        con = driver->connect("tcp://127.0.0.1:3306", "ojimenez", "xxxx");

        // Selecciona la base de dades
        con->setSchema("API_Activitats_db");

        std::cout << "Conexió a la base de dades establerta amb èxit!" << std::endl;

        // Tanca la connexió
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "Error de connexió: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Excepció: " << e.what() << std::endl;
    }
    return 0;
}
