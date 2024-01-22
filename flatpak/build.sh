tar -xf vcpkg-linux.tar.xz
export VCPKG_ROOT="${PWD}/vcpkg"
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install . --prefix "/app"