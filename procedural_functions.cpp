#include "procedural_functions.h"

// Declarations and descriptions in the header file
double thyNtregSize(double Time)
{
    // returns the size of the Thy nTreg subset varying with time
    double basl{9};
    double theta{250};
    double n{2};
    double X{32};
    double q{4};

    return (exp(basl) + (theta * pow(Time, n)) * (1 - (pow(Time, q) / (pow(X, q) + pow(Time, q)))));
}

std::vector<int> samplerNfromVector(std::vector<int> &fromvec, std::vector<int> &tovec, int N_elements)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::sample(fromvec.begin(), fromvec.end(), std::back_inserter(tovec), N_elements, g);

    return tovec;
}

// Aggregate functions
void writeResultsToCSV(Agebin &agebin, double time_now, std::ofstream &countsresfile, std::ofstream &kiresfile)
{
    if (countsresfile.is_open())
    {
        countsresfile << time_now << ", " << agebin.getAgebinID()
                      << ", " << agebin.getB0().getNumCells()
                      << ", " << agebin.getB1().getNumCells()
                      << ", " << agebin.getB2().getNumCells()
                      << ", " << agebin.getB3().getNumCells() << '\n';
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }

    if (kiresfile.is_open())
    {
        kiresfile << time_now << ", " << agebin.getAgebinID()
                  << ", " << agebin.getB0().getKifrac()
                  << ", " << agebin.getB1().getKifrac()
                  << ", " << agebin.getB2().getKifrac()
                  << ", " << agebin.getB3().getKifrac() << '\n';
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }
}

/**
 * Calculates the frequency of each clone ID in the given Subpop object and writes the results to a CSV file.
 *
 * @param subpop The Subpop object containing the clone IDs.
 */
std::unordered_map<int, int> getSubpopCloneFreq(Subpop &subpop)
{
    std::vector<int> cloneIDvec{subpop.getCloneIDvec()};

    // Create an unordered_map to count frequencies
    std::unordered_map<int, int> freq;

    // Count the frequencies
    for (int num : cloneIDvec)
    {
        freq[num]++;
    }

    return freq;
}

void writeCloneFreqToCSV(Agebin &agebin, double time_now, std::ofstream &nai_disfile, std::ofstream &nai_incfile, std::ofstream &mem_fastfile, std::ofstream &mem_slowfile)
{
    std::unordered_map<int, int> nai_disFreq{getSubpopCloneFreq(agebin.getB0())};
    std::unordered_map<int, int> nai_incFreq{getSubpopCloneFreq(agebin.getB1())};
    std::unordered_map<int, int> mem_fastFreq{getSubpopCloneFreq(agebin.getB2())};
    std::unordered_map<int, int> mem_slowFreq{getSubpopCloneFreq(agebin.getB3())};

    if (nai_disfile.is_open())
    {
        for (auto &pair : nai_disFreq)
        {
            nai_disfile << time_now << ", " << agebin.getAgebinID() << ", " << pair.first << ", " << pair.second << '\n';
        }
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }

    if (nai_incfile.is_open())
    {
        for (auto &pair : nai_incFreq)
        {
            nai_incfile << time_now << ", " << agebin.getAgebinID() << ", " << pair.first << ", " << pair.second << '\n';
        }
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }

    if (mem_fastfile.is_open())
    {
        for (auto &pair : mem_fastFreq)
        {
            mem_fastfile << time_now << ", " << agebin.getAgebinID() << ", " << pair.first << ", " << pair.second << '\n';
        }
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }

    if (mem_slowfile.is_open())
    {
        for (auto &pair : mem_slowFreq)
        {
            mem_slowfile << time_now << ", " << agebin.getAgebinID() << ", " << pair.first << ", " << pair.second << '\n';
        }
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }
}

std::vector<int> differenceOfVectors(std::vector<int> &vec1, std::vector<int> &vec2)
{
    // Sort the vectors
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    // Vector to hold the difference
    std::vector<int> diff;

    // Find the difference
    std::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(diff));

    return diff;
}

// returns the vector of unique elemnts if the m_cloneID_vec
/* std::vector<int> Subpop::getuniqueClones()
{
    std::vector<int> v{m_cloneIDvec};
    std::sort(v.begin(), v.end()); // sort vector since unique erases continuous duplicates only
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return v;
} */