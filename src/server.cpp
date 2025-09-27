#include "server.hpp"
#include "user.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

// Database conntection string (education purpose) no recomended hardcoded credentials
const std::string DB_CONNECTION;
DB_CONNECTION = "host=localhost port=5432 dbname=mydatabase user=admin password=admin";

void setup_routes(crow::SimpleApp &app)
{
    // Root route
    CROW_ROUTE(app, "/")([](){
        return ("Welcome to the cppwebserver!");
    });

    // GET /users - Return all users
    
    // POST /users - Add a new user
    
    // PUT /users/:id - Update a user

    // DELETE /users/:id - Delete a user
}
