
## Important

Disclaimer:
This is not a “classic” standalone library, but a collection of reusable C++ and Qt code used across my projects.
APIs and implementations may change over time as the codebase evolves and is refactored.

This repository is under continuous development — expect changes!
Feel free to open issues or pull requests if you have suggestions or spot bugs.

## Description

Draupnir-lib is a growing set of utility classes, containers, and widgets for C++ and Qt projects. The name is inspired by Odin’s legendary ring Draupnir — a symbol of endless growth and replication.

## Contents

- Pure C++
    - fixed_map: Compile-time associative container for key-to-type value mapping.
    - fixed_tuple_map: Compile-time associative container for key-to-multiple-types mapping. Effectively fixed_map<keys, std::tuple<Args...>> with extra convenience methods.

- C++ / Qt
    - AppSettings: Helper for working with persistent application settings (QSettings-based).
    - MessageSystem: Infrastructure to display notifications to user about the events within the application.
    - ProxyHelper: Consists of: ProxyHelper itself (static utility for converting QNetworkProxy objects to and from QString representation); ProxyEditDialog (dialog window for editing QNetworkProxy settings); ProxyEditWidget (widget for editing and displaying QNetworkProxy objects)

## Odin’s Ring

Draupnir — Odin’s magical ring, which every ninth night would create eight new rings of equal power and value.

## License
[GNU Lesser General Public License v3.0](https://choosealicense.com/licenses/lgpl-3.0/)


