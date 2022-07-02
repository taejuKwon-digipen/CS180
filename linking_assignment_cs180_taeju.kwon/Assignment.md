# Linking Assignment

For this assignment you will practice your knowledge of linking by help creating a simple ASCII game with plugins.

## Introduction

In this game, you walk around a "dungeon" and try yo collect coins and torches. Torches allow you to see within a certain radius. When your torch supply is exhausted the player cannot see anything and the game is over.

The map of the dungeon is generated from plugins. The user will be able to select which plugin to use and will play the maps it provides. On example plugin which generates maps randomly has been provided.

The game starts up looking for all plugins and loads them. It uses the first one that it finds. There is a special input event that allows the user to cycle through different  plugins. Specifically, if you press the `n` key then it will use the next plugin to display the game.

See the provided samples to run and try out the game on both Windows and Ubuntu. Use `w`, `s`, `a` and `d` to walk around the dungeon.

In order to draw the ASCII graphics that work for both Windows and POSIX operating systems the project utilizes the  [curses library](https://en.wikipedia.org/wiki/Curses_%28programming_library%29). There are multiple implementations of this library, so for the Window OS [pdcurses](https://github.com/wmcbrine/PDCurses) will be used and for POSIX platforms [ncurses](https://invisible-island.net/ncurses/ncurses.html) will be used.

## Project Setup and Dependencies

### Curses

To have the curses library be available on Ubuntu, install it using the `apt` command like so
```txt
sudo apt install libncurses-dev
```

For Windows, it is easiest if you install `pdcurses` using [vcpkg](https://github.com/microsoft/vcpkg).

Once you have `vcpkg` setup you can run

```txt
vcpkg integrate install
vcpkg install pdcurses:x64-windows
```

The "integrate install" will expose all libraries installed with vcpkg to Visual Studio. The next command installs the curses library.

### CMake to build the project

We are using [CMake](https://cmake.org) to define the project and build it for either Windows or Ubuntu. 


To build the project on the command line **`cd`** into the root of the project and invoke the following commands:
```sh
# create a folder called build and generate build files from this source directory
# Tell cmake to generate Release version of the project. This is needed when making makefiles
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
# create the assignment exe using the build files
# --config Release isn't needed if we have makefiles but if we have XCode/Visual Studio then 
#   we need this flag to get a Release exe
cmake --build build --config Release
```

You should then be able to find the executable `FindFire` in `/build/Release/`


## Code Organization

This project produces 4 target files.

1. core - This is a dynamic library that contains all the common code shared between the game and the plugins. The main program and all plugins will link with this library automatically when the program starts.
2. main - Executable program for the game. 
3. plugins/random_maps - This a dynamic library that implements the map generator plugin. This will be linked into the game at runtime programmatically
4. plugins/student three maps - You will need to create your own plugin project. You should name it after your own name and your plugin will provided three maps. This too will be linked into the game at runtime programmatically

Since the `core` code needs to be shared between the main executable and many plugin dlls, we don't want to duplicate it's code instructions in all of them. That is why we are creating it as a dynamic library rather than a static library.

The `core` library contains the gameplay logic and most of the plugin interface, which needs to be available for both the main exe and plugins.

The `main` exe contains everything else that doesn't need to be shared.

### Plugin Code interface

#### plugin_visibility.h

This is a helper header file that defines some much needed macros.

It defines `CORE_API` and `PLUGIN_API`. Both are used to appropriately add the dll export/import attributes for the Windows OS and visibility attribute for Posix.

When we are creating the `core` library then common things need to be **exported**. When we are using the `core` library then we want to **import** the functions. This is what `CORE_API` provides.

When we are creating a plugin library then it's functions need to be **exported** and when the plugin is being using by others then it's functions should be **imported**. This is what `PLUGIN_API` provides.

Since we will be loading the plugin libraries programmatically, we are going to simplify the symbol names for functions using `extern "C"` and avoid C++ name mangling. We use the `CORE_FUNC` and `PLUGIN_FUNC` attribute macros to help us with that.

They are defined like so:

```c
#define CORE_FUNC   extern "C" CORE_API
#define PLUGIN_FUNC extern "C" PLUGIN_API
```

#### plugin_api.h

The plugin api header defines what plugin libraries need to create in order to work with our game. It is defined like:

```c++
#include "plugin_visibility.h"

class PluginFactory;

// This will be called by the game after loading the plugin
// This is where it will register the game features it 
// customizes via the PluginFactory interface
PLUGIN_FUNC bool PluginInit(PluginFactory& plugin_factory);

// Used to get a unique string to identify the plugin
PLUGIN_FUNC const char* PluginDisplayName();
```

If the plugin is missing any of these functions then an exception will be thrown and the game will exit with error message.

#### PluginFactory

A game could have any number of features be exposed and customized. This game currently only supports one and that is the map generator interface.

This PluginFactory is used by Plugins to tell the game what features they will customize. To customize the map generator interface it has the following definitions.

```c++
class PluginFactory
{
public:
    // pointer to function that takes in nothing and returns an
    //   IMapGenerator implementation
    using CreateMapGeneratorInstance  = gameplay::IMapGenerator* (*)();
    // pointer to function that takes in a pointer to an IMapGenerator
    //   returns nothing
    using ReleaseMapGeneratorInstance = void (*)(gameplay::IMapGenerator*); 

    // This function is used to add a creation/destruction pair of functions 
    //  from the plugin to the game
    //  Should be called in the Plugin's Init function
    CORE_API void RegisterMapGeneratorPlugin(std::string unique_name, 
                            CreateMapGeneratorInstance create_map_generator,
                            ReleaseMapGeneratorInstance release_map_generator);

    // We will use unique_ptr to auto delete the IMapGenerator with the 
    // release function provided by the plugin
    using IMapGenertorPtr = std::unique_ptr<gameplay::IMapGenerator, ReleaseMapGeneratorInstance>;
    // This is used by the game to actually invoke the creation function to use the 
    //  plugin's implementation of IMapGenerator
    CORE_API IMapGenertorPtr CreateMapGenerator(const std::string& unique_name) const;

private:
    struct AllocPair
    {
        CreateMapGeneratorInstance  create{nullptr};
        ReleaseMapGeneratorInstance release{nullptr};
    };
    std::unordered_map<std::string, AllocPair> mapGenMakers{};
};
```


#### PluginInstance

This class is in charge of loading a plugin by using the Operating system calls. You will need to create an implementation of this class for Windows and Posix.

```c++
class PluginInstance
{
public:
    // Constructor saves the plugin file and does not yet load the dynamic library
    explicit PluginInstance(std::filesystem::path dll_file_path);
    // Loads the dynamic library and gets all of the functions defined in plugin_api.h
    // Invokes the PluginInit function
    // Throws an std::runtime_error with a helpful error message if something goes wrong
    // or if it fails to find a function it was looking for
    void                                Load(PluginFactory& plugin_factory) const;
    // Unloads the dynamic library - removed it from the process virtual memory space
    void                                Unload() const noexcept;
    [[nodiscard]] bool                  IsLoaded() const noexcept;
    [[nodiscard]] std::filesystem::path GetFilePath() const noexcept;
    [[nodiscard]] std::string           GetDisplayName() const noexcept;

private:
    // We are using the PIMPL pattern to hide away the OS specific types
    class Impl;
    std::shared_ptr<Impl> impl;
};
```

## Task 1 - Implement PluginInstance

You must implement a Windows and Posix version of the `PlugInstance` class. The Windows version will be written in `PluginInstance_win32.cpp` and Posix version will be done in `PluginInstance_posix.cpp`.

After completing this task you should be able to reproduce the provided samples FindFire exe.

### Windows API System Calls needed

System Call | Description
-|-
[`LoadLibrary()`](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya)|This will load a DLL file and make it usable in the process
[`FreeLibrary()`](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary)| This will remove the DLL from the process
[`GetProcAddress()`](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress)| This will help you find a symbol inside of a loaded DLL
[`GetLastError()`](https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror)| This is used to get an error code when an api function has failed. Note that a `GetLastWin32ErrorAsString()` helper function has been provided to turn that error code into a human readable message

### Posix API System Calls needed

System Call | Description
-|-
[`dlopen()`](https://man7.org/linux/man-pages/man3/dlopen.3.html) | This will load a so file and make it usable in the process
[`dlclose()`](https://man7.org/linux/man-pages/man3/dlopen.3.html) | his will remove the so from the process
[`dlsym()`](https://man7.org/linux/man-pages/man3/dlsym.3.html)| This will help you find a symbol inside of a loaded so
[`dlerror()`](https://man7.org/linux/man-pages/man3/dlerror.3.html)| Use this to get an error string when something goes wrong


## Task 2 - Create another map generating plugin

Create one new plugin project that implements the map generation interface.

To keep things simple, have it cycle through 3 hard coded maps. Once a player leaves map 3 then they should be brought to back to map 1.

1. Map 1 - Should use the Wall tiles to spell your name
2. Map 2 - Should use the Wall tiles to draw a smiley face
3. Map 3 - Should use the Wall tiles to draw a simple animal of your choosing

Obviously each map must have one or more exits, torches and coins.

Create a `plugins/NAMEs_three_maps` folder, where NAME is your name Put all of your plugin source code there and create a `CMakeLists.txt` file so we can build your plugin.

Your CMakeLists.txt should look something like : 
```cmake
# Adding a MODULE library will allow us to make a shared library that can be loaded programmatically
# Instead of Ben, you should use your own name
add_library(bens_maps MODULE bens_maps.cpp bens_maps.h)

# Add this line to make sure you are getting all the compiler warning checks
target_link_libraries(bens_maps PRIVATE core project_warnings)

# Add these lines so that we can auto copy our plugin to the plugins folder
add_custom_command(TARGET bens_maps POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bens_maps> ${CMAKE_BINARY_DIR}/dist/$<CONFIG>/plugins/
)
```

Once you are ready to start building your plugin, make sure to update the `plugins/CMakeLists.txt` file so that it adds your new project directory.

Should look something like : 

```cmake
add_subdirectory(random_maps)
add_subdirectory(bens_maps)
```

## Create a ReadMe

Create a **`ReadMe.md`** markdown file that defines the following.

1. Your name, assignment name/number, course name, term
2. Section describing anything incomplete about the assignment
3. Section describing something you're proud of about the assignment

# Submission

Submit a 7z archive of your assignment on the course site. 

Make sure no auto generated files are inside. We should be able to extract your archive then build and run the game using CMake.

## Grading Rubric

- [ ] [**core**] Implemented the `PluginInstance` class for Windows in `PluginInstance_win32.cpp` and for Posix in `PluginInstance_posix.cpp`
- [ ] [**core**] Created a new plugin that cycles through three hardcoded maps as described above
- [ ] `PluginInstance` hides the OS specific types & functions by using the PIMPL pattern.
- [ ] Players can cycle through the different map generation versions.
- [ ] Create a properly filled out `ReadMe.md` file.
- [ ] All source files have a proper header comment with your name, assignment name/number, course name, term.
- [ ] All source files compile without warnings and without errors on both platforms
- [ ] Correct files submitted. No unnecessary files submitted.
- [ ] Followed the appropriate Code Quality Guidelines and Code Style Guidelines.

Scores for Assignments will be given as the following:

Score        | Assessment
------------ | ----------
Zero         | Nothing turned in at all
Failing      | Attempted something
Rudimentary  | Close to meeting core requirements
Satisfactory | Meets all of the core requirements
Good         | Clearly meets all requirements 
Excellent    | High quality, well beyond the requirements

### Ideas to get an Excellent score

Add another customizable feature to the game via plugins. Like a save file system. Or maybe hooks to do something when game events happen, like playing sounds when a player collides with a wall, collects a coin, an achievement system, etc...

Create a new plugin type to customize how the game looks. Perhaps have a version that uses sprites to represent the game visuals.
