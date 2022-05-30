# Nickvision Application
 A template for creating Nickvision applications

# Features
- Modern gtk 4 and libadwaita design
    - XML files used for UI definitions (linked as a binary gresource)
    - System/Light/Dark theme support
- Modern C++ codebase
    - CMake build system
    - C++ 20, thread safety, object oriented
- A custom update framework that supports checking for new updates and downloading them all from within the application
- Support for saving application settings via a json configuration

# Screenshots
![MainWindow](https://user-images.githubusercontent.com/17648453/170999392-c5169c58-04d3-4b5b-aaa4-dabee0c183c8.png)
![Working](https://user-images.githubusercontent.com/17648453/170999406-d2c33e22-0763-48a9-9709-7f7943224f03.png)
![LightTheme](https://user-images.githubusercontent.com/17648453/170999416-f155e478-8844-400f-9a55-291b9315b892.png)
![SettingsDialog](https://user-images.githubusercontent.com/17648453/170999436-bc228ec0-4e64-4655-873c-a9a49de7d1e1.png)
![KeyboardShortcuts](https://user-images.githubusercontent.com/17648453/170999442-5173c188-4207-4f89-a4ff-f621d1257463.png)

# Dependencies
- [c++20](https://en.cppreference.com/w/cpp/20) [Only for building the app]
- [GCR_CMake](https://github.com/Makman2/GCR_CMake) [Only for building the app]
- [gtk4](https://gtk.org/)
- [libadwaita](https://gnome.pages.gitlab.gnome.org/libadwaita/)
- [curlpp](http://www.curlpp.org/)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

