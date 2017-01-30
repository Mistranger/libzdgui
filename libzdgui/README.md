# libZDGUI v0.0.1

## About libZDGUI.
libZDGUI is a graphical user interface (GUI) widget toolkit for use in mods for ZDoom-based games. It provides an extensible event-driven framework and a set of default widgets like buttons, windows, listboxes etc.

Features:
  - Generic widget framework which could run any kind of widget combinations.
  - Easy creation of new widgets.
  - Mouse control (supports dragging)
  - Easy addition of new fonts and cursors.
  - Graphics system which uses clipping rectangles to draw overlapping widgets of any complexity, including support for scaled images.
  - Widget focusing system which is able to set a modal focus for widgets.
  - Utility module with C++ STL-like vectors, lists, queues and strings.

Default widgets:
  - Window (basic resizable and draggable window)
  - Button (generic image button)
  - Label (with word wrapping)
  - Container (could include any kind of other widgets, including other containers)
  - Scrolling box (provides scroll bars to display content not fit on screen)
  - List box (containing string values)
  - Checkbox
  - Radiobutton (grouped in containers)
  - Slider (horizontal and vertical)
  - Image (any image)

## Requirements
  - ZDoom-based engine (GZDoom and QZDoom are recommended)
  - GDCC compiler
  - ZIP/PK3 project structure

## Getting started.
libZDGUI is distributed as a set of *.c and *.h files, which are meant to be compiled statically by GDCC compiler alongside with your project. That means that you just put libZDGUI source and header files in /scripts folder of your project. libZDGUI also includes a set of default fonts, cursors and graphics for widgets, so you can use the library with default set, but of course it's recommended to use your mod graphics for them.
libZDGUI is ready to use - it contains an example project (libzdgui.c) file - test window and music player.

Below is libZDGUI project structure:
  - acs/ - contains compiled ACS code
  - graphics/ - contains default graphics for widgets
  - scripts/ - source code of library with example project file libzdgui.c
  - default fonts - files with .lmp extension in root folder.
  - FONTDEFS - background image "font"
  - LOADACS - libZDGUI ACS loader

To use the library, put LOADACS, fonts and contents of graphics/ and scripts/ folders into corresponding folders of your project (it should be decompressed for libZDGUI compilation). As for FONTDEFS, if you use custom fonts, include contents of FONTDEFS file into your FONTDEFS file.
To compile libZDGUI, use build.bat file to create a compiled ACS code. For first use, you should set a path to your GDCC compiler directory. Open build.bat in text editor and replace ".\..\gdcc" to your gdcc path.
After compilation, you should see "libzdgui" file in acs folder. Now you can pack your project into PK3 and run it.

## License
libZDGUI uses the BSD license. See COPYING file for more details.