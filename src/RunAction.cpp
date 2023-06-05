#include "RunAction.h"
#include "MyRun.h"
#include "useraction.h"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "PhtDetector.h"
#include "G4VHitsCollection.hh"



RunAction::RunAction(): G4UserRunAction(), 
fNx(0), 
fNy(0), 
fNz(0){

    /* SDName.push_back(G4String("PhantomSD")); */


};

RunAction::~RunAction(){;}


G4Run* RunAction::GenerateRun(){ 
    return new MyRun(); 
    
}




void RunAction::EndOfRunAction(const G4Run* aRun){

    MyRun* myRun = (MyRun*)aRun;

    if( IsMaster() ){
         MyRun* myRun = (MyRun*)aRun;

        myRun->DumpAllScorer();

         const phtDetector* detector = (const phtDetector*)
            (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
            detector->GetNumberOfSegmentsInPhantom(fNx,fNy,fNz);

        G4cout << "============================================================="
        << G4endl;
        G4cout << " Number of event processed : "<< aRun->GetNumberOfEvent() << G4endl;
        G4cout << "============================================================="
        << G4endl;
        
        G4THitsMap<G4double>* cFx = myRun->GetHitsMap("PhantomSD/CellFlux");
           
        G4cout << std::setw(20) << cFx->GetName() << G4endl;
        

        G4int ix = fNx/2;  
        G4int iy = fNy/2;
        G4int iz;  
        G4cout << "============================================================="
        << G4endl;
        for ( iz = 0; iz < fNz; iz++){ 

            G4double* cF = (*cFx)[CopyNo(ix,iy,iz)];
            if ( !cF ) cF = new G4double(0.0);
            G4cout << std::setw(6) << iz << "  "
                << std::setw(15) << (*cF)*cm*cm   <<" /cm2"
                << G4endl;
        }
        G4cout << "============================================================="
        << G4endl;
        
        std::ofstream  file("totED.csv");
        file <<"ix"<< ";"<<"iy"<<";"<<"iz"<<";"<< "Fluxo" <<  G4endl;
        for ( iz = 0; iz < fNz; iz++){   
            for ( iy = 0; iy < fNy; iy++){ 
                for ( ix = 0; ix < fNx; ix++){ 
                    G4double* totED = (*cFx)[CopyNo(ix,iy,iz)];
                    if ( !totED ) totED = new G4double(0.0) ; 
                    file << ix << ";"<<iy<<";"<<iz<<";"<<  *totED <<  G4endl; 

                }
            }
        }
        file.close(); 
        
    }
}

