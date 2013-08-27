Raytracer
=========

Simple Raytracer built in C++ for my graphics course.

## Compiling

Uses the Microsoft C++ compiler and nmake for compilation. Command to compile `nmake all`

## Running

The raytracer takes a `.scn` file that specifies the lights, objects and object properties. These input files are located in `Raytracer/e`.
The raytracer can be run by passing the name of the file as an argument with the executable.

### Examples

![Example 3](https://raw.github.com/hnathani/Raytracer/master/prerendered/example3.png)
`raytracer e/example3.scn anti` `anti` adds anti aliasing

![Example 4](https://raw.github.com/hnathani/Raytracer/master/prerendered/example4.png)
`raytracer e/example4.scn`

![Example 5](https://raw.github.com/hnathani/Raytracer/master/prerendered/example5.png)
`raytracer e/example5.scn`

![Example motionblur](https://raw.github.com/hnathani/Raytracer/master/prerendered/motionblur.png)
- `raytracer e/motionblur.scn`

![Example motionblur](https://raw.github.com/hnathani/Raytracer/master/prerendered/spotlight.png)
- `raytracer e/spotlight.scn`



