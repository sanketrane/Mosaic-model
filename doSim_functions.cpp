#include "doSim_functions.h"

// Definitions

void doSubpop(Subpop &subpop, std::vector<double> &par_vec, Subpop *precursor1 = nullptr, Subpop *precursor2 = nullptr)
{
    // fate of the subpop in this timestep
    // define input params
    double timeStep{par_vec[0]};
    double kilossrate{par_vec[1]}; // rate of loss of Ki67 on T cells

    // first define probabilities
    double p_loss{1 - exp(-subpop.lossrate() * timeStep)};     // prob of cellular loss by death and differentiation
    double p_egress{1 - exp(-subpop.egressrate() * timeStep)}; // prob of differentiation to memory
    double p_divide{1 - exp(-subpop.divrate() * timeStep)};    // prob of division
    double p_kiflip{1 - exp(-kilossrate * timeStep)};          // prob that a cell stochastically loses Ki67 expression

    // get the vector storing clone IDs from the subpop and assign it to a new vector to modify
    std::vector<int> cloneIDvec{subpop.getCloneIDvec()};

    // fate decisions
    // 1) Persistance -- i.e. tallying after total loss (death + egress) -- at preseant we only have info about total loss rate from our fits
    int num_persist{randbinom(subpop.getNumCells(), 1 - p_loss)};
    if (num_persist <= 0)
    {
        return; // no cells in the subpop
    }

    // Sample num_persist elements from the m_cloneID_vec without replacement.
    std::vector<int> clone_persist;
    samplerNfromVector(cloneIDvec, clone_persist, num_persist);

    // 2) Egress -- sub-part of the total loss -- need to track separately for clonal movement

    // lost clones: differnce between original and persisting
    std::vector<int> clone_loss{differenceOfVectors(cloneIDvec, clone_persist)};

    // egressed clones are sampled from the lost clones
    int num_egress{randbinom(subpop.getNumCells(), p_egress)};
    std::vector<int> clone_egress; // vector to store the egressed clones

    // sampling without replacement
    samplerNfromVector(clone_loss, clone_egress, num_egress);

    // Ki67 loss and egress are independent events --
    // number of Ki67 pos cells in egressed compartment = ki fraction in original * num_egress
    int numKi67_egress = subpop.getKifrac() * num_egress;

    // 3) Ageing -- loss in KI67pos cells
    int num_loseKi67{randbinom(num_persist, p_kiflip)}; // sequential

    // update ki67pos numbers
    int num_NewKi67pos{subpop.getNumKi67pos()};
    if (subpop.getNumKi67pos() <= 0)
    {
        return; // no Ki67pos cells in the subpop
    }
    else if ((num_loseKi67 + numKi67_egress) > subpop.getNumKi67pos())
    {
        return; // not enough Ki67pos cells in the subpop
    }

    // leftover Ki67 positive cells after kiflip and egress
    num_NewKi67pos -= (num_loseKi67 + numKi67_egress);

    // 4) Division -- all cells if divide become Ki67 positive (irresepctive of their prior Ki67 status -- i.e., all cells if divide become Ki67 positive)
    int num_divide{randbinom(num_persist, p_divide)}; // sequential
    // final number of Ki67 positive cells
    num_NewKi67pos += num_divide; // two progeny - one parent

    // Addition to the existing clone families from division --  Sample num_NewKi67pos elements from the clone_persist without replacement.
    // note: no new clones are added at this stage -- cells to existing clones are added.
    std::vector<int> clone_persist_div(clone_persist); // intiating clone_persist_div as clone persist

    // sampling function appends sampled vector at the back of clone_persist_div -- contains persisting and new divided cells
    samplerNfromVector(clone_persist, clone_persist_div, num_divide);

    // 5) Influx -- new cells coming from subpop_(n-1)
    // append influx vector to persisting + division-addition-clones vector
    std::vector<int> clone_persist_div_inflx(clone_persist_div); // instatiating clone_persist_div as clone persist

    if (precursor1 != nullptr && precursor2 == nullptr)
    {
        clone_persist_div_inflx.insert(clone_persist_div_inflx.end(), precursor1->getCloneEgress().begin(), precursor1->getCloneEgress().end());
        num_NewKi67pos += precursor1->getNumKi67Egress();

        if (subpop.getCellType() == Subpop::CellType::nai_dis)
        {
            std::cout << "Influx from thy_Ntreg: " << precursor1->getCloneEgress().size()
                      << " current clone_persist_div_inflx size: " << clone_persist_div_inflx.size()
                      << '\n';
        }
    }
    else if (precursor1 != nullptr && precursor2 != nullptr)
    {
        clone_persist_div_inflx.insert(clone_persist_div_inflx.end(), precursor1->getCloneEgress().begin(), precursor1->getCloneEgress().end());
        clone_persist_div_inflx.insert(clone_persist_div_inflx.end(), precursor2->getCloneEgress().begin(), precursor2->getCloneEgress().end());
        num_NewKi67pos += precursor1->getNumKi67Egress() + precursor2->getNumKi67Egress();
    }

    // update the subpop object
    subpop.setNumKi67pos(num_NewKi67pos);
    subpop.setCloneIDvec(clone_persist_div_inflx);
    subpop.setNumKi67Egress(numKi67_egress);
    subpop.setCloneEgress(clone_egress);

    return;
}

void doAgebin(Agebin &A, std::vector<double> &par_vec)
{
    // sequential from subpop naive to memory
    if (A.getB0().getNumCells() > 0)
    {
        doSubpop(A.getB0(), par_vec, nullptr, nullptr);
    }

    if (A.getB1().getNumCells() > 0)
    {
        doSubpop(A.getB1(), par_vec, nullptr, nullptr);
    }

    if (A.getB2().getNumCells() > 0)
    {
        doSubpop(A.getB2(), par_vec, &A.getB0(), &A.getB1());
    }

    if (A.getB3().getNumCells() > 0)
    {
        doSubpop(A.getB3(), par_vec, &A.getB2());
    }

    // Ageing
    A.incrementAgebinID();
}
