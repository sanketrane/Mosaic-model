#include <iostream>
#include <vector>
#include <random>
#include "Subpop.h"

// look for the function descriptions in the header file

// generatting samples from a univariate binomial distribution from size = nTrials and prob=prob
int randbinom(int nTrials, float prob)
{
    std::random_device rand_dev;        // non deterministic random number generator (uses hardware noise)
    std::mt19937 generator(rand_dev()); // seed the mersenne twister algorithm -- mersenne twister generates well spaced pseudoradnom numbers
    std::binomial_distribution<int> distr(nTrials, prob);
    return distr(generator);
}

// Teach operator<< how to print a CellType
// std::ostream is the type of std::cout
// The return type and parameter type are references (to prevent copies from being made)!
std::ostream &operator<<(std::ostream &out, Subpop::CellType x)
{
    switch (x)
    {
    case Subpop::CellType::nai_dis:
        return out << "Naive displaceable";
    case Subpop::CellType::nai_inc:
        return out << "Naive incumbent";
    case Subpop::CellType::mem_fast:
        return out << "Memory fast";
    case Subpop::CellType::mem_slow:
        return out << "Memory slow";
    default:
        return out << "?No subpop detected?";
    }
}