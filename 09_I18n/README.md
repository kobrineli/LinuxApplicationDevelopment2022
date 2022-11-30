### Build guesser

    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
    cmake --build .

### Run with Russian language

    cd build
    LANG=ru_RU.UTF-8 ./guess
