#include "sensordetector.h"
#include "MyHit.h"
#include "G4PSCellFlux.hh"



sensordetector::sensordetector(const G4String &name) : G4VSensitiveDetector(name){
    ;
}
sensordetector::~sensordetector(){
    ;
}

G4bool sensordetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist){
    
    if(true){
        auto aHit =  new MyHit( aStep );
        aHit->Print();
    }
}