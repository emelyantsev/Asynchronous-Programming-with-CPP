FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-24.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y \
       libboost-all-dev \
       libgtest-dev \
       cmake \
       build-essential \
       git \
    && cd /usr/src/gtest \
    && cmake . \
    && make \
    && mv lib/*.a /usr/lib \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*