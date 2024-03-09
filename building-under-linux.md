# Building under Linux

## Installing the dependencies

*Zeitgeist* requires
- *make* Version 4.3.0 or better.
- *gcc* Version 11.4.0 or better.
- *cmake* Version 3.22.1 or better.
- Optional: *gdb* Version 12.1 or better.

## Building
- Checkout `zeitgeist` [https://github.com/michaelheilmann/zeitgeist.git](https://github.com/michaelheilmann/zeitgeist.git) into a directory in your file system.
  For the remainder of this manual, we denote this by the `<source-directory>`.
- Create a directory in which the build files will be generate.
  That directory should not be below the source directory `<source-directory>`.
  For the remainder of this manual, we denote this by the `<build-directory>`.
- Open a console and enter the build directory `<build-directory>`.
- Enter `cmake <source-directory>`.
- The makefiles should have been generated in `<build-directory>`.

The above builds *Zeitgeist* without debug information. To build *Zeitgeist* with debug information add the parameter `-DCMAKE_BUILD_TYPE="Debug"` to the cmake command.
