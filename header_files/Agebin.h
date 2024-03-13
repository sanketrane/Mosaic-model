#include <iostream>
#include <vector>
#include "Subpop.h"

#ifndef AGE_BIN
#define AGE_BIN

// defining and (default) initializing a SubsetClass
// carries the information for each subset
class Agebin
{
public:
    Agebin() = default; // default constructor

    Agebin(int age, std::vector<int> &b0v, int b0k, std::vector<int> &b1v, int b1k, std::vector<int> &b2v, int b2k, std::vector<int> &b3v, int b3k) // constructor
        : m_agebinID{age}, B0{Subpop::CellType::nai_dis}, B1{Subpop::CellType::nai_inc}, B2{Subpop::CellType::mem_fast}, B3{Subpop::CellType::mem_slow}
    {
        B0.setCloneIDvec(b0v);
        B1.setCloneIDvec(b1v);
        B2.setCloneIDvec(b2v);
        B3.setCloneIDvec(b3v);

        B0.setNumKi67pos(b0k);
        B1.setNumKi67pos(b1k);
        B2.setNumKi67pos(b2k);
        B3.setNumKi67pos(b3k);
    }

    Agebin(int age, std::vector<int> b0vNu, int b0kNu) // constructor 2
        : m_agebinID{age}, B0{Subpop::CellType::nai_dis}, B1{Subpop::CellType::nai_inc}, B2{Subpop::CellType::mem_fast}, B3{Subpop::CellType::mem_slow}
    {
        B0.setCloneIDvec(b0vNu);
        B0.setNumKi67pos(b0kNu);
    }

    // getter functions
    Subpop &getB0() { return B0; }
    Subpop &getB1() { return B1; }
    Subpop &getB2() { return B2; }
    Subpop &getB3() { return B3; }

    const auto &getAgebinID() const { return m_agebinID; }
    const auto &getB0Type() const { return B0.getCellType(); }
    const auto &getB1Type() const { return B1.getCellType(); }
    const auto &getB2Type() const { return B2.getCellType(); }
    const auto &getB3Type() const { return B3.getCellType(); }

    // setter functions
    void incrementAgebinID() { ++m_agebinID; }

    void setB0(std::vector<int> b) { B0.setCloneIDvec(b); }
    void setB1(std::vector<int> b) { B1.setCloneIDvec(b); }
    void setB2(std::vector<int> b) { B2.setCloneIDvec(b); }
    void setB3(std::vector<int> b) { B3.setCloneIDvec(b); }

private:
    int m_agebinID{};
    Subpop B0{Subpop::CellType::nai_dis};
    Subpop B1{Subpop::CellType::nai_inc};
    Subpop B2{Subpop::CellType::mem_fast};
    Subpop B3{Subpop::CellType::mem_slow};
};

#endif