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

# Lighting
*OpenGL Commons* knows various kinds of lighting a scene.

## Ambient lighting
The ambient light cast on particular fragment only depends on the light color.
Let
- `c_f = (c_f.r, c_f.g, c_f.b)` be the fragment color
- `c_l = (c_l.r, c_l.g, c_l.b)` be the light color
then the resulting fragment color `c'_f` is computed by
```
c'_f = c_l * c_f
```
We implement diffuse lighting interface by the following uniforms:
- `uniform vec3 ambientLightColor`

## Diffuse lighting
The diffuse light cast on a particular fragment depends on the fragments normal, the lights direction and the light color.
Let
- `c_f = (c_f.r, c_f.g, c_f.b)` be the fragment color
- `c_l = (c_l.r, c_l.g, c_l.b)` be the light color
- `n_f = (n_f.x, n_f.y, n_f.z)` be the fragment normal (unit vector)
- `n_l = (n_l.x, n_l.y, n_l.z)` is the light direction (unit vector)

then the resulting fragment color `c'_f` is computed by
```
intensity = max(dot(n_l, n_f), 0)
c'_f = (intensity * c_l) * c_f
```

We implement diffuse lighting interface by the following uniforms:
- `uniform vec3 diffuseLightColor`
- `uniform vec3 diffuseLightDirection`

## ~~Point Lights~~
~~A point light consists of a point. It emits light from the point in every direction.~~

## ~~Directional Lights~~
~~A directional light consists of a plane at infinity. Every point in the plane emits a light into the direction of the plane normal.~~

## ~~Spot Lights~~
~~A spot light consists of a point which emits a cone of light into a direction.~~
