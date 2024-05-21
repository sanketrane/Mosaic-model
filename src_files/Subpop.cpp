// Purpose: Implementation of the Subpop class.

// header file
#include "header_files/Subpop.h"

// member functions of the Subpop class

/**
 * @return The fraction of Ki67 expressing cells in the subset.
 */
double Subpop::getKifrac() // calculates Ki67 fraction
{
    if (getNumCells() == 0 || getNumKi67pos() == 0)
    {
        return 0;
    }
    else
    {
        return static_cast<double>(getNumKi67pos()) / static_cast<double>(getNumCells());
    }
}

/**
 * @return The loss rate as a double value based on the cell type.
 */
double Subpop::lossrate()
{
    switch (getCellType())
    {
    case Subpop::CellType::nai_dis:
        return 0.035;
    case Subpop::CellType::nai_inc:
        return 0.061;
    case Subpop::CellType::mem_fast:
        return 0.1;
    case Subpop::CellType::mem_slow:
        return 0.022;
    default:
        std::cerr << "Error: Cell type not recognized\n";
        return 0;
    }
}

/**
 * @return The division rate as a double value based on the cell type.
 */
double Subpop::divrate()
{
    switch (getCellType())
    {
    case Subpop::CellType::nai_dis:
        return 0.017;
    case Subpop::CellType::nai_inc:
        return 0.062;
    case Subpop::CellType::mem_fast:
        return 0.15;
    case Subpop::CellType::mem_slow:
        return 0.023;
    default:
        std::cerr << "Error: Cell type not recognized\n";
        return 0;
    }
}

/**
 * @return the egress rate as a double value based on the cell type.
 */
double Subpop::egressrate()
{
    switch (getCellType())
    {
    case Subpop::CellType::nai_dis:
        return 0.001;
    case Subpop::CellType::nai_inc:
        return 0.001;
    case Subpop::CellType::mem_fast:
        return 0.23;
    case Subpop::CellType::mem_slow:
        return 0.0;
    default:
        std::cerr << "Error: Cell type not recognized\n";
        return 0;
    }
}
