FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install flex bison graphviz gawk clang-12 lldb-12 lld-12 clangd-12 clang-tidy-12 clang-format-12 clang-tools-12 llvm-12-dev llvm-12-tools libomp-12-dev libc++-12-dev libc++abi-12-dev libclang-common-12-dev libclang-12-dev libclang-cpp12-dev libunwind-dev

