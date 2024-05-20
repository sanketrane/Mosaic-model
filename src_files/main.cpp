#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <thread>
#include <cstdlib> 
#include <string>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/info.h>
#include <tbb/parallel_for.h>
#include <tbb/task_arena.h>
#include <tbb/global_control.h>

// header files
#include "header_files/Agebin.h"
#include "header_files/Subpop.h"
#include "header_files/accessory_functions.h"
#include "header_files/procedural_functions.h"
#include "header_files/doSim_functions.h"

int main(int argc, char* argv[])
{   
    int cloneUniverse{100}; // total number of clone IDs to draw data from 1 million
    double tFin = std::atof(argv[1]);
    double t0 = std::atof(argv[2]);
    double tStep = std::atof(argv[3]);
    double kilossrate = std::atof(argv[4]);
    double thyinfluxrate = std::atof(argv[5]);
    std::string resfilePath = argv[6];
    int agg_count_t = std::atof(argv[7]) / tStep;
    std::string cloneFreqfilePath = argv[8];
    int agg_freq_t = std::atof(argv[9]) / tStep;

    int NumAgebins_t0 = static_cast<int>(t0 / tStep);

    // array of parameters
    std::vector<double> par_vec{tStep, kilossrate};
    // vector of Agebin objects -- increases in size by 1 at each timestep
    tbb::concurrent_vector<Agebin> A;

    // initialize the agebin objects at t0 -- 50 agebins since tStep = 0.1 (Number of agebins = t/tStep)
    for (int i = 0; i < NumAgebins_t0; ++i)
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

        A.push_back(Agebin(NumAgebins_t0 - i, b0v, b0k, b1v, b1k, b2v, b2k, b3v, b3k));
    }
    
    // remove the file if it exists
    // std::remove(resfilePath);
    // std::remove(cloneFreqfilePath);

    std::ofstream resfile(resfilePath);
    resfile << "Time, Agebin, Total_nai_dis, Total_nai_inc, Total_mem_fast, Total_mem_slow, Kifrac_nai_dis, Kifrac_nai_inc, Kifrac_mem_fast, Kifrac_mem_slow \n";

    // std::ofstream cloneFreqfile("output_files/cloneFreq1.csv");
    std::ofstream cloneFreqfile(cloneFreqfilePath);
    cloneFreqfile << "Time, Agebin, Clone ID, seq_nai_dis, seq_nai_inc, seq_mem_fast, seq_mem_slow \n";

    for (int i = 0; i < A.size(); i++)
    {
        // intial conditions to CSV
        writeResultsToCSV(A.at(i), t0, resfile);         // counts and Ki67 fractions
        writeCloneFreqToCSV(A.at(i), t0, cloneFreqfile); // clone frequencies
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

        // // second loop -- iterating over agebins
        tbb::parallel_for(tbb::blocked_range<size_t>(0, A.size()),
            [&](const tbb::blocked_range<size_t>& r) {
                for (size_t i = r.begin(); i != r.end(); ++i) {
                    // update the agebins at every step
                    doAgebin(A[i], par_vec);
                }
            }
            // tbb::simple_partitioner()
        );

        if (x % agg_count_t == 0) // number of steps 70 == 7 days, since each step is 0.1 days
        {
            for(int i = 0; i < A.size(); i++){
                writeResultsToCSV(A[i], currentTime, resfile);
            }

        }

        if (x % agg_freq_t == 0) // number of steps 900 == 90 days, since each step is 0.1 days
        {
            for(int i = 0; i < A.size(); i++){
                writeCloneFreqToCSV(A[i], currentTime, cloneFreqfile);
            }
        }

        A.push_back(Agebin(0, b0vNu, b0kNu));
    }

    cloneFreqfile.close();
    resfile.close();

    std::cout << "Simulation complete\n";

    return 0;
}