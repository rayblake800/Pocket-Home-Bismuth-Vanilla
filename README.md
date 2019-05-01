# ![Bismuth project image](./assets/login/chipVector.png?raw=true "Bismuth project image") Pocket-Home Bismuth Edition
An application launcher and system manager for portable Linux devices.

![Paged Menu](./docs/images/paged.gif?raw=true "Paged Menu") ![Scrolling Menu](./docs/images/scrolling.gif?raw=true "Scrolling Menu")

Pocket-Home Bismuth is a complete rewrite of the Pocket-Home launcher. It introduces a full-featured, configurable application menu, improved Wifi control and performance, and initial support for devices other than the PocketC.H.I.P. The project's biggest focus is on creating extensively documented, reusable code, to make it as easy as possible to update, extend, and adapt.

- [Building and Installation](./docs/BuildAndInstall.md)
- [Documentation](./docs/Main.md)
- [Configuration File Guide](./docs/Configuration.md)
- [Change Log](./docs/Changelog.md)
- [Project Task List](./docs/taskList/TODO.md)

Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications. The menu interface and contents are fully customizable, either within the application, or by editing the application's JSON configuration files.

### Compatibility
Support for devices other than the PocketC.H.I.P. is available, but incomplete.  The application UI will automatically adapt to any screen resolution, but layout and image asset configuration may need some adjustment. The biggest limitation is the lack of input flexibility, as application control still requires a full keyboard and a mouse or touch screen. To enable system control features on other devices, some of the default system control commands may need to be overridden. See the [system command file documentation](./docs/configuration/commands.md) for more information on how to provide replacement commands. To disable features that depend on the C.H.I.P i2c bus or other C.H.I.P-specific hardware, compile with `make CHIP_FEATURES=0`.

### Development Goals
  Now that NTC is gone and the PocketCHIP has ceased production, the primary goal of this project is to make Pocket-Home as easy as possible to adapt to similar handheld GNU/Linux devices. Future updates will focus on support for a greater variety of hardware options, input controls, and system configurations. See the [project tasklist](./docs/TODO.txt) for more information on future project goals.


  Pocket-Home Bismuth is inspired by my experiences with the GP2X, the Tapwave Zodiac, and of course, the PocketCHIP itself. Now that niche handheld devices like these are relatively affordable and easy to design, I want to make Bismuth into my ideal interface for using these devices.
