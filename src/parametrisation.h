#pragma once 

#include "G4Types.hh"
#include "G4VNestedParameterisation.hh" 
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include <vector>


class parametrisation : public G4VNestedParameterisation {

public: 
    parametrisation(const G4ThreeVector& voxelSize, 
                    G4int nz, std::vector<G4Material*>& mat);
    ~parametrisation();


   G4Material* ComputeMaterial(G4VPhysicalVolume *currentVol,
                                const G4int repNo, 
                                const G4VTouchable *parentTouch=0);
    
    G4int GetNumberOfMaterials() const; 
    G4Material* GetMaterial(G4int idx) const; 

    void ComputeTransformation(const G4int no,
                               G4VPhysicalVolume *currentPV) const; 

    void ComputeDimensions(G4Box& box, const G4int, const G4VPhysicalVolume* ) const; 

 

private:
    G4double fdX,fdY,fdZ;
    G4int fNz;
    std::vector<G4double>  fpZ;
    std::vector<G4Material*> fMat;

};