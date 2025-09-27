FROM gcc:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    libpq-dev \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

# Create a virtual environment and install Conan
RUN python3 -m venv /opt/venv && \
    /opt/venv/bin/pip install --upgrade pip setuptools && \
    /opt/venv/bin/pip install conan

# Add the virtual environment to PATH
ENV PATH="/opt/venv/bin:$PATH"

# Create a valid Conan default profile
RUN mkdir -p /root/.conan2/profiles && \
    echo "[settings]" > /root/.conan2/profiles/default && \
    echo "os=Linux" >> /root/.conan2/profiles/default && \
    echo "arch=x86_64" >> /root/.conan2/profiles/default && \
    echo "build_type=Release" >> /root/.conan2/profiles/default && \
    echo "compiler=gcc" >> /root/.conan2/profiles/default && \
    echo "compiler.version=15" >> /root/.conan2/profiles/default && \
    echo "compiler.libcxx=libstdc++11" >> /root/.conan2/profiles/default && \
    echo "compiler.cppstd=17" >> /root/.conan2/profiles/default

# Set the CFLAGS environment variable globally
ENV CFLAGS="-std=c99 -D_POSIX_C_SOURCE=200809L"

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Ensure the build directory is clean
RUN rm -rf build

# Build the cppwebserver
RUN mkdir build && cd build && \
    conan install .. --build=missing && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make

# Expose the server port
EXPOSE 18080

# Run the cppwebserver
CMD ["./build/cppwebserver"]