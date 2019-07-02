#include "StackingAction.hh"
#include "G4StackManager.hh"
#include "G4DNAChemistryManager.hh"
#include "G4Track.hh"

StackingAction::StackingAction()
: G4UserStackingAction()
{}

StackingAction::~StackingAction()
{}

void StackingAction::NewStage()
{
  if(stackManager->GetNTotalTrack() == 0)
  {
    G4cout << "Physics stage ends" << G4endl;
    G4DNAChemistryManager::Instance()->Run();
  }
}

