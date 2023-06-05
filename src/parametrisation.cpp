#include "parametrisation.h"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

using namespace CLHEP;


parametrisation::parametrisation(const G4ThreeVector& voxelSize, 
G4int nz, std::vector<G4Material*>& mat)
: G4VNestedParameterisation(),
fdX(voxelSize.x()),
fdY(voxelSize.y()),
fdZ(voxelSize.z()),
fNz(nz),
fMat(mat){

    fpZ.clear();
    G4double zp;
    for ( G4int iz = 0; iz < fNz; iz++){
        zp = (-fNz+1+2*iz)*fdZ;
        fpZ.push_back(zp);
    }; 
}


parametrisation::~parametrisation(){
    fpZ.clear();
}


G4Material* parametrisation::ComputeMaterial(G4VPhysicalVolume* currentVol, 
                  const G4int copyNo, 
                  const G4VTouchable* parentTouch){
    if(parentTouch==0) return fMat[0];
    G4int ix = parentTouch->GetReplicaNumber(0);
    G4int iy = parentTouch->GetReplicaNumber(1);
    G4int iz = copyNo;
  
    G4Material* mat=0;
    if ( iz > 52 ) {
      mat = fMat[0];
    } if (iz <= 52 && iz > 40){ 
      mat = fMat[0];
    } if(iz <= 40) {
      mat = fMat[0];
    };

    return mat;
}


 G4int parametrisation::GetNumberOfMaterials() const{
  return fMat.size();
} 


 void parametrisation::ComputeTransformation(const G4int copyNo, 
                                            G4VPhysicalVolume* physVol) const{
  G4ThreeVector position(0.,0.,fpZ[copyNo]);
  physVol->SetTranslation(position);
} 

G4Material* parametrisation::GetMaterial(G4int i) const{
  return fMat[i];
}


void parametrisation
::ComputeDimensions(G4Box& box, const G4int, const G4VPhysicalVolume* ) const{
  box.SetXHalfLength(fdX);
  box.SetYHalfLength(fdY);
  box.SetZHalfLength(fdZ);
} 


