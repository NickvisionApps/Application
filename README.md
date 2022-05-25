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
![HomePage](https://user-images.githubusercontent.com/17648453/170175673-698dac78-fb2b-463a-98b2-fda0d139aab0.png)
![FormPage](https://user-images.githubusercontent.com/17648453/170175690-afb43229-6a41-4748-ad5e-23e7dd584288.png)
![PreferencesDialog](https://user-images.githubusercontent.com/17648453/170175708-3102ea68-1d6a-4fb7-af89-041cac7def8d.png)
![LightMode](https://user-images.githubusercontent.com/17648453/170175726-4d033c26-a866-4497-9037-e3b7c42209a9.png)
![AboutDialog](https://user-images.githubusercontent.com/17648453/169936936-de9f17e9-ee75-40aa-97f3-5ac5b119a6da.png)

# Dependencies
- [c++20](https://en.cppreference.com/w/cpp/20)
- [gtk4](https://gtk.org/)
- [libadwaita](https://gnome.pages.gitlab.gnome.org/libadwaita/)
- [GCR_CMake](https://github.com/Makman2/GCR_CMake)
- [curlpp](http://www.curlpp.org/)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

