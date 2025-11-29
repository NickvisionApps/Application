# Application
<img src="resources/org.nickvision.application.svg" width="100" height="100"/>

 **Create new Nickvision applications**

## Features
- A template for creating cross-platform desktop apps in C#
    - Linux support through Gtk4/libadwaita
    - Windows support through WindowsAppSDK/WinUI

## Chat
<a href='https://matrix.to/#/#nickvision:matrix.org'><img width='140' alt='Join our room' src='https://user-images.githubusercontent.com/17648453/196094077-c896527d-af6d-4b43-a5d8-e34a00ffd8f6.png'/></a>

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for details on how can you help the project and how to provide information so we can help you in case of troubles with the app.

## Building

Application is a .NET 10 project and can easily be built on any platform. Besides, .NET 10 the following are required system dependencies for building each project:

- Shared
	- [gettext](https://www.gnu.org/software/gettext/)
		- Can be installed on Windows using `msys2`
- WinUI
	- [WindowsAppSDK](https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/)
- GNOME
	- [Gtk4](https://docs.gtk.org/gtk4/)
	- [libadwaita](https://gitlab.gnome.org/GNOME/libadwaita)
	- [blueprint-compiler](https://gitlab.gnome.org/GNOME/blueprint-compiler)

Once all dependencies are available on the system, simply run `dotnet run --project Nickvision.Application.WinUI` or `dotnet run --project Nickvision.Application.GNOME` to run the version of the app for your system.

## Code of Conduct

This project follows the [GNOME Code of Conduct](https://conduct.gnome.org/).
