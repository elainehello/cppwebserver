#include "server.hpp"
#include "user.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include <cstdlib> // For std::getenv

using json = nlohmann::json;

// Database connection string using environment variables
const std::string DB_CONNECTION =
    "host=" + std::string(std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "localhost") +
    " port=" + std::string(std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "5432") +
    " dbname=" + std::string(std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "mydatabase") +
    " user=" + std::string(std::getenv("DB_USER") ? std::getenv("DB_USER") : "myuser") +
    " password=" + std::string(std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "mypassword");

void setup_routes(crow::SimpleApp &app)
{
    // Root route
    CROW_ROUTE(app, "/")([]()
                         { return ("Welcome to the cppwebserver!"); });

    // GET /users - Return all users
    CROW_ROUTE(app, "/users").methods("GET"_method)([]()
                                                    {
        try {
            pqxx::connection conn(DB_CONNECTION);
            pqxx::work txn(conn);

            pqxx::result result = txn.exec("SELECT * FROM users");

            json response = json::array();
            for (const auto &row : result) {
                response.push_back({
                    {"user_id", row["user_id"].as<int>()},
                    {"gender", row["gender"].as<std::string>()},
                    {"age", row["age"].as<int>()},
                    {"estimated_salary", row["estimated_salary"].as<int>()},
                    {"purchased", row["purchased"].as<bool>()}
                });
            }

            txn.commit();
            return crow::response(response.dump());
        } catch (const std::exception &e) {
            return (crow::response(500, e.what()));
        } });

    // POST /users - Add a new user

    // PUT /users/:id - Update a user

    // DELETE /users/:id - Delete a user
}
