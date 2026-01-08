FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    git \
    build-essential \
    libboost-all-dev \
    googletest \
    libgtest-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/google/benchmark.git /tmp/benchmark \
    && cd /tmp/benchmark \
    && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_GTEST_TESTS=OFF .. \
    && make -j$(nproc) \
    && make install \
    && rm -rf /tmp/benchmark