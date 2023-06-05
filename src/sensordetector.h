#pragma once
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"


class sensordetector : public G4VSensitiveDetector{

public:
    sensordetector(const G4String &name);
    ~sensordetector();

    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

};

