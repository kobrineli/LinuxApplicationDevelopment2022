### Build rhasher

    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
    cmake --build .

### Build with readline

    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DREADLINE=1 ..
    cmake --build .

### Testing

    cd build
    make test

