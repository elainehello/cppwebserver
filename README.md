````markdown
# cppwebserver

This project sets up a **C++ web server (`cppwebserver`)** with a **PostgreSQL database** using Docker Compose. Follow the steps below to build, configure, and run the application.

---

## Prerequisites

1. **Install Docker**  
   Ensure Docker is installed on your system. Follow the official [Docker installation guide](https://docs.docker.com/get-docker/).

2. **Install Docker Compose**  
   Docker Compose is included with Docker Desktop. For Linux, follow the [Docker Compose installation guide](https://docs.docker.com/compose/install/).

3. **Log in to Docker Hub**  
   Open a terminal and run:

   ```bash
   docker login
````

4. **Clone the repository**

   ```bash
   git clone <repository-url>
   cd cppwebserver
   ```

---

## Build Docker Images

Build the Docker images for PostgreSQL and the C++ web server:

```bash
# Build PostgreSQL image
docker build -t elainehello/my-postgres-db:v1.1 .

# Build cppwebserver image
docker build -t elainehello/cppwebserver:v1.0 .
```

---

## Start Services

Start the services in **detached mode**:

```bash
docker compose up -d
```

Check the status of the services:

```bash
docker compose ps
```

View logs:

```bash
docker compose logs -f
```

Stop and remove containers, networks, and volumes:

```bash
docker compose down
```

---

## Test the Web Server

The `cppwebserver` listens on **port 18080**. Test endpoints using `curl`:

### GET /users

```bash
curl -v http://127.0.0.1:18080/users
```

### POST /users

```bash
curl -v -X POST -H "Content-Type: application/json" \
-d '{"gender": "male", "age": 30, "estimated_salary": 50000, "purchased": true}' \
http://127.0.0.1:18080/users
```

### PUT /users/:id

```bash
curl -v -X PUT -H "Content-Type: application/json" \
-d '{"gender": "female", "age": 25, "estimated_salary": 60000, "purchased": false}' \
http://127.0.0.1:18080/users/1
```

### DELETE /users/:id

```bash
curl -v -X DELETE http://127.0.0.1:18080/users/1
```