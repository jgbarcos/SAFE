# SAFE 

Simple C++ 2D game engine built over SDL.

Aiming for flexibility by using an Entity-Component-System architecture with Lua scripting support.

### Prerequisites

(Note: Currently tested under Ubuntu with the GCC compiler.)

A C++14 compatible compiler.

It requires the following libraries to be installed:

 - SDL2
 - SDL2_image
 - SDL2_ttf
 - SDL2_gfx
 - Box2D
 - Lua5.2 (it would probably also work with Lua5.1)
 - [Sol2](https://github.com/ThePhD/sol2) (already provided with a single header file at /include/sol.hpp)
 
Download [this folder](https://www.dropbox.com/sh/7dvd05e6xlt1t14/AABFb1CHoQ_S2wz9L4k3wsyMa?dl=0) into the project folder. It contains some graphic assets.

### Usage


Run CMake to generate build files and compile:

```
cmake .
make
```

Generate documentation files with Doxygen:

```
doxygen Doxyfile
```

### Acknowledgments
 
- [Lazy Foo's SDL tutorials](http://lazyfoo.net/SDL_tutorials/)
- [Elias Daler's insights on ECS and Lua scripting within C++](https://eliasdaler.github.io/)
 
