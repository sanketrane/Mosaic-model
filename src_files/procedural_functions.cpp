#include "header_files/procedural_functions.h"

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
void writeResultsToCSV(Agebin &agebin, double time_now, std::ofstream &resfile)
{
    if (resfile.is_open())
    {
        resfile << time_now << ", " << agebin.getAgebinID()
                << ", " << agebin.getB0().getNumCells()
                << ", " << agebin.getB1().getNumCells()
                << ", " << agebin.getB2().getNumCells()
                << ", " << agebin.getB3().getNumCells()
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
 * Calculates the frequency of each clone ID in the given Subpop object
 *
 * @param subpop The Subpop object containing the clone IDs.
 */
std::map<int, int> getSubpopCloneFreq(Subpop &subpop)
{
    std::vector<int> cloneIDvec{subpop.getCloneIDvec()};

    // Create an unordered_map to count frequencies
    std::map<int, int> freq;

    // Count the frequencies
    for (int num : cloneIDvec)
    {
        freq[num]++;
    }

    return freq;
}

void writeCloneFreqToCSV(Agebin &agebin, double time_now, std::ofstream &cloneFreqfile)
{

    //std::map<int, int> nai_disFreq, nai_incFreq, mem_fastFreq, mem_slowFreq;
    std::map<int, int> nai_disFreq{getSubpopCloneFreq(agebin.getB0())};
    std::map<int, int> nai_incFreq{getSubpopCloneFreq(agebin.getB1())};
    std::map<int, int> mem_fastFreq{getSubpopCloneFreq(agebin.getB2())};
    std::map<int, int> mem_slowFreq{getSubpopCloneFreq(agebin.getB3())};

    // create a joined table
    std::map<int, std::tuple<int, int, int, int>> joined_table;

    // add elements from nai_disFreq to the joined table
    for (const auto &row : nai_disFreq)
    {
        std::get<0>(joined_table[row.first]) = row.second;
    }

    // add elements from nai_incFreq to the joined table
    for (const auto &row : nai_incFreq)
    {
        std::get<1>(joined_table[row.first]) = row.second;
    }

    // add elements from mem_fastFreq to the joined table
    for (const auto &row : mem_fastFreq)
    {
        std::get<2>(joined_table[row.first]) = row.second;
    }

    // add elements from mem_slowFreq to the joined table
    for (const auto &row : mem_slowFreq)
    {
        std::get<3>(joined_table[row.first]) = row.second;
    }

    // sort the joined table by key
    std::map<int, std::tuple<int, int, int, int>> sorted_joined_table(joined_table.begin(), joined_table.end());

    // write the joined table to a CSV file
    if (cloneFreqfile.is_open())
    {
        for (const auto &row : sorted_joined_table)
        {
            cloneFreqfile << time_now << ", " << agebin.getAgebinID() << ", " << row.first << ", " << std::get<0>(row.second) << ", " << std::get<1>(row.second) << ", " << std::get<2>(row.second) << ", " << std::get<3>(row.second) << '\n';
        }
    }
    else
    {
        std::cerr << "Error: File not open\n";
    }
    return;
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