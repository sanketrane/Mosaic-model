#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <cmath>

// header files
#include "header_files/Agebin.h"
#include "header_files/Subpop.h"
#include "header_files/accessory_functions.h"
#include "header_files/procedural_functions.h"
#include "header_files/doSim_functions.h"

int main()
{
    [[maybe_unused]] int cloneUniverse{100000}; // total number of clone IDs to draw data from 1 million
    double tFin{450.0};
    double t0{5.0};
    double tStep{0.1};
    [[maybe_unused]] int NumAgebins_t0 = static_cast<int>(t0 / tStep);

    // define input params -- based on model fits
    double kilossrate{0.23};   // rate of loss of Ki67 on T cells
    double thyinfluxrate{0.4}; // rate of influx of naive Tregs from thymus to peripheral naive Treg pool

    // array of parameters
    std::vector<double> par_vec{tStep, kilossrate};

    // vector of Agebin objects -- increases in size by 1 at each timestep
    std::vector<Agebin> A; //

    // initialize the agebin objects at t0 -- 50 agebins since tStep = 0.1 (Number of agebins = t/tStep)
    for (int i = 0; i < 50; ++i)
    {
        // initialize the agebins
        // initial number of cells in each subset are based on data

        // we start with 1000 * 50 = 50,000 cells in the naive dis subset -- this will be fed from naive thy subset at every tStep
        std::vector<int> b0v{randVector(1000, 0, cloneUniverse)};

        // we start with 1000 * 50 = 50,000 cells in the naive Inc subset -- No influx after t0 in this subset -- stays same in size
        std::vector<int> b1v{randVector(1000, 0, cloneUniverse)};

        // we start with 200 * 50 = 10,000 cells in the memory subset -- split 90:10 between fast:slow
        std::vector<int> b2v{randVector(150, 0, cloneUniverse)}; //-- this will be fed from naive dis + naive inc subsets at every tStep
        std::vector<int> b3v{randVector(50, 0, cloneUniverse)};  //-- this will be fed from mem_fast inc subsets at every tStep

        // Note: memory slow subset at t0 -- 0 cells -- value initialized to 0 in the constructor

        // initial number of Ki67 positive cells in each subset are based on intuition
        int b0k{400 + rand() % (700 - 400 + 1)}; // random number between 400 and 700
        int b1k{400 + rand() % (700 - 400 + 1)};
        int b2k{90 + rand() % (120 - 90 + 1)}; // random number between 120 and 180
        int b3k{30 + rand() % (45 - 30 + 1)};  // random number between 120 and 180

        A.push_back(Agebin(50 - i, b0v, b0k, b1v, b1k, b2v, b2k, b3v, b3k));
    }

    // Open (or create) a CSV file in write mode
    std::ofstream countresfile("output_files/CountsResults.csv");
    countresfile << "Time, Agebin, Total_nai_dis, Total_nai_inc, Total_mem_fast, Total_mem_slow \n";
    std::ofstream kiresfile("output_files/KiResults.csv");
    kiresfile << "Time, Agebin, Kifrac_nai_dis, Kifrac_nai_inc, Kifrac_mem_fast, Kifrac_mem_slow \n";

    std::ofstream nai_disfile("output_files/nai_disFreq.csv");
    nai_disfile << "Time, Agebin, Clone ID, Frequency \n";
    std::ofstream nai_incfile("output_files/nai_incFreq.csv");
    nai_incfile << "Time, Agebin, Clone ID, Frequency \n";
    std::ofstream mem_fastfile("output_files/mem_fastFreq.csv");
    mem_fastfile << "Time, Agebin, Clone ID, Frequency \n";
    std::ofstream mem_slowfile("output_files/mem_slowFreq.csv");
    mem_slowfile << "Time, Agebin, Clone ID, Frequency \n";

    for (std::vector<Agebin>::size_type i = 0; i < A.size(); i++) // for (auto i = 0u; i < A.size(); i++) this works as well
    {
        // intial conditions to CSV
        writeResultsToCSV(A.at(i), 5, countresfile, kiresfile);                                // counts and Ki67 fractions
        writeCloneFreqToCSV(A.at(i), 5, nai_disfile, nai_incfile, mem_fastfile, mem_slowfile); // clone frequencies
    }

    // step counter intiatied at 50 since we have already initialized 50 agebins
    int x = 50;

    // main loop -- iterating over time
    for (double currentTime = t0 + tStep; currentTime < tFin; currentTime += tStep)
    {

        double thynaiInflux = thyinfluxrate * thyNtregSize(currentTime) * tStep; // in
        std::vector<int> b0vNu{randVector(thynaiInflux, 0, 500)};
        // std::cout << "Thymic naive influx: " << thynaiInflux << " B0 clonevec size: " << b0vNu.size() << '\n';
        double fraction = 0.3 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (0.5 - 0.3)));
        int b0kNu = thynaiInflux * fraction;

        x++; // step counter
        if (x % 100 == 0)
        {
            std::cout << "Time: " << currentTime << " Step: " << x << '\n';
        }

        // second loop -- iterating over agebins
        for (auto i = 0u; i < A.size(); i++)
        {
            // update the agebins at every step
            doAgebin(A.at(i), par_vec);

            if (x % 70 == 0) // number of steps 70 == 7 days, since each step is 0.1 days
            {
                // write the results to a CSV file every 7 days
                writeResultsToCSV(A.at(i), currentTime, countresfile, kiresfile);
            }

            if (x % 300 == 0) // number of steps 300 == 30 days, since each step is 0.1 days
            {
                // write the clone frequencies to a CSV file every 30 days
                writeCloneFreqToCSV(A.at(i), currentTime, nai_disfile, nai_incfile, mem_fastfile, mem_slowfile);
            }
        }
        // add a new agebin of cells of age 0
        A.push_back(Agebin(0, b0vNu, b0kNu));
    }

    nai_disfile.close();
    nai_incfile.close();
    mem_fastfile.close();
    mem_slowfile.close();
    countresfile.close();
    kiresfile.close();

    std::cout << "Simulation complete\n";

    return 0;
}