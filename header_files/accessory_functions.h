// Created by: Sanket Rane (March 2024)

#ifndef ACESSORY_FUNCTIONS
#define ACESSORY_FUNCTIONS

// This file contains the declarations of the accessory functions used in the main program.

// include statements
#include <iostream>
#include <vector>
#include <random>

// header files
#include "Subpop.h"

// forward declarations of the functions

/**
 * Generates a random number of successes in a given number of trials, following a binomial distribution.
 *
 * @param nTrials The number of trials.
 * @param prob The probability of success in each trial.
 * @return The number of successes.
 */
int randbinom(int nTrials, float prob);

/**
 * template function to generate a random vector of specified length with elements ranging from 'start' to 'end'.
 *
 * @param length The length of the vector to be generated.
 * @param start The starting value for the elements of the vector.
 * @param end The ending value for the elements of the vector.
 * @return A vector of random elements within the specified range.
 */
template <typename T>
std::vector<T> randVector(int length, T start, T end) // define a template function
{
    // Create a vector of integers
    std::vector<T> v(length);

    // Generate a random number generator
    std::random_device rand_dev; // non deterministic random number generator (uses hardware noise)
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<> dist(start, end);

    // Fill the vector with random numbers
    for (auto i = 0u; i < v.size(); i++)
    {
        v[i] = dist(gen);
    }

    return v;
}

/**
 * Prints the elements of a vector.
 *
 * This function takes a vector of type `T` and prints its elements to the standard output.
 * The elements are separated by the specified separator string.
 *
 * @param vector The vector to be printed.
 * @param sep The separator string to be used between elements. Default is a single space.
 */

template <typename T>
void print_vector(const std::vector<T> &vector, std::string sep = " ") // define a template function
{
    // Iterating vector by using index
    for (auto i = 0; i < vector.size(); i++)
    {
        // Printing the element at
        // index 'i' of vector
        std::cout << vector[i] << sep;
    }
    std::cout << '\n';
}

/**
 * @brief Teach operator << how to print a CellType
 * std::ostream is the type of std::cout
 *
 * @param out desired print output for the cell type variable
 * @param x cell type variable
 * @return std::ostream&
 *
 * The return type and parameter type are references (to prevent copies from being made)!
 */
std::ostream &operator<<(std::ostream &out, Subpop::CellType x);

#endif // ACESSORY_FUNCTIONS