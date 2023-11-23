# outOfSpaceShaders
A

<p align="center">
  <br>
  <img src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExemNvc3l6YjNoZnJvMzIzaTM4dGJpcnkwcGx2bTJpNHZ2MjB4ajNiOSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/YoSK8vQNBqgrA6kbmM/giphy.gif" alt="pic" width="500">
  <br>
</p>

<p align="center">
  <a href="#Files">Files</a> •
  <a href="#Features">Features</a> •
  <a href="#how-to-use">How To Use</a>
</p>

## Files

- **configure.sh**: A shell script to set up the project dependencies and environment.
- **build.sh**: A shell script to build the project using CMake.
- **run.sh**: A shell script to execute the compiled graphics application.
- **clean.sh**: A shell script to clean up build artifacts and generated files.
- **src**: A directory containing the source code files for the graphics application.
  - **main.cpp**: A

## Features

The main features of the graphics application include:

- A

## How To Use

To clone and run this application, you'll need [WSL (Windows Subsystem for Linux)](https://learn.microsoft.com/en-us/windows/wsl/install) and the following tools installed on it: [Git](https://git-scm.com), [C++ compiler](https://www.fdi.ucm.es/profesor/luis/fp/devtools/mingw.html), [CMake](https://cmake.org/download/), [Make](https://linuxhint.com/install-make-ubuntu/), [glew](https://zoomadmin.com/HowToInstall/UbuntuPackage/libglew-dev), [glm](https://sourceforge.net/projects/glm.mirror/), [tbb](https://www.intel.com/content/www/us/en/developer/tools/oneapi/onetbb.html), and [SDL2](https://www.oreilly.com/library/view/rust-programming-by/9781788390637/386c15eb-41b2-41b4-bd65-154a750a58d8.xhtml). From your command line:

```bash
# Clone this repository
$ git clone https://github.com/bl33h/outOfSpaceShaders

# Open the project
$ cd outOfSpaceShaders

# Give execution permissions
$ chmod +x configure.sh
$ chmod +x build.sh
$ chmod +x run.sh

# Run the app
$ ./run.sh
