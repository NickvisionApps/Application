export VCPKG_ROOT=${PWD}/vcpkg
ls -a
mkdir out-linux
cd out-linux
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install . --prefix /app