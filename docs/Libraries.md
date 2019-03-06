# Project Libraries
This page will hold documentation links for major libraries used by this project, along with brief descriptions of their role in this project.

## [JUCE](https://docs.juce.com/master/index.html)
The JUCE Library is the foundation this project is built upon. Pocket-Home Bismuth uses the JUCE Library's GUI system, event handling, thread handling, unit test system, and innumerable other features.

## [libnm-glib](https://developer.gnome.org/libnm-glib/stable/)
Libnm-glib connects with NetworkManager to monitor network connection status and to provide network connection controls. Pocket-Home Bismuth uses libnm-glib to implement its [Wifi connection module](./modules/Wifi.md). This library is deprecated, but still in use because the PocketCHIP's Debian Jesse repositories lack the updated libnm library.

## [GLib](https://developer.gnome.org/glib/)
GLib provides a large collection of basic tools used to implement other libraries and applications, including libnm-glib. Pocket-Home Bismuth uses GLib's object and event systems to work with LibNM, and its D-Bus communication tools to handle additional inter-process communication with NetworkManager. This project's [GLib module](./modules/GLib.md) exists to simplify interaction with the GLib library.
