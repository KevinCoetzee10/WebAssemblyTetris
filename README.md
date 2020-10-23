# Tetris using WebAssembly

![image-20201023231259757](.\screenshots\image-20201023231259757.png)

## Project Description

In this project, we will implement the popular Tetris game as a practical example of how to use WebAssembly language. Our game logic will be written in C++, and then compiled to a WebAssembly module, which will then be utilized by our client-side Javascript and HTML files in order to host our Tetris game in a web browser. Our HTML files will then be hosted by a simple http server. For this project, all setup steps assume you are developing within a Windows 10 environment.

## Environment Setup

### Python Prerequisite

On Windows 10, Python version 2.1.12 or newer is needed in order to run the Emscripten SDK from GitHub. 

### Emscripten Setup

As we will be writing our game logic in C++, we will need an SDK that is able to compile C++ code to WebAssembly, or corresponding .wasm files. There are various compilers that are capable of doing this. In this project, we will be using the popular Emscripten SDK.

1. Clone the Emscripten GitHub repository (https://github.com/emscripten-core/emsdk.git) into a folder on your local machine.
   This can be done with the GitHub Desktop application, or by the following command in terminal environments supporting Git commands: 
   `git clone https://github.com/emscripten-core/emsdk.git`
2. Open a Powershell terminal window as an Administrator.
3. Enable running unsigned scripts, as this is needed to run the Emscripten setup scripts.
   This can be done with the following command: 
   `set-executionpolicy remotesigned`
4. In the Powershell terminal, navigate into the folder containing the Emscripten GitHub repository on your local machine, and pull the latest version of the Emscripten SDK. 
   This can be done with the GitHub Desktop application, or by the following command in terminal environments supporting Git commands: `git pull`
5. Download and install the latest versions of the Emscripten SDK tools - this might take a while. 
   This can be done with the following command: 
   `emsdk install latest`
6. Activate the latest versions of the Emscripten SDK tools that you downloaded and installed in the previous step.
   This can be done with the following command: 
   `emsdk activate latest`
7. Set PATH and environment variables for Emscripten ease of use. This makes Emscripten more accessible from folders outside of the SDK repo itself.
   This can be done with the following command: 
   `emsdk_env.bat`

## Creating The Game

The written code will be divided between 4 files - **index.html**, **style.css**, **script.js** and **tetris.cpp**.

### index.html

The HTML file contains the grid that we will use in order to display our Tetris GUI, as well as the buttons that we will use to take input for moving left and right, and rotating the active block.

![image-20201023221227192](.\screenshots\image-20201023221227192.png)

![image-20201023221403925](.\screenshots\image-20201023221403925.png)

We also include our CSS styling, as well as both our own JavaScript code and the JavaScript glue code produced by Emscripten in our HTML file. 

![image-20201023225020892](.\screenshots\image-20201023225020892.png)

### style.css

The CSS file contains some basic styling code, such as widths, heights, padding and background colours. 
More importantly, it contains the column and row setup for our Tetris grid. 
We also create classes for three different colours, to differentiate between different types of Tetris blocks. 

![image-20201023221820528](.\screenshots\image-20201023221820528.png)

![image-20201023221921495](.\screenshots\image-20201023221921495.png)

### script.js

This JavaScript file initializes a set of variables and manipulates our HTML Tetris grid. It also acts as a middleman between functions called by our HTML input buttons and our C++ code (compiled to WASM) that handles our game logic.

Some basic grid manipulation includes clearing our Tetris grid and converting the integer values for each cell received from WASM into a corresponding colour and class present in our CSS file. The JavaScript file also contains our timing interval, which determines the pace at which the game progresses. 

![image-20201023222734586](.\screenshots\image-20201023222734586.png)

Our C++ code (compiled to WASM) returns vectors when multiple values need to be retrieved. The JavaScript file therefore also has some utility functionality for converting these vectors to arrays more easily interpreted as our Tetris grid.

![image-20201023222634320](.\screenshots\image-20201023222634320.png)

### tetris.cpp

Our CPP file contains our Tetris game logic, handling the more computationally expensive parts of our game. This functionality is then exposed to our JavaScript code, in order to represent this within our Tetris grid. In order to make our data type conversions from C++ to WASM possible, so our JavaScript code can interpret it, we include some Emscripten header files.

![image-20201023224251785](.\screenshots\image-20201023224251785.png)

In order to make index calculation and rotations less complex, our Tetris blocks are stored as single-dimensional arrays each representing 4x4 cell squares, with integers indicating the colour represented by each cell. We also have some utility functions for cleaner index to coordinate conversions, and vice versa. These are used extensively when doing block rotations.

The CPP file contains a **tetrisStart** function, that initializes our persistent variables and generates our first random Tetris block onto a C++ array representing the entire game grid. Here, we also have a utility function in order to convert these arrays to vectors for communication through our WASM file. 

![image-20201023223517225](.\screenshots\image-20201023223517225.png)

Our CPP file also contains the functionality for moving these generated blocks left and right, and rotating these blocks, as well as performing checks ahead-of-time to determine whether these movements would be legal or not. We also have functions that return the gameOver state of the game, which the Javascript calls within each time interval it completes, and a utility function for removing completed rows and moving down all above blocks. These functions are quite lengthy and can all be found within the referenced Git Repository. Below is a sample of the moveDown function, demonstrating the basic flow of our movement functions.

![image-20201023224359084](.\screenshots\image-20201023224359084.png)

Finally, in order to make these functions available for use by our JavaScript code, we need to bind them to JavaScript functions through Emscripten. As we return integer vectors in many of our functions, it is also necessary to explicitly register an integer vector conversion.

![image-20201023224624009](.\screenshots\image-20201023224624009.png)

## Building The Project

The building process involves compiling our tetris.cpp file into a corresponding tetris.wasm module, as well as a tetris.js file containing our glue code making the WASM module usage possible within JavaScript. We will use the Emscripten SDK (setup earlier in this tutorial) to do these conversions.

The following commands can be used to compile our tetris.cpp file (note that this command needs to be run from the context of the emsdk repo downloaded earlier, and the paths need to be relative from this folder).

`cd emsdk/upstream/emscripten``
./emcc --bind -o /path/from/this/folder/to/tetris/repo/tetris.js /path/from/this/folder/to/tetris/repo/tetris.cpp`

Note also that we specify a **tetris.js** file in this command. This tells EMCC to produce the glue code we require within the folder we specify. When creating your own projects, any errors at this step will probably be a result of compilation errors within your CPP file.

## Running The Project

After **tetris.js** and **tetris.wasm** have been produced by EMCC, running our project is as simple as serving the **index.html** file on our local machine. This can be done in many ways, such as downloading a Live Server plugin within your IDE, however, below is a set of commands that uses **NPM** to download a Node.js server library, which can then easily be run.

Navigate to the project's git repository and execute the following commands:
`npm install -g http-server`
`http-server`

The URLs where your index.html will be served are then displayed in your terminal.

## Source Code

All source code can be found at https://github.com/KevinCoetzee10/WebAssemblyTetris