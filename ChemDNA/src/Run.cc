#include "Run.hh"
#include "RunAction.hh"
#include "ScoreSpecies.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"

#include <map>

Run::Run()
 : G4Run(),
fSumEdep(0),
fScorerRun(0){
    G4MultiFunctionalDetector* mfdet =  dynamic_cast<G4MultiFunctionalDetector*>
    (G4SDManager::GetSDMpointer()->FindSensitiveDetector("WaterMFDet"));
    G4int CollectionIDspecies =
    G4SDManager::GetSDMpointer()->GetCollectionID("WaterMFDet/Species");
    fScorerRun = mfdet->GetPrimitive(CollectionIDspecies);
}

Run::~Run()
{
}

void Run::RecordEvent(const G4Event* event)
{
  if(event->IsAborted()) return;

  G4int CollectionID =
  G4SDManager::GetSDMpointer()->GetCollectionID("WaterMFDet/Species");
    
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(!HCE) return;

  G4THitsMap<G4double>* evtMap =
  static_cast<G4THitsMap<G4double>*>(HCE->GetHC(CollectionID));

  std::map<G4int,G4double*>::iterator itr;
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++){
    G4double edep = *(itr->second);
    fSumEdep+=edep;
  }

  G4Run::RecordEvent(event);
}

void Run::Merge(const G4Run* aRun)
{
  if(aRun == this){
    return;
  }

  const Run* localRun = static_cast<const Run*>(aRun);
  fSumEdep += localRun->fSumEdep;
  
  ScoreSpecies* masterScorer =
  dynamic_cast<ScoreSpecies*>(this->fScorerRun);
  ScoreSpecies* localScorer =
  dynamic_cast<ScoreSpecies*>(localRun->fScorerRun);
  masterScorer->AbsorbResultsFromWorkerScorer(localScorer);
    
  G4Run::Merge(aRun);
}
