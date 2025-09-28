FROM gcc:12

# Install dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    libpq-dev \
    make \
    wget \
    perl \
    pkg-config \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install the latest version of CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-linux-x86_64.sh && \
    chmod +x cmake-3.27.7-linux-x86_64.sh && \
    ./cmake-3.27.7-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.27.7-linux-x86_64.sh

# Create a virtual environment and install Conan
RUN python3 -m venv /opt/venv && \
    /opt/venv/bin/pip install --upgrade pip setuptools conan

# Add the virtual environment to PATH
ENV PATH="/opt/venv/bin:$PATH"

# Create a valid Conan default profile
RUN mkdir -p /root/.conan2/profiles && \
    echo "[settings]" > /root/.conan2/profiles/default && \
    echo "os=Linux" >> /root/.conan2/profiles/default && \
    echo "arch=x86_64" >> /root/.conan2/profiles/default && \
    echo "build_type=Release" >> /root/.conan2/profiles/default && \
    echo "compiler=gcc" >> /root/.conan2/profiles/default && \
    echo "compiler.version=12" >> /root/.conan2/profiles/default && \
    echo "compiler.libcxx=libstdc++11" >> /root/.conan2/profiles/default && \
    echo "compiler.cppstd=17" >> /root/.conan2/profiles/default && \
    echo "[conf]" >> /root/.conan2/profiles/default && \
    echo "tools.build:cflags=['-std=gnu11']" >> /root/.conan2/profiles/default

# Do NOT set global CFLAGS/CPPFLAGS (avoids breaking OpenSSL build)
ENV CONAN_CPU_COUNT=1

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Ensure the build directory is clean and build the cppwebserver
RUN rm -rf build && \
    conan install . --output-folder=build --build=missing -s build_type=Release -o openssl/*:no_async=True && \
    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-std=c99 -D_POSIX_C_SOURCE=200809L" && \
    cmake --build build -j$(nproc)

# Expose the server port
EXPOSE 18080

# Run the cppwebserver
CMD ["./build/cppwebserver"]