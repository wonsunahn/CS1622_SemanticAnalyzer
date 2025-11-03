FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install flex bison graphviz gawk clang-12 lldb-12 lld-12 clangd-12 clang-tidy-12 clang-format-12 clang-tools-12 llvm-12-dev llvm-12-tools libomp-12-dev libc++-12-dev libc++abi-12-dev libclang-common-12-dev libclang-12-dev libclang-cpp12-dev

# Set clang-12 as the default clang version
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-12 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-12 100 \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-12 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-12 100 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-12 100 \
    && update-alternatives --install /usr/bin/lld lld /usr/bin/lld-12 100 \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-12 100 \
    && update-alternatives --set clang /usr/bin/clang-12 \
    && update-alternatives --set clang++ /usr/bin/clang++-12 \
    && update-alternatives --set clangd /usr/bin/clangd-12 \
    && update-alternatives --set clang-tidy /usr/bin/clang-tidy-12 \
    && update-alternatives --set clang-format /usr/bin/clang-format-12 \
    && update-alternatives --set lld /usr/bin/lld-12 \
    && update-alternatives --set lldb /usr/bin/lldb-12

# Link libunwind8.so.1 to libunwind.so since clang-12 looks for libunwind.so, and it expects version 1
# For x86 architectures
RUN [ -f "/usr/lib/x86_64-linux-gnu/libunwind.so.1" ] && ln -s /usr/lib/x86_64-linux-gnu/libunwind.so.1 /usr/lib/x86_64-linux-gnu/libunwind.so || exit 0
# For ARM architectures
RUN [ -f "/usr/lib/aarch64-linux-gnu/libunwind.so.1" ] && ln -s /usr/lib/aarch64-linux-gnu/libunwind.so.1 /usr/lib/aarch64-linux-gnu/libunwind.so || exit 0