# IdLib Math
*IdLib Math* provides
- matrix and vector math
- color math
for embedding into C programs.
*IdLib Math* is licensed under the [Zlib license](LICENSE).

## Integration into your CMake project
Integrating IdLib into the sources of your CMake-based project is the intended use of *IdLib Math*.

Follow these steps to integrate this library into your CMake-based project:

- Create a directory for external dependencies within your project directory. For example `3rdParty`.
- Create a folder `IdLib-Math` in that directory.
- Download/checkout *IdLib Math* into that folder such that the contents of the directory in which this `README.md` file is located in directly resides under `3rdParty/IdLib-Math`.
- Add to your project the `3rdParty/IdLib-Math/CMakeLists.txt` using `add_subdirectory`.
- Link your target `YourTarget` to *IdLib Math* using `target_link_libraries(YourTarget idlib-math)`.
- In your C/C++ code, you can include now the functionality of *IdLib by adding the preprocessor include directive `#include "idlib-math.h"`.

## Building
For development (or if you really want to use prebuilt binaries), you can do standalone builds of *IdLib Math*.

*IdLib Math* builds *Windows 11* and *Linux*.
- [Building *IdLib Math*: Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building *IdLib Math*: Linux](building-under-linux)
