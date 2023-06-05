#pragma once

#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;

class source : public G4VUserPrimaryGeneratorAction{
 
public:
  source(); 
  virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
  G4ParticleGun* fsource;
  G4bool fRandomDirection;
  G4GeneralParticleSource* generalsource;


};