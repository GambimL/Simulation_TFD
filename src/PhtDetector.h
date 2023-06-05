#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class phtDetector : public G4VUserDetectorConstruction {

public:
    phtDetector(); 
    ~phtDetector();
    virtual G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField();


    void GetNumberOfSegmentsInPhantom(G4int& nx, G4int& ny, G4int& nz)
        const{ nx=fNx; ny = fNy; nz = fNz; };

   

private:

    G4ThreeVector wordsize;
    G4ThreeVector agtsize;
    G4ThreeVector detecwsize;
    G4ThreeVector phantomsize;
    G4int fNx,fNy,fNz; 
    G4LogicalVolume* lvwordZ;
   
    






};





