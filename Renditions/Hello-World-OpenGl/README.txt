# Rendition *Hello World*
This rendition opens a window, renders `Hello, World!` into that window.
The application will close if a sigint/sigabort is received or the window is closed.

## How this application works
OpenGL 3.x context creation is complicated on most systems.
The steps are:
- (1) Create an OpenGL 1.x window and an OpenGL 1.x context.
- (2) Activate the OpenGL 1.x context link two functions.
- (3) Destroy the OpenGL 1.x context and the OpenGL 1.x window.
      RETAIN the two linked functions.
- (4) Create a new-style window using the linked function and a new style OpenGL context using the linked function
- (5) Activate the OpenGL 3.x context. Use OpenGL 3.x.
- (6) Relinquish the two linked functions.
- (7) Destroy the OpenGL 3.x context and the OpenGL 3.x window.
