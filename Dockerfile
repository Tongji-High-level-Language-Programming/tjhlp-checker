FROM debian:trixie-slim

# Install dependencies
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
        gnupg2 wget ca-certificates apt-transport-https \
        autoconf automake cmake dpkg-dev file make patch libc6-dev \
        zlib1g-dev libzstd-dev

# Install LLVM
RUN echo "deb https://apt.llvm.org/trixie llvm-toolchain-trixie-22 main" \
        > /etc/apt/sources.list.d/llvm.list && \
    wget -qO /etc/apt/trusted.gpg.d/llvm.asc \
        https://apt.llvm.org/llvm-snapshot.gpg.key && \
    apt-get update && \
    apt-get install -y -t llvm-toolchain-trixie-22 clang-22 clang-tidy-22 clang-format-22 lld-22 libc++-22-dev libc++abi-22-dev libclang-22-dev && \
    for f in /usr/lib/llvm-*/bin/*; do ln -sf "$f" /usr/bin; done && \
    ln -sf clang /usr/bin/cc && \
    ln -sf clang /usr/bin/c89 && \
    ln -sf clang /usr/bin/c99 && \
    ln -sf clang++ /usr/bin/c++ && \
    ln -sf clang++ /usr/bin/g++ && \
    rm -rf /var/lib/apt/lists/*

# Install Developer Tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        git cmake

WORKDIR /workspace