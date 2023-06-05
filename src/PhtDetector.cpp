#include "PhtDetector.h"
#include "G4Element.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4Colour.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSCellFlux3D.hh"
#include "G4SDManager.hh"
#include "G4Voxelizer.hh"
#include "parametrisation.h"
#include "G4PVParameterised.hh"
#include "G4Navigator.hh"
#include "G4PSPassageCellFlux3D.hh"
#include "G4PSEnergyDeposit3D.hh"

 
 
using namespace CLHEP;


phtDetector::phtDetector() : G4VUserDetectorConstruction(), wordsize(0) 
{
  // Default size of water phantom,and segmentation.
  wordsize.setX(32.*mm);
  wordsize.setY(32.*mm);
  wordsize.setZ(15*mm);
  fNx = fNy = 128;
  fNz =  60;
  phantomsize.setX(10.0);
  phantomsize.setY(32.0);
  phantomsize.setZ(32.0);
  agtsize.setZ(2.0);
  agtsize.setY(32.0);
  agtsize.setX(32.0);

};


phtDetector::~phtDetector(){
};

G4VPhysicalVolume* phtDetector::Construct(){
  
  G4bool checkOverlaps;
  G4double a, z, density;
  G4int nelements;
  G4double tx, ty, tz;
  

  
  //definindo o material 
  auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  G4MaterialPropertiesTable* proAir = new G4MaterialPropertiesTable();
  air->SetMaterialPropertiesTable(proAir);
  auto water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  G4MaterialPropertiesTable* proWa = new G4MaterialPropertiesTable();
  water->SetMaterialPropertiesTable(proWa); 
  auto skin = G4NistManager::Instance()->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4MaterialPropertiesTable* proAc = new G4MaterialPropertiesTable();
  skin->SetMaterialPropertiesTable(proAc);
  
  //propriedades 
  const int N_DATA = 2; 
  G4double energiesair[2] = {1.95*eV, 1.95*eV};
  G4double rindexair[5] = {1.0, 1.0};
  G4double energies[N_DATA] = {1.95 * MeV, 1.95 * MeV};
  G4double rindexwa[N_DATA] = {1.41, 1.41};
  G4double abslengthwa[N_DATA] = {3.70 * mm, 3.70 * mm};
  G4double rindexac[N_DATA] = {1.9, 1.9};
  G4double abslengthac[N_DATA] = {3.70 * mm, 3.70 * mm};
  G4double miehg[N_DATA] = {0.053, 0.053};
  G4double miewaterconst[3] = { 0.81, 0.81, 1};

  proWa->AddProperty("RINDEX", energies, rindexwa, N_DATA);
  proWa->AddProperty("ABSLENGTH", energies, abslengthwa, N_DATA);


  proAc->AddProperty("RINDEX", energies, rindexac, N_DATA);
  proAc->AddProperty("ABSLENGTH", energies, abslengthac, N_DATA);
  proAc->AddProperty("MIEHG", energies, miehg, N_DATA);
  proAc->AddConstProperty("MIEHG_FORWARD", miewaterconst[1]);
  proAc->AddConstProperty("MIEHG_BACKWARD", miewaterconst[1]); 
  proAc->AddConstProperty("MIEHG_FORWARD_RATIO", miewaterconst[2]); 
  proAir->AddProperty("RINDEX", energiesair, rindexair, N_DATA);
    
 


  //definindo o detector 32.0, 32.0, 15.5
  //The word
  G4ThreeVector WordSize = wordsize;

  
 

  G4Box* wair = 
    new G4Box("airbox", tx = WordSize.x()/2 * mm, ty = WordSize.y()/2 * mm, tz = WordSize.z()/2 * mm);
  G4LogicalVolume* lair = new G4LogicalVolume(wair, air, "logicalair", 0, 0, 0);
  G4VPhysicalVolume* pair = 
    new G4PVPlacement(0, {0,0,0}, lair, "air", 0, false, 0);
  

  G4Box* world = 
    new G4Box("Worldbox", tx = WordSize.x()/2 * mm, WordSize.y()/2 * mm, tz = WordSize.z()/2 * mm);
  G4LogicalVolume* lworld = new G4LogicalVolume(world, water, "logicalWorld", 0, 0, 0);
  G4VPhysicalVolume* world_phys = 
    new G4PVPlacement(0, {0,0,0*mm}, lworld, "world", lair, false, 0);
  lworld->SetVisAttributes( new G4VisAttributes(G4Color::Yellow()));
  lworld->SetVisAttributes( new G4VisAttributes(false));



  //The phanton
  G4Box* Pht = new G4Box("phantonbox", tx =  WordSize.x()/2 *mm, ty 
   = WordSize.y()/2 * mm, tz = 5 * mm);
  G4LogicalVolume* lPht = new G4LogicalVolume(Pht, skin, "logicalphanton", 0, 0, 0);
  G4VPhysicalVolume* ppht = 
    new G4PVPlacement(0, {0,0,-2.5*mm}, lPht, "phanton", lair, false, 0);
  lPht->SetVisAttributes( new G4VisAttributes(G4Color::Red()));
  
  

  //The AC
  G4Box* couplAgt = new G4Box("agtacopbox", tx =  32.0/2 * mm, ty = 32.0/2 * mm, tz = 1.5 * mm );
  G4LogicalVolume* lCoup = new G4LogicalVolume(couplAgt, water, "logicalagtacop", 0, 0, 0);
  G4VPhysicalVolume* pCoup = 
    new G4PVPlacement(0, {0,0,4*mm}, lCoup, "agtacop", lair, false, 0);
  lCoup->SetVisAttributes( new G4VisAttributes(G4Colour::Cyan()));


  //define the voxel filter
  G4int nxCells = fNx; 
  G4int nyCells = fNy;
  G4int nzCells = fNz;  


  G4ThreeVector sensSize;
  sensSize.setX(WordSize.x()/(G4double)nxCells);
  sensSize.setY(WordSize.y()/(G4double)nyCells);
  sensSize.setZ(WordSize.z()/(G4double)nzCells);

  //The Voxel phanton
  //Y SLICE
  G4Box* vwordY = 
    new G4Box("voxelwordY", tx = 32.0/2 *mm, ty = sensSize.y()* mm, tz = WordSize.z()/2 * mm);
    G4LogicalVolume* lvwordY = new G4LogicalVolume(vwordY, water, "lvoxelwordY");
  G4PVReplica* rvwordY = 
    new G4PVReplica("rvoxelwordY", lvwordY, lworld, kYAxis, fNy, sensSize.y());
 
  
  

  //X SLICE
  G4Box* vwordX = 
    new G4Box("voxelwordX", tx = sensSize.x()/2 *mm, ty = sensSize.y()/2 * mm, tz = WordSize.z()/2 * mm);
  G4LogicalVolume* lvwordX = new G4LogicalVolume(vwordX, water, "lvoxelwordX");
  G4PVReplica* rvwordX = new G4PVReplica("rvoxelwordX", lvwordX, lvwordY, 
                                          kXAxis, fNx, sensSize.x());

                                         

  //Z SLICE
  G4String zVoxName("phantomSens");
  G4VSolid* vwordZ = 
  new G4Box(zVoxName, sensSize.x()/2.,sensSize.y()/2.,tz = sensSize.z()/2 * mm);
  lvwordZ = new G4LogicalVolume(vwordZ,water,zVoxName);

  std::vector<G4Material*> simulationMaterial;
  simulationMaterial.push_back(air);
  simulationMaterial.push_back(water);
  simulationMaterial.push_back(skin);

  parametrisation* param =  new parametrisation(sensSize/2, nzCells, simulationMaterial);
  G4VPhysicalVolume* physiPhantomSens = new G4PVParameterised("PhantomSens",    
                          lvwordZ,    
                          lvwordX,           
                          kUndefined,         
                          nzCells,           
                          param);  
  

  
  

  G4VisAttributes* visAttri = new G4VisAttributes(G4Colour::Yellow());
  visAttri->SetForceWireframe(true);

  

  auto surfaceAirAgt = new G4OpticalSurface("AirAgt surface");
  surfaceAirAgt->SetModel(unified);
  surfaceAirAgt->SetType(dielectric_dielectric);
  surfaceAirAgt->SetFinish(polished);

  auto propAirAgt = new G4MaterialPropertiesTable();
  surfaceAirAgt->SetMaterialPropertiesTable(propAirAgt);

  auto logsurfaceAirAgt =  
    new G4LogicalBorderSurface("airtoagt", pair, pCoup, surfaceAirAgt);
  auto logsurfaceaggtair =  
    new G4LogicalBorderSurface("agttoair", pCoup, pair, surfaceAirAgt);

  G4double refletairagt[N_DATA] = {0.5, 0.5};
  propAirAgt->AddProperty("REFLECTIVITY",energies ,refletairagt, N_DATA);


  auto surfacephtAgt = new G4OpticalSurface("phtAgt surface");
  surfacephtAgt->SetModel(unified);
  surfacephtAgt->SetType(dielectric_dielectric);
  surfacephtAgt->SetFinish(polished);

  auto propphtAgt = new G4MaterialPropertiesTable();
  surfacephtAgt->SetMaterialPropertiesTable(propphtAgt);

  auto logsurfacephtAgt =  
    new G4LogicalBorderSurface("airtoagt", ppht, pCoup, surfacephtAgt);
  auto logsurfaceaggtpht =  
    new G4LogicalBorderSurface("agttoair", pCoup, ppht, surfacephtAgt);

  G4double refletphtagt[N_DATA] = {0.5, 0.5};
  propAirAgt->AddProperty("REFLECTIVITY",energies ,refletairagt, N_DATA);

  
 
 return pair;
}


void phtDetector::ConstructSDandField(){

  G4SDManager* manangerSD = G4SDManager::GetSDMpointer();
  G4String phantomName = "PhantomSD";
  G4MultiFunctionalDetector* scorer =  new G4MultiFunctionalDetector(phantomName);
  manangerSD->AddNewDetector(scorer);
  lvwordZ->SetSensitiveDetector(scorer);
  G4PSPassageCellFlux3D* lightflux = new G4PSPassageCellFlux3D("CellFlux", fNx, fNy, fNz);
  scorer->RegisterPrimitive(lightflux);
  

} 