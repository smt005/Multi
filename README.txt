
An amateur project to explore everything that may be required to develop your own games.

Current versions, for "Windows 10" and "OSX Sierra".

- To generate a project a "Windows 10" (debug), required "Make 3.81", "CMake 2.8", "Visual Studio 17". Command in the console "make WIN_GLES".
- To generate a project a "OSX High Sierra 10" (debug), required "Make 3.81", "CMake 2.8", "XCode 9.2". Command in the terminal "make OSX". To run an application not from Xcode, you must copy the file "Multi/Project/ThirdParty/GLFW/OSX/libglfw.3.dylib" to directory "/usr/local/lib/".

Control (GameTerrain):
Esc - closing the program.
WASDRF - moving the camera.
Shift (holding) - decrease the speed of moving the camera 4 times.
Space - creation of dynamic objects.
L - display of the direction of light.

Used libraries:
- OpenGL, OpenGL ES 2
- GLFW
- GLM
- FreeImage
- Bullet 3

Link:
https://sites.google.com/site/intelligencecells/home
https://youtu.be/ApGcZ2DcSfU?list=PLO-E0yPxWRZG1sPBP7UzHfQ8pLoj2yUWp