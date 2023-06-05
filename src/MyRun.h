#pragma once 

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include <vector>

class MyRun : public G4Run {
  
public:
    MyRun();
    virtual ~MyRun();

    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);
    G4THitsMap<G4double>  GetFlux();

    G4int GetNumeberOfHitsMap() const {return runMap.size();}
    G4THitsMap<G4double>* GetHitsMap(G4int i) {return runMap[i];}
    G4THitsMap<G4double>* GetHitsMap(const G4String &detName, 
                                        const G4String &collectionName);
    G4THitsMap<G4double>* GetHitsMap (const G4String &fullName);

    void DumpAllScorer();

private:
    std::vector<G4String> collectionName;
    std::vector<G4THitsMap<G4double>*> runMap;
    std::vector<G4int> collectionID;
    
    

};

