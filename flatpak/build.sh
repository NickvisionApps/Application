export VCPKG_ROOT=${PWD}
mkdir out-linux
cd out-linux
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/app
cmake --build .
cmake --install .
rm -rf /app/lib/org.nickvision.application/CMakeFiles