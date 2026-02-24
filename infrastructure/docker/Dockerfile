# Multi-stage Dockerfile for Voltray Engine
FROM ubuntu:22.04 AS base

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    ninja-build \
    git \
    wget \
    curl \
    pkg-config \
    # Graphics and window system dependencies
    xorg-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libxext-dev \
    libxfixes-dev \
    # Audio dependencies
    libasound2-dev \
    libpulse-dev \
    # Additional utilities
    zip \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Development stage
FROM base AS development

# Install additional development tools
RUN apt-get update && apt-get install -y \
    gdb \
    valgrind \
    doxygen \
    graphviz \
    clang-format \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for development
RUN useradd -m -s /bin/bash vscode && \
    usermod -aG sudo vscode && \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER vscode
WORKDIR /workspace

# Build stage
FROM base AS builder

# Copy source code
COPY . /workspace/

# Clean any residual build artifacts to prevent CMake cache conflicts
RUN rm -rf build build-* CMakeCache.txt CMakeFiles

# Configure and build the project
RUN cmake -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/opt/voltray

RUN cmake --build build --parallel $(nproc)

# Runtime stage
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    libglu1-mesa \
    libxrandr2 \
    libxinerama1 \
    libxcursor1 \
    libxi6 \
    libxext6 \
    libxfixes3 \
    libasound2 \
    libpulse0 \
    && rm -rf /var/lib/apt/lists/*

# Copy built application from builder stage
COPY --from=builder /workspace/build/Voltray /usr/local/bin/voltray
COPY --from=builder /workspace/Shaders /usr/local/share/voltray/Shaders

# Create application user
RUN useradd -m -s /bin/bash voltray
USER voltray

# Set up display for GUI applications
ENV DISPLAY=:0

WORKDIR /home/voltray

# Default command
CMD ["voltray"]
