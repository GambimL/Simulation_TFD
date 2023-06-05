
#include "source.h"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4GeneralParticleSource.hh"
#include "Randomize.hh"



source::source() : 
  G4VUserPrimaryGeneratorAction(), 
  fsource(0),
  generalsource(0) {
   

  //utilizando uma fonte geral
  generalsource =  new G4GeneralParticleSource();
  generalsource->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
  generalsource->SetParticlePolarization(G4ThreeVector(0., 0., 0.));

  //Utilizando a ParticleGun
  /*G4int numparticles = 1;
  G4String particlename = "gamma"; 
  fRandomDirection = true;*/

  /*fsource = new G4ParticleGun (numparticles);
  fsource->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
  fsource->SetParticleEnergy(1.95 * MeV);
  fsource->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));*/
 
}


void source::GeneratePrimaries(G4Event* anEvent)
{
  //particulas aleatorias
  /* if(fRandomDirection)
  {
    G4double theta = CLHEP::halfpi * G4UniformRand();
    G4double phi   = CLHEP::twopi * G4UniformRand();
    G4double x     = std::cos(theta);
    G4double y     = std::sin(theta) * std::sin(phi);
    G4double z     = std::sin(theta) * std::cos(phi);
    G4ThreeVector dir(x, y, z);
    fsource->SetParticleMomentumDirection(dir);
  }; */
 
  //Utilizando a ParticleGun
  //fsource->GeneratePrimaryVertex(anEvent);

  //utilizando uma fonte geral
  generalsource->GeneratePrimaryVertex(anEvent);
}
