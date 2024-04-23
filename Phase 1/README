# Project Build Guide

This guide outlines the steps to compile and link the project manually, generate a JSON build configuration from a `.build` file, and then create a `build.ninja` file for use with the Ninja build system.

## Manual Compilation anßd Linking

1. Compile `hello.cc` to object file:
   ```
   g++ -c hello.cc -o hello.o
    ```
2. Create static library `hello.a`:
   ```
   ar rcs hello.a hello.o
   ```
3. Compile `greet.cc` to object file:
   ```
   g++ -c greet.cc -o greet.o
   ```
4. Link object files to create executable `greet`:
   ```
   g++ greet.o hello.a -o greet
   ```

    Alternatively, for direct linking with `ld` (advanced):
   ```
    ld -o greet greet.o hello.o -lc
   ```
5. Run executable `greet`:
   ```
   ./greet name
   ```

## Building with Make or Ninja (Manually)

1. Use Ninja to build the project as described in `build.ninja`:

   ```
   ninja
   ```

2. Use Make to build the project as described in `makefile`:

   ```
   make
   ```

## Generating JSON Build Configuration from `.build` File

Use a Python script to convert a custom `.build` file into a JSON format (`foo.json`), which describes the build configuration.

1. Run the Python script to generate `foo.json`:
   ```
   python3 1.py
   ```