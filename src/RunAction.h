#pragma once

#include "G4Run.hh"
#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction{
public:
    RunAction();
    ~RunAction();

    G4Run* GenerateRun() override;
    void EndOfRunAction(const G4Run*) override;

    G4int CopyNo(G4int ix, G4int iy, G4int iz) { return (iy*(fNx*fNz)+ix*fNz+iz); }
private:
    std::vector<G4String> SDName;  
    G4int fNx, fNy, fNz;
};