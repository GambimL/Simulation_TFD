#include "MyRun.h"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"



MyRun::MyRun(){
    G4SDManager* SDmananger = G4SDManager::GetSDMpointer();

    std::vector<G4String> detectorname;
    detectorname.push_back(G4String("PhantomSD"));
    G4cout << detectorname[0] << G4endl;
    
    
    G4String detName = detectorname[0];
        
    G4MultiFunctionalDetector* detector = (G4MultiFunctionalDetector*)
                                                (SDmananger->FindSensitiveDetector(detName));
    G4cout << detector->GetPrimitive(0) << G4endl;
    if(detector){
        G4VPrimitiveScorer* scorer = detector->GetPrimitive(0);
        G4String colName = scorer->GetName();
        G4String fColName = detName +"/"+ colName;
        G4int colID = SDmananger->GetCollectionID(fColName);

        if ( colID >= 0 ){
        G4cout << "++ "<<fColName<< " id " << colID
        << G4endl;
        collectionName.push_back(fColName);
        collectionID.push_back(colID);
        runMap.push_back(new G4THitsMap<G4double>(detName,colName));
        }else{
        G4cout << "** collection " << fColName << " not found. "
        << G4endl;
        };
        
    };
    

};

MyRun::~MyRun(){
   
     G4int nMap = runMap.size();
    for ( G4int i = 0; i < nMap; i++){
        if(runMap[i] ) runMap[i]->clear();
    }
    collectionName.clear();
    collectionID.clear();
    runMap.clear();
 
}


void MyRun::RecordEvent(const G4Event* event){
    numberOfEvent++;
    
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    if(!HCE) return;

    G4int ncol = collectionID.size();
    for(G4int i = 0; i < ncol; i++){
        G4THitsMap<G4double>* eventMap=0;
        if(collectionID[i] >= 0){
            eventMap = (G4THitsMap<G4double>*)(HCE->GetHC(collectionID[i]));
        }else{
        G4cout <<" Error evtMap Not Found "<< i << G4endl;
        };
        if(eventMap){
            *runMap[i] += *eventMap;
        };
    };

};


void MyRun::Merge(const G4Run* run){

    const MyRun* localRun = static_cast<const MyRun*>(run);
    

    G4int ncol = localRun->collectionID.size();
    for ( G4int i = 0; i < ncol ; i++ ){
        if ( localRun->collectionID[i] >= 0 ){
            *runMap[i] += *localRun->runMap[i];
        };
    
    };
    G4Run::Merge(run);
};

G4THitsMap<G4double>* MyRun::GetHitsMap(const G4String &detName, 
                                        const G4String &collectionName){
    G4String fullName = detName+"/"+collectionName;
    return GetHitsMap(fullName);
    
}

G4THitsMap<G4double>* MyRun::GetHitsMap(const G4String &fullName){
    G4int nCol = collectionID.size();
    for ( G4int i = 0; i < nCol; i++){

        if ( collectionName[i] == fullName ){
            return runMap[i];
        };
    };
    return NULL;
    
}



void MyRun::DumpAllScorer(){

  
  G4int n = GetNumeberOfHitsMap();
  
  for ( G4int i = 0; i < n ; i++ ){
    G4THitsMap<G4double>* runMap =GetHitsMap(i);
    if ( runMap ) {
      G4cout << " PrimitiveScorer RUN " 
             << runMap->GetSDname() <<","<< runMap->GetName() << G4endl;
      G4cout << " Number of entries " << runMap->entries() << G4endl;

    }
  }
}