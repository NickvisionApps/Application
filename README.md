# Application
<img src="resources/org.nickvision.application.svg" width="100" height="100"/>

 **Create new Nickvision applications**

## Features
- A template for creating cross-platform desktop apps in C++
    - Linux support through GTK4/libadwaita
    - Windows support through WindowsAppSDK/WinUI

## Chat
<a href='https://matrix.to/#/#nickvision:matrix.org'><img width='140' alt='Join our room' src='https://user-images.githubusercontent.com/17648453/196094077-c896527d-af6d-4b43-a5d8-e34a00ffd8f6.png'/></a>

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for details on how can you help the project and how to provide information so we can help you in case of troubles with the app.

## Building Manually
Application uses `cmake` as its build system and `vcpkg` to *optionally* manage its dependencies.

Ensure `cmake` and `vcpkg` are installed on your system before building and installing Application.

A C++20 compiler is also required to build Application.

### Dependencies
The following are a list of dependencies used by Application. 

The recommendation is to (and below commands will) use vcpkg to pull these dependencies. However, vcpkg is not a requirement as long as the system provides these dependencies correctly.

#### All Platforms
- `libnick`

#### Linux
- `blueprint-compiler` (Not available from vcpkg)
- `libxmlpp`

### Configuring vcpkg
1. Set the `VCPKG_ROOT` environment variable to the path of your vcpkg installation's root directory.
#### Windows
1. Set the `VCPKG_DEFAULT_TRIPLET` environment variable to `x64-windows`
1. Run `vcpkg install libnick`
#### Linux
1. Set the `VCPKG_DEFAULT_TRIPLET` environment variable to `x64-linux`
1. Run `vcpkg install libnick libxmlpp`

### Building
1. First, clone/download the repo.
1. Open a terminal and navigate to the repo's root directory.
1. Create a new `build` directory and `cd` into it.
#### Windows
1. From the `build` folder, run `cmake .. -G "Visual Studio 17 2022"`.
1. From the `build` folder, run `cmake --build . --config Release`.
1. After these commands complete, Application will be successfully built and its binaries can be found in the `org.nickvision.application.winui/Release` folder of the `build` folder.
#### Windows (Portable)
1. From the `build` folder, run `cmake .. -G "Visual Studio 17 2022" -DBUILD_AS_PORTABLE=ON`.
1. From the `build` folder, run `cmake --build . --config Release`.
1. After these commands complete, Application will be successfully built and its binaries can be found in the `org.nickvision.application.winui/Release` folder of the `build` folder.
#### Linux
1. From the `build` folder, run `cmake .. -DCMAKE_BUILD_TYPE=Release`.
1. From the `build` folder, run `cmake --build .`.
1. After these commands complete, Application will be successfully built and its binaries can be found in the `org.nickvision.application.gnome` folder of the `build` folder.

## Code of Conduct

This project follows the [GNOME Code of Conduct](https://conduct.gnome.org/).
