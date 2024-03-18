// Created by: Sanket Rane (March 2024)

#ifndef SUBPOP_CLASS
#define SUBPOP_CLASS

// include statements
#include <iostream>
#include <vector>

// defining and (default) initializing a Subpop class
// carries the information for each subset

class Subpop
{
public:
    enum class CellType
    {
        nai_dis,
        nai_inc,
        mem_fast,
        mem_slow,
    };

    Subpop() = default; // default constructor

    Subpop(CellType celltype) // constructor
        : Tp{celltype}
    {
    }

    // getter functions
    const auto &getCellType() const { return Tp; }
    const auto &getCloneIDvec() const { return m_cloneIDvec; }
    auto getNumCells() const { return m_cloneIDvec.size(); } // note no reference .size() produces temp local object
    const auto &getNumKi67pos() const { return m_numKi67pos; }

    const auto &getCloneEgress() const { return m_cloneEgress; }
    const auto &getNumKi67Egress() const { return m_numKi67Egress; }

    // setter functions
    void setCloneIDvec(std::vector<int> b) { m_cloneIDvec = b; }
    void setNumKi67pos(int numK) { m_numKi67pos = numK; }

    void setCloneEgress(std::vector<int> &vec) { m_cloneEgress = vec; }
    void setNumKi67Egress(int numK) { m_numKi67Egress = numK; }

    // member functions
    double getKifrac();
    double lossrate();
    double divrate();
    double egressrate();

private:
    CellType Tp;                      // type of the subset
    std::vector<int> m_cloneIDvec{};  // vector containing clones in this subset
    int m_numKi67pos{};               // number of cells that are currently Ki67 positive
    std::vector<int> m_cloneEgress{}; // vector containing clones moving to next subset
    int m_numKi67Egress{};            // number of KI67 positive cells that are moving to next subset
};

#endif // SUBPOP_CLASS