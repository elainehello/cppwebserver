message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(Crow)
find_package(nlohmann_json)
find_package(spdlog)
find_package(cpprestsdk)
find_package(libpqxx)

set(CONANDEPS_LEGACY  Crow::Crow  nlohmann_json::nlohmann_json  spdlog::spdlog  cpprestsdk::cpprestsdk  libpqxx::pqxx )