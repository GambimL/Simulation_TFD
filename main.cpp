#include "src/PhtDetector.h"
#include "src/source.h"
#include "src/useraction.h"
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#define G4VIS_USE_OPENGLX
#define G4VIS_USE_OPENGLQT 
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ScoringManager.hh"

int main(int argc, char** argv){

  auto rmananger = new G4RunManager();
  G4ScoringManager* scoringManager = G4ScoringManager::GetScoringManager(); 
  
  G4VModularPhysicsList* physlist = new FTFP_BERT;
  G4OpticalPhysics* opt = new G4OpticalPhysics();
  physlist->RegisterPhysics(opt);

  rmananger->SetUserInitialization(new phtDetector());
  rmananger->SetUserInitialization(physlist);
  rmananger->SetUserInitialization(new useraction());
  rmananger->Initialize();
  
  auto visMananger = new G4VisExecutive("errors");
  visMananger -> Initialize(); 
  auto *uiExecutive = new G4UIExecutive(argc, argv, "Qt");
  uiExecutive->SessionStart();

};

