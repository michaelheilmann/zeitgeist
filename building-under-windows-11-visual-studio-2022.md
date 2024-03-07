# Building under Windows 11/Visual Studio Community 2022

## Installing the dependencies

*Zeitgeist* requires
- *Microsoft Visual Studio Community 2022* Version 17.9.2 or better.
  *Microsoft Visual Studio Community 2022* is available at [https://visualstudio.microsoft.com/vs/community/](https://visualstudio.microsoft.com/vs/community/).
- *Kitware CMake* Version 3.28.3 or better.
  *Kitware CMake* is available at [https://cmake.org/](https://cmake.org/).


##
- Checkout `zeitgeist` [https://github.com/michaelheilmann/zeitgeist.git](https://github.com/michaelheilmann/zeitgeist.git) into a directory in your file system.
  For the remainder of this manual, we denote this by the `<source-directory>`.
- Create a directory in which the build files will be generate.
  That directory should not be below the source directory `<source-directory>`.
  For the remainder of this manual, we denote this by the `<build-directory>`.
- Open a console and enter the build directory `<build-directory>`.
- Enter `cmake <source-directory>`.
- The Visual Studio Community 2022 solution file `Zeitgeist.sln` should have been generated in `<build-directory>`.
