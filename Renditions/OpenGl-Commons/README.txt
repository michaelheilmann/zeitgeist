# Rendition Library *OpenGl-Common*
Provides OpenGL 3.x windowing & context creation/destruction as well as OpenGL (extension) function linking.

Provides OpenGL 3.x windowing & context creation/destruction:
- (1) Create an OpenGL 1.x window and an OpenGL 1.x context.
- (2) Activate the OpenGL 1.x context link OpenGL 3.x context functions.
- (3) Destroy the OpenGL 1.x context and the OpenGL 1.x window.
      RETAIN the OpenGl 3.x context functions.
- (4) Create a new-style window using the linked function and a new style OpenGL context using the linked function
- (5) Activate the OpenGL 3.x context. Use OpenGL 3.x.
- (6) Relinquish the two linked functions.
- (7) Destroy the OpenGL 3.x context and the OpenGL 3.x window.

# Coordinate Systems
*OpenGL Commons* uses a *right handed coordinate system*.
The positive x axis is pointing from the left side of the canvas to the right side,
The positive y axis is pointing from the bottom side of the canvas to the top side.
The negative z axis is pointing into the screen.
