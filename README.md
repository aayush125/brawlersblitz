# SDL2 Project

This is a C++ project that uses the SDL2 (Simple DirectMedia Layer) library for game development.

## Dependencies

To build and run this project, you will need the following dependencies:

- SDL2 v2.0.16
- SDL2_image v2.0.5
- SDL2_ttf v2.0.15
- g++ (with C++20 support)

## Installation

1. Download the SDL2, SDL2_image, and SDL2_ttf libraries from the official websites.
2. Extract the libraries to the following directories in your project:
   - `lib/SDL2`
   - `lib/SDL2_image`
   - `lib/SDL2_ttf`
3. Ensure the include directories are set up correctly by creating a symbolic link or copying the SDL2 include files to the `include/SDL2` directory.

## Building the Project

1. Navigate to the project's root directory.
2. Run `make` to compile the project.
3. The executable will be generated in the `bin/` directory.

## Directory Structure

The project has the following directory structure:
├── include/
│   └── headers/
│   └── SDL2/
├── lib/
│   ├── SDL2/
│   ├── SDL2_image/
│   └── SDL2_ttf/
├── src/
├── obj/
├── bin/
└── makefile

- `include/`: Contains the header files for the project.
- `include/SDL2`: Contains SDL2 header files.
- `lib/`: Contains the SDL2, SDL2_image, and SDL2_ttf library files.
- `src/`: Contains the source code files for the project.
- `obj/`: Contains the compiled object files.
- `bin/`: Contains the compiled executable.
- `makefile`: The build configuration file.

## License

This project is licensed under the [MIT License](LICENSE).