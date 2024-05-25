#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <cxxopts.hpp>

// header files
#include "header_files/Agebin.h"
#include "header_files/Subpop.h"
#include "header_files/accessory_functions.h"
#include "header_files/procedural_functions.h"
#include "header_files/doSim_functions.h"

int main(int argc, char *argv[])
{
    // parse the input arguments
    cxxopts::Options options("MyProgram", "One line description of MyProgram"); // Create a cxxopts::Options instance.

    options.add_options()
        /*
        list of arguments to parse to the program
        format: ("name (oprtional short name separated by a comma)", "description", cxxopts::value<type>())
        if the third argument is not provided, the value is assumed to be a boolean value
         */
        ("tFin", "Final time of simulation", cxxopts::value<double>())                                                        //
        ("tInit", "Initial time of simulation", cxxopts::value<double>())                                                     //
        ("tStep", "Time step of simulation", cxxopts::value<double>()->default_value("0.04"))                                 // defaults to timestep of 0.04 days ~= 1 hour
        ("cloneUniverse", "Total number of clone IDs to consider", cxxopts::value<int>())                                     //
        ("kilossrate", "Parameter in the model -- Rate of Ki67 loss", cxxopts::value<double>())                               //
        ("thyinfluxrate", "parameter in the model -- Rate of thymic naive influx", cxxopts::value<double>())                  //
        ("resfilePath", "Path to the results file", cxxopts::value<std::string>()->default_value("output_files/Results.csv")) //
        ("writeRes_counter", "step counter for main results output", cxxopts::value<int>())                                   //
        ("h,help", "Print usage");                                                                                            //

    auto inputArgs = options.parse(argc, argv);

    if (inputArgs.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    double tFin = inputArgs["tFin"].as<double>();
    double tInit = inputArgs["tInit"].as<double>();
    double tStep = inputArgs["tStep"].as<double>();
    int cloneUniverse = inputArgs["cloneUniverse"].as<int>();
    double kilossrate = inputArgs["kilossrate"].as<double>();
    double thyinfluxrate = inputArgs["thyinfluxrate"].as<double>();
    std::string resfilePath = inputArgs["resfilePath"].as<std::string>();
    int writeRes_counter = inputArgs["writeRes_counter"].as<int>();

    // intial number of the mosaic classes -- here cell age based bins
    int NumAgebins_tInit = static_cast<int>(tInit / tStep);
    std::cout << "NumAgebins_tInit: " << NumAgebins_tInit << '\n';

    // array of parameters (fixed) to be passed to the "do" functions
    std::vector<double> par_vec{tStep, kilossrate};

    // vector of Agebin objects -- increases in size by 1 at each timestep
    tbb::concurrent_vector<Agebin> A;

    // initialize the agebin objects at tInit
    for (size_t i = 0; i < A.size(); i++)
    {
        // initialize the agebins
        // initial number of cells in each subset are based on data

        // we start with 1000 * 50 = 50,000 cells in the naive dis subset -- this will be fed from naive thy subset at every tStep
        std::vector<int> b0v{randVector(1000, 0, cloneUniverse)};

        // we start with 1000 * 50 = 50,000 cells in the naive Inc subset -- No influx after tInit in this subset -- stays same in size
        std::vector<int> b1v{randVector(1000, 0, cloneUniverse)};

        // we start with 200 * 50 = 10,000 cells in the memory subset -- split 90:10 between fast:slow
        std::vector<int> b2v{randVector(150, 0, cloneUniverse)}; //-- this will be fed from naive dis + naive inc subsets at every tStep
        std::vector<int> b3v{randVector(50, 0, cloneUniverse)};  //-- this will be fed from mem_fast inc subsets at every tStep

        // Note: memory slow subset at tInit -- 0 cells -- value initialized to 0 in the constructor

        // initial number of Ki67 positive cells in each subset are based on intuition
        int b0k{400 + rand() % (700 - 400 + 1)}; // random number between 400 and 700
        int b1k{400 + rand() % (700 - 400 + 1)};
        int b2k{90 + rand() % (120 - 90 + 1)}; // random number between 120 and 180
        int b3k{30 + rand() % (45 - 30 + 1)};  // random number between 120 and 180

        A.push_back(Agebin(NumAgebins_tInit - i, b0v, b0k, b1v, b1k, b2v, b2k, b3v, b3k));
    }

    // remove the output files if they exists
    std::remove(resfilePath.c_str());

    // open the output files
    std::ofstream resfile(resfilePath);
    // write the header to the results file
    resfile << "Time, Agebin, Total_nai_dis, Total_nai_inc, Total_mem_fast, Total_mem_slow, Kifrac_nai_dis, Kifrac_nai_inc, Kifrac_mem_fast, Kifrac_mem_slow \n";

    for (size_t i = 0; i < A.size(); i++) // A.size returns a variable of type size_t which is an unsigned integer type; i needs to match
    {
        // write the initial values to the output files
        writeResultsToCSV(A.at(i), tInit, resfile);
    }

    // step counter intiated at NumAgebins_tInit since we have already initialized NumAgebins_tInit agebins and we want to make sure number of steps is consistent with the number of agebins
    int x = NumAgebins_tInit;

    // main loop -- iterating over time
    for (double currentTime = tInit + tStep; currentTime < tFin; currentTime += tStep)
    {

        // Influx of thymic naive cells as vecotrs of clone IDs
        double numThyInflux = thyinfluxrate * thyNtregSize(currentTime) * tStep; // thymic naive influx at every time step
        // numThyInflux number of naive dis cells coming from thymus
        // as vector of clone IDs sampled from a uniform disribution ranging from 0 to clone universe
        std::vector<int> b0vNu{randVector(numThyInflux, 0, cloneUniverse)}; // TODO: check if this is the correct. Should it be 0 to cloneUniverse-1?

        // TODO: make this change with time
        double fraction = 0.3 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (0.5 - 0.3)));
        int b0kNu = numThyInflux * fraction;

        x++; // step counter
        int stepPrint = static_cast<int>(tFin / (10.0 * tStep));
        // std::cout << "stepPrint: " << stepPrint << "\n";
        if (x % stepPrint == 0)
        {
            std::cout << "Time: " << currentTime << " Step: " << x << '\n';
        }

        // // second loop -- iterating over agebins
        tbb::parallel_for(tbb::blocked_range<size_t>(0, A.size()),
                          [&](const tbb::blocked_range<size_t> &r)
                          {
                              for (size_t i = r.begin(); i != r.end(); ++i)
                              {
                                  // update the agebins at every step
                                  doAgebin(A[i], par_vec);
                              }
                          }
                          // tbb::simple_partitioner()
        );

        if (x % writeRes_counter == 0)
        {
            for (size_t i = 0; i < A.size(); i++)
            {
                writeResultsToCSV(A[i], currentTime, resfile);
            }
        }

        A.push_back(Agebin(0, b0vNu, b0kNu));
    }

    resfile.close();

    std::cout << "Simulation complete\n";

    return 0;
}