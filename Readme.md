#Multiplatform Arcade Game Engine v2
![Imgur](http://i.imgur.com/Qf707M2.png)
![Imgur](http://i.imgur.com/J8jh7Uo.png)

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

#Ish good, ja!
You can easily compile the engine with help from CMake (https://cmake.org/) to generate a visual studio project. If you're going to make a game with it, however:
- Wait for me to actually finish and document the engine (for now, the "documentation" is typing dump() in the console)

#Compiling MAGE
- If you want to use MAGE you need to compile it.
- Cmake the project anywhere you want.
- No, you shouldn't need to import anything. Dependencies are included.
- THAT IS, unless you need MAGEdit. In that case, you're going to need wxWidgets installed somewhere on your computer for Cmake to let you compile the editor.

#Folder Structure?
- Cmake will generate a folder for your project when it plonks down the MAGE executable. You have:
  - A "Modules" folder for DLL extensions based off the extension template (included, probably) and containing that
  - A "Packs" folder for your resources and scripts.
  - Some DLLs the engine needs next to MAGE.exe to start up including sfml and openal DLLs.
  - A copy of MAGEdit.exe (if you compiled it)
- The Packs folder is the most important. MAGE Games are meant to be moddable - when the engine starts, every "pack" in the Packs folder will be loaded (via execution of main.chai). Your game content need only reside in one Pack, but you could spread it over multiple packs too.
- At present, "packs" are just folders that must contain main.chai.
- The Modules folder allows you to add extra capabilities to the engine in C++ rather than writing chaiscript code, via new DLL files. You can also write general code for your game in C++, I guess.

#Scripting
- The game engine uses chaiscript as its scripting engine.
- This is a big topic and the wiki (soon) elaborates on it.
