# DraupnirLib

Reusable building blocks for C++ and Qt applications.

## Overview

Started as attempt to collect reusable code in one place and ended with a mini-library containing utility classes, widgets, and modules that may be used in many applications. The code base is intentionally modular: you can pull in just the pieces you need or reuse the entire set as the foundation for a Qt application.

The project is named after Odin’s legendary ring Draupnir - a symbol of endless replication - to reflect the goal of creating components that can be shared and extended as the surrounding applications evolve.

## Project status

This repository is actively developed and refactored. APIs are not frozen, and backwards-incompatible changes may (and will) appear without notice until v1.0.0 will be tagged and released.

## Highlights

- **Header-only containers** – e.g. `fixed_map` and `fixed_tuple_map` for compile-time associative data.
- **Application settings infrastructure** – thin wrappers around `QSettings` to keep persistent configuration in sync.
- **Templates for menus and menu handlers** - creating menus using templates and create handlers to this templte menus using another templates.
- **Message system** – templates and widgets to surface notifications to the end user.
- **Proxy helper utilities** – reusable dialogs and widgets for configuring `QNetworkProxy` instances.
- **UI bricks and utilities** – small components to reduce boilerplate when building Qt interfaces.

## Repository layout

| Path / module         | Description |
|-----------------------|-------------|
| `include/containers`  | Header-only utilities such as `fixed_map` and supporting traits. |
| `include/handler_templates` | Template implementations of menu handlers and supporting policies. |
| `include/message_system` | Glue code that combines handlers, UI builders, and message traits. |
| `include/proxy_helper` | Proxy helper utilities and Qt widgets. |
| `include/settings_registry` | Helpers for registering and validating application settings. |
| `include/ui_bricks` | Small reusable Qt UI components. |
| `modules/*.pri` | Qt project include (`.pri`) files to import individual modules into a `.pro` project. |
| `tests/` | Qt Test projects covering containers, handlers, message system, proxy helper, and more. |

### Requirements

- A C++17-capable compiler (tested with GCC)
- Qt 5.15 or newer (Qt 6 is supported by most modules, but specific widgets may still rely on Qt 5 APIs).
- CMake or qmake, depending on how you prefer to integrate the modules.

### Integration options

1. **Qt projects** – include the desired `.pri` file from `modules/` in your `.pro` file.
2. **CMake projects** – add the `include/` directory to your include path and link against Qt modules required by the classes you
   use.

## Additional resources

- [LGPL v3.0 license](https://choosealicense.com/licenses/lgpl-3.0/)
- [Qt documentation](https://doc.qt.io/) – background for Qt concepts used throughout the library.

## License

This project is licensed under the [GNU Lesser General Public License v3.0](https://choosealicense.com/licenses/lgpl-3.0/).
