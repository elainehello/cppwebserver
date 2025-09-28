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
                         { return "Welcome to the cppwebserver!"; });

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
            return crow::response(200, response.dump());
        } catch (const std::exception &e) {
            return crow::response(500, e.what());
        } });

    // POST /users - Add a new user
    CROW_ROUTE(app, "/users").methods("POST"_method)([](const crow::request &req)
                                                     {
        try {
            auto body = json::parse(req.body);

            pqxx::connection conn(DB_CONNECTION);
            pqxx::work txn(conn);

            txn.exec_params(
                "INSERT INTO users (gender, age, estimated_salary, purchased) VALUES ($1, $2, $3, $4)",
                body["gender"].get<std::string>(),
                body["age"].get<int>(),
                body["estimated_salary"].get<int>(),
                body["purchased"].get<bool>());

            txn.commit();

            return crow::response(201, "User created successfully");
        } catch (const std::exception &e) {
            return crow::response(500, e.what());
        } });

    // PUT /users/:id - Update a user
    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)([](const crow::request &req, int user_id)
                                                          {
        try {
            auto body = json::parse(req.body);

            pqxx::connection conn(DB_CONNECTION);
            pqxx::work txn(conn);

            txn.exec_params(
                "UPDATE users SET gender=$1, age=$2, estimated_salary=$3, purchased=$4 WHERE user_id=$5",
                body["gender"].get<std::string>(),
                body["age"].get<int>(),
                body["estimated_salary"].get<int>(),
                body["purchased"].get<bool>(),
                user_id);

            txn.commit();

            return crow::response(200, "User updated successfully");
        } catch (const std::exception &e) {
            return crow::response(500, e.what());
        } });

    // DELETE /users/:id - Delete a user
    CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)([](int user_id)
                                                             {
        try {
            pqxx::connection conn(DB_CONNECTION);
            pqxx::work txn(conn);

            txn.exec_params("DELETE FROM users WHERE user_id=$1", user_id);

            txn.commit();

            return crow::response(200, "User deleted successfully");
        } catch (const std::exception &e) {
            return crow::response(500, e.what());
        } });
}
