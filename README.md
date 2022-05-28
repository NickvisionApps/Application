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
![MainWindow](https://user-images.githubusercontent.com/17648453/170398862-fc04696d-a7a9-42f9-a29c-b7d74558a84b.png)
![Working](https://user-images.githubusercontent.com/17648453/170398868-34a99ab0-0834-46dd-89a0-3f7cf0dabd84.png)
![PreferencesDialog](https://user-images.githubusercontent.com/17648453/170175708-3102ea68-1d6a-4fb7-af89-041cac7def8d.png)
![LightMode](https://user-images.githubusercontent.com/17648453/170398887-5a83a2f4-3973-4bff-b5f4-d778c015e970.png)
![AboutDialog](https://user-images.githubusercontent.com/17648453/169936936-de9f17e9-ee75-40aa-97f3-5ac5b119a6da.png)

# Dependencies
- [c++20](https://en.cppreference.com/w/cpp/20) [Only for building the app]
- [GCR_CMake](https://github.com/Makman2/GCR_CMake) [Only for building the app]
- [gtk4](https://gtk.org/)
- [libadwaita](https://gnome.pages.gitlab.gnome.org/libadwaita/)
- [curlpp](http://www.curlpp.org/)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

