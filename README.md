## MoSAIC model

### Modular Stochastic and Adaptive Iterating classes

A fine-grained population-structured probabilistic model that tracks cohorts/clusters of cells over time and across lineages

## Setup and Execution:
0. Clone the directory either by direct download method or by using git clone
1. Navigate to the directory in `Terminal`                  ## using `cd` command >> `cd path/to/your/directory`

2. Compile the mosaic_program using the command `make`      ## make calls the Makefile, which assumes that your source files (c++ files here) are in the current directory
                                                            ## and that you want to output the executable to the same directory. 
                                                            ## åIt also assumes that you're using the g++ compiler.

Note:
`make` only re-compiles code that needs to be recompiled.      ## checks for modifications to source files and only re-compiles modified files
 **Every time changes are made to source C++ files, program needs to be re-compiled by running `make` again before updated version of the program can be exceuted.**

3. If there are no compilation errors, this will produce an executable file mosaic_program
    run the program by typing `./mosaic_program`


## Author/s

* **Sanket Rane** - *Initial work* - [sanketrane](https://github.com/sanketrane)
