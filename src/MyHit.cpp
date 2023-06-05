#include "MyHit.h"
#include "G4UnitsTable.hh"

MyHit::MyHit(G4Step* aStep) : TrackID( aStep->GetTrack()->GetTrackID() ),
    position( aStep->GetPreStepPoint()->GetPosition() ),
    direction( aStep->GetPreStepPoint()->GetMomentumDirection() )
{}

MyHit::~MyHit(){
    
}

void MyHit::Print(){

    G4cout
    << "TrackID" << TrackID
    << "Posição" << std::setw(7) << G4BestUnit(position, "Length")
    << "Direção" << std::setw(7) << direction
    << G4endl;

}