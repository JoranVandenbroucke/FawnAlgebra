@echo off
:: cang
cmake --fresh -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ --log-level=ERROR
:: cmake --fresh -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER=clang++ --log-level=ERROR

:: GCC
:: cmake --fresh -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ --log-level=ERROR
:: cmake --fresh -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER=g++ --log-level=ERROR

:: MSVC
:: cmake --fresh -S . -B build -DCMAKE_BUILD_TYPE=Debug --log-level=ERROR -G "Visual Studio 17 2022" -A x64
:: cmake --fresh -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo --log-level=ERROR -G "Visual Studio 17 2022" -A x64
@echo on
