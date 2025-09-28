#include <crow.h>
#include "server.hpp"

int main()
{
    crow::SimpleApp app;
    

    // Set up routes
    setup_routes(app);

    // Start the server
    app.bindaddr("0.0.0.0").port(18080).multithreaded().run();

    return(0);
}