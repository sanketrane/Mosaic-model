// Created by: Sanket Rane (March 2024)

#ifndef PROCEDURAL_FUNCTIONS
#define PROCEDURAL_FUNCTIONS

// include statements
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

// header files
#include "Agebin.h"
#include "accessory_functions.h"

// forward declarations

/**
 * Calculates the size of thy naive subset based on the given time.
 *
 * @param Time The time value used to calculate thyNtreg size.
 * @return The size of thymic naive treg subset.
 */
double thyNtregSize(double Time); // forward declaration

// function to sample N number of elements from a vector and return samples as a new vector
/**
 * @brief Samples N elements from a source vector and copies them to a destination vector.
 *
 * This function takes two vectors, `fromvec` and `tovec`, and samples `N_elements` number of elements from `fromvec`.
 * The sampled elements are then copied to `tovec`. The function modifies `tovec` and does not modify `fromvec`.
 *
 * @param fromvec The source vector from which elements will be sampled.
 * @param tovec The destination vector where the sampled elements will be copied.
 * @param N_elements The number of elements to sample from `fromvec`.
 *
 * @return tovec .
 */
std::vector<int> samplerNfromVector(std::vector<int> &fromvec, std::vector<int> &tovec, int N_elements); // forward declaration

// Aggregate functions

/**
 * Writes the results from evaluation of an Agebin object to a CSV file.
 * Agebins are evaluated at each time step using doAgebin function.
 *
 * @param agebin The Agebin object containing the results to be written.
 * @param time_now The current time.
 * @param resfile The output file stream to write the results to.
 */
void writeResultsToCSV(Agebin &agebin, double time_now, std::ofstream &resfile);

/**
 * Calculates the frequency of each clone ID in the given Subpop object and writes the results to a CSV file.
 *
 * @param subpop The Subpop object containing the clone IDs.
 */
std::map<int, int> getSubpopCloneFreq(Subpop &subpop); // forward declaration

/**
 * Writes clone frequencies within a subpop to a CSV file.
 * gets the cloneIDvec of the subpop and counts the frequency of each clone ID.
 * sequentially iterates through each subpop.
 *
 * This function takes an `Agebin` object, the current time, and two `ofstream` objects as parameters.
 * It writes the clone frequency to a CSV file specified by the `naifile` and `memfile` parameters.
 *
 * @param agebin The `Agebin` object containing the clone frequency data.
 * @param time_now The current time.
 * @param naifile The output file stream for the NAI (Normalized Age Index) data.
 * @param memfile The output file stream for the memory data.
 */

void writeCloneFreqToCSV(Agebin &agebin, double time_now, std::ofstream &cloneFreqfile);

/**
 * Calculates the difference between two vectors.
 *
 * This function takes two vectors, `vec1` and `vec2`, and returns a new vector
 * that contains the elements that are present in `vec1` but not in `vec2`.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return A new vector containing the difference between `vec1` and `vec2`.
 */
std::vector<int> differenceOfVectors(std::vector<int> &vec1, std::vector<int> &vec2);

// returns the vector of unique elemnts if the m_cloneID_vec
/* std::vector<int> Subpop::getuniqueClones()
{
    std::vector<int> v{m_cloneIDvec};
    std::sort(v.begin(), v.end()); // sort vector since unique erases continuous duplicates only
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return v;
} */

#endif // PROCEDURAL_FUNCTIONS
