# Shepherd Dog

Shepherd Dog is a single-player game based on Base0, which is the starter code for the game0 in the 15-466-f17 course developed by Jim McCann.

In the game, the player will control the location of the shepherd dog with the mouse. The goal is to prevent any of the sheep from escaping from the area by touching them, forcing them to change their direction. Watch out that the sheep would become faster as the time passes!

The design document with more details is available [here](http://graphics.cs.cmu.edu/courses/15-466-f17/game0-designs/hungyuc/) .

## Requirements

 - glm
 - libSDL2
 - modern C++ compiler


## Building

This game was built with Visual Studio 2015 on Windows 10 when developing.

Clone [kit-libs-win](https://github.com/ixchow/kit-libs-win) into the kit-libs-win subdirectory:
```
  git clone https://github.com/ixchow/kit-libs-win
```
Now you can:
```
  cl.exe /EHsc /W3 /WX /MD /Ikit-libs-win\out\include /Ikit-libs-win\out\include\SDL2 main.cpp Draw.cpp gl_shims.cpp /link /SUBSYSTEM:CONSOLE /LIBPATH:kit-libs-win\out\lib SDL2main.lib SDL2.lib OpenGL32.lib
```

