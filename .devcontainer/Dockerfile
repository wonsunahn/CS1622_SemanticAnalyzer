FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-20.04

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install flex bison graphviz gawk clang-10 lldb-10 lld-10 clangd-10 clang-tidy-10 clang-format-10 clang-tools-10 llvm-10-dev llvm-10-tools libomp-10-dev libc++-10-dev libc++abi-10-dev libclang-common-10-dev libclang-10-dev libclang-cpp10-dev

