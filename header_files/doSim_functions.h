// Created by: Sanket Rane (March 2024)

#ifndef DOSIM_FUNCTIONS_H
#define DOSIM_FUNCTIONS_H

// include statements
#include <vector>

// header files
#include "Agebin.h"
#include "Subpop.h"
#include "procedural_functions.h"
#include "accessory_functions.h"

// forward declarations

/**
 * Probabilistically evaluates the state of the subpopulation.
 *
 * This function performs a sequential probabilstic evaluation on the cells contained in this subpopulation.
 *
 * @param subpop The subpopulation on which the evaluation is performed.
 * @param par_vec The vector of parameters used in the evaluation.
 * @param precursor1 (Optional) The first precursor subpopulation.
 * @param precursor2 (Optional) The second precursor subpopulation.
 */
void doSubpop(Subpop &subpop, std::vector<double> &par_vec);

/**
 * Performs the probabilistic math operations on the given Agebin.
 *
 * sequential evalaution of the subpopulations -- B0 -> B1 -> B2 -> B3
 *
 * @param A The Agebin object to perform the operation on.
 * @param par_vec The vector of parameters used in the operation.
 */
void doAgebin(Agebin &A, std::vector<double> &par_vec);

#endif // DOSIM_FUNCTIONS_H