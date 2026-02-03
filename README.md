# DraupnirLib

Reusable building blocks for C++ and Qt applications.

## Overview

Started as attempt to collect reusable code in one place and ended with a mini-library containing utility classes, widgets, and modules that may be used in many applications. The code base is intentionally modular: you can pull in just the pieces you need or reuse the entire set as the foundation for a Qt application.

The project is named after Odin’s legendary ring Draupnir - a symbol of endless replication - to reflect the goal of creating components that can be shared and extended as the surrounding applications evolve.

## Project status

This repository is actively developed and refactored. APIs are not frozen, and backwards-incompatible changes may (and will) appear without notice until v1.0.0 will be tagged and released.

### Requirements

- A C++20-capable compiler (tested with GCC)
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
