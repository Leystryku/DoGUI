Hello DoGUI: (not yet completed) Bloat-free Graphical User interface for C++ with minimal dependencies and a sleek default design

# DoGUI
___

DoGUI is a (not yet completed) project which I developed sometime ago in 2017. It's supposed to be a platform-independent GUI with lots of useful elements and interchangeable
render implementations.

DoGUI was supposed to be integrated in video games, desktop applications, fullscreen applications and consoles.
However, after weeks of development I lost interest so maybe someone else will feel like picking it up and/or using it for his application.

Unlike ImGui which has huge .cpp and .h files, this project focuses a bit more on file structure etc.
It currently only runs on Windows.
To compile it you need to install the Direct X SDK and then it should compile just fine. 
I used Microsoft Visual Studio 2016 and Direct X SDK June to compile it.
There isn't any complicated build process, you can just include the files and use them within your project,
however, depending on your application you may need to adapt the test case.

# Showcase
___

How do I build the current version?
The current version works as a overlay over any Window, whether it may be a game or a application you want to extend.
I used to use premake to generate the project files, compile it with MSVC 2015 and inject it into game processes (e.g. hl2.exe, csgo.exe, EngineX.dat, etc.).


![Showcase image 1](https://github.com/Leystryku/DoGUI/blob/master/showcase/showcase.png?raw=true)
![Showcase image 2](https://github.com/Leystryku/DoGUI/blob/master/showcase/showcase2.png?raw=true)
![Showcase image 3](https://github.com/Leystryku/DoGUI/blob/master/showcase/showcase3.png?raw=true)
![Showcase image 4](https://github.com/Leystryku/DoGUI/blob/master/showcase/showcase4.png?raw=true)

# Todo
___
The important things which I wanted to do back in the day was, finish the implementation for the sliders,
and finish the implementation of the UI elements for table views. After that improvements to the current implementations (e.g. Textboxes might be able to work even better,
even though they already have decent looking smooth sliding etc). And work on cross platform compability as well as support for OpenGL and Vulkan. Once that's done, support for different designs,
easier customizability, plugins. More and better examples etc.

# FAQ
___
Can you help me compile this?
It's relatively easy. First run premake to generate the project files. Then if you have the Direct X SDK and MSVS installed all you need to do is hit compile and it'll build to build/release/ or build/debug/ depending on your settings.

Are you planning to continue this project?
Currently not but maybe some time in the future when I'm working on a game engine or something I may do so.

# Notes
___
If you do use it, I'd appreciate if you gave me some credit and used it for something good.
I'd also like to thank Alex R Sagen (https://steamcommunity.com/id/trinityx/)for creating me the Photoshop files, helping the Theme look good.
