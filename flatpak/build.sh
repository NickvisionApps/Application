export VCPKG_ROOT=${PWD}
mkdir out-linux
cd out-linux
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/app
cmake --build .
rm -rf org.nickvision.application.gnome/CMakeFiles
cmake --install .