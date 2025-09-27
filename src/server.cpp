#include "server.hpp"
#include "user.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

// In-memory database of users
std::vector<User> users = {
    {1, "Alice", "alice@example.com"},
    {2, "Bob", "bob@example.com"}};

void setup_routes(crow::SimpleApp &app)
{
    // Root route
    CROW_ROUTE(app, "/")([](){
        return ("Welcome to the cppwebserver!");
    });

    // GET /users - Return all users
    CROW_ROUTE(app, "/users").methods("GET"_method)([]()
                                                    {
        json response = json::array();
        for (const auto& user : users) {
            response.push_back({
                {"id", user.id},
                {"name", user.name},
                {"email", user.email}
            });
        }
        return response.dump(); });

    // POST /users - Add a new user
    CROW_ROUTE(app, "/users").methods("POST"_method)([](const crow::request &req)
                                                     {
        auto body = json::parse(req.body);
        User new_user = {
            static_cast<int>(users.size() + 1),
            body["name"].get<std::string>(),
            body["email"].get<std::string>()
        };
        users.push_back(new_user);
        return crow::response(201, "User added successfully"); });
}
