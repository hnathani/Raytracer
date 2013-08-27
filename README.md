Raytracer
=========

Simple Raytracer built in C++ for my graphics course.

## Compiling

Uses the Microsoft C++ compiler and nmake for compilation. Command to compile `nmake all`

## Running

The raytracer takes a `.scn` file that specifies the lights, objects and object properties. These input files are located in `Raytracer/e`.
The raytracer can be run by passing the name of the file as an argument with the executable.

### Examples
- `raytracer e/example1.scn anti`
!(https://raw.github.com/hnathani/Raytracer/master/prerendered/example1.png)
- `raytracer e/example2.scn anti`
- `raytracer e/example3.scn anti`
- `raytracer e/example4.scn anti`
- `raytracer e/example5.scn anti`
- `raytracer e/motionblur.scn anti`
- `raytracer e/spotlight.scn anti`

`anti` adds anti aliasing

