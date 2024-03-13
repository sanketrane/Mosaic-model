# MoSAIC Model

## Description

MoSAIC (Modular Stochastic and Adaptive Iterating Classes) is a fine-grained population-structured probabilistic model that tracks cohorts/clusters of cells over time and across lineages.

## Setup and Execution

Follow these steps to setup and execute the MoSAIC model:

1. **Clone the repository**: You can do this either by direct download method or by using `git clone`.

2. **Navigate to the directory**: Use the `cd` command in Terminal. For example, `cd path/to/your/directory`.

3. **Compile the program**: Use the `make` command to compile the `mosaic_program`. This command calls the Makefile, which assumes that your source files (c++ files) are in the current directory and that you want to output the executable to the same directory. It also assumes that you're using the g++ compiler.

   Note: `make` only re-compiles code that needs to be recompiled. It checks for modifications to source files and only re-compiles modified files. Every time changes are made to source C++ files, the program needs to be re-compiled by running `make` again before the updated version of the program can be executed.

4. **Run the program**: If there are no compilation errors, this will produce an executable file `mosaic_program`. Run the program by typing `./mosaic_program`.

## Authors

* **Sanket Rane** - *Initial work* - [sanketrane](https://github.com/sanketrane)