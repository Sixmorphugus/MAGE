# Multiplatform Arcade Game Engine
![Imgur](http://i.imgur.com/Qf707M2.png)

Unfinished SFML-based game engine with:
- Scripting engine (chai)
- Object oriented design
- Shader support
- Cameras
- Gamestates
- Levels
- Serialization
- Animation
- Scheduling and tweening
- Resource management
- Tilemap support

You can easily compile the engine with help from CMake (https://cmake.org/) to generate a visual studio project. If you're going to make a game with it, however:
- Wait for me to actually finish and document the engine (for now, the "documentation" is typing dump() in the console)
- Use the MAGEDevkit (https://github.com/Sixmorphugus/MAGEDevkit) which includes a cool level editor and example project already written in for you.

# Using MAGE on its own
- If you want to use MAGE on its own you need to compile it.
- Cmake the project anywhere you want and then compile the versions you need - most likley Debug and Release.
- Import the MAGE library into your IDE the usual way.
- MAKE SURE TO ALSO IMPORT /MAGE'S COPY/ OF THE SFML LIBRARY - includes are in extlibs/SFML/include, and libs are in the bin dir next to MAGE.dll.
- Honestly, MAGEDevkit is quite a bit easier...

# Images of the engine running
![GIF](http://i.imgur.com/m9ghGF6.gif)
![GIF](http://i.imgur.com/HSjQ5Ek.png)
![GIF](http://i.imgur.com/PbXY6ee.gif)
![IMG](http://i.imgur.com/xAaRhDh.png)
![IMG](http://i.imgur.com/vLhTlaJ.png)
![GIF](http://i.imgur.com/fvMkg7q.gif)
![IMG](http://i.imgur.com/yok5V9q.png)
