# Application
<img src="NickvisionApplication.Shared/Resources/org.nickvision.application.png" width="100" height="100"/>

 **A template for creating Nickvision applications**

# Features
- A cross-platform C# application
  - Windows UI in Windows App SDK (WinUI 3)
  - GNOME UI in gir.core (Gtk4/Libadwaita)

# Chat
<a href='https://matrix.to/#/#nickvision:matrix.org'><img width='140' alt='Join our room' src='https://user-images.githubusercontent.com/17648453/196094077-c896527d-af6d-4b43-a5d8-e34a00ffd8f6.png'/></a>

# Screenshots
![WinUILight](https://user-images.githubusercontent.com/17648453/207794202-2ec536e0-106c-451d-b380-2091c2db96bf.png)
![WinUIDark](https://user-images.githubusercontent.com/17648453/207794094-f6e371b0-9c0e-4356-b9ea-7cdca7eb3b05.png)
![GNOMELight](https://user-images.githubusercontent.com/17648453/206729313-01c7f089-2393-49a3-9dd6-a861b7d7c84f.png)
![GNOMEDark](https://user-images.githubusercontent.com/17648453/206729405-d73d5912-7773-451d-941d-297ddf4d9c6b.png)

# Translating
Everyone is welcome to translate this app into their native or known languages, so that the application is accessible to everyone.

To start translating the app, fork the repository and clone it locally.

In the `NickvisionApplication.Shared/Resources` folder you will see a file called `String.resx`. This is a C# resource file that contains all the strings for the application. Simply copy that file and rename it `String.<lang-code>.resx`. For example, if I'm creating an Italian translation, the copied file would be called `Strings.it.resx`. Once you have your copied file, simply replace each `<value>` block of each `<data>` string block with your language's appropriate translation.

To check your translation file, make sure your system is in the locale of the language you are translating and run the app. You should see your translated strings!

Once all changes to your translated file are made, make sure the file is in the path `NickvisionApplication.Shared/Resources/String.<lang-code>.resx` and commit these changes, and then create a pull request to the project.

# Dependencies
- [.NET 7](https://dotnet.microsoft.com/en-us/)

