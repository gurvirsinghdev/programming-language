# Using ubuntu as the base image for the container.
FROM ubuntu:24.04

# Telling the opreating system that tty is not available. 
ENV DEBIAN_FRONTEND=noninteractive

# Installing required dependencies. 
RUN apt-get update && apt-get install -y \
    git \
    clang \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Setting the working directory for the container. 
WORKDIR /app