#pragma once
#include "G4VHit.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

class MyHit : public G4VHit{

public:
    MyHit(G4Step* aStep);
    ~MyHit();

    void Print() override;
private:

    G4int TrackID;
    G4ThreeVector position, direction;


};