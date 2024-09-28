#include "lib/crow_all.h"
#include "inc/ApiController.hpp"
#include "inc/SqlManager.hpp"

int main() {
    crow::SimpleApp app;

    auto sqlManager = std::make_shared<SqlManager>("username", "password", "database");
    ApiController apiController(sqlManager);
    
    apiController.initRoutes(app);

    app.port(8080).multithreaded().run();
    return 0;
}