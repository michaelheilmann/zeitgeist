# IdLib
*IdLib* provides
- matrix and vector math
- color math
for embedding into C programs.
IdLib is licensed under the [Zlib license](LICENSE).

## Integration into your CMake project
Create a directory for external dependencies within your project directory.
For example `3rdParty`. Create a folder `IdLib` in that directory.
Download/checkout *IdLib* into that folder such that the contents of the directory
in which this `README.md` file is located in directly reside under `3rdParty/IdLib`.
Add to your project the `3rdParty/IdLib/CMakeLists.txt` using `add_subdirectory`.
Link your target `YourTarget` to *IdLib* using `target_link_libraries(YourTarget idlib)`.
In your C/C++ code, you can include now the functionality of idlib by adding the preprocessor include directive `#include "idlib.h"`.
Integrating IdLib into the sources of your project is intended use:

- Create a directory for external dependencies within your project directory. For example `3rdParty`.
- Create a folder `IdLib` in that directory.
- Download/checkout *IdLib* into that folder such that the contents of the directory in which this `README.md` file is located in directly resides under `3rdParty/IdLib`.
- Add to your project the `3rdParty/IdLib/CMakeLists.txt` using `add_subdirectory`.
- Link your target `YourTarget` to *IdLib* using `target_link_libraries(YourTarget idlib)`.
- In your C/C++ code, you can include now the functionality of *IdLib by adding the preprocessor include directive `#include "idlib.h"`.

## Building
For development (or if you really want to use prebuilt binaries), you can do standalone builds of *IdLib*.

*IdLib* builds *Windows 11* and *Linux*.
- [Building *IdLib*: Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building *IdLib*: Linux](building-under-linux)
