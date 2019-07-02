#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"

#include "G4DNAChemistryManager.hh"
#include "G4MoleculeCounter.hh"
#include "G4Scheduler.hh"

#include "G4H2O.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction());
  G4DNAChemistryManager::Instance()->ResetCounterWhenRunEnds(false);
}

void ActionInitialization::Build() const
{
  G4MoleculeCounter::Use();
  G4MoleculeCounter::Instance()->DontRegister(G4H2O::Definition());
  
  if(G4Threading::IsMultithreadedApplication() == false)
  {
      G4DNAChemistryManager::Instance()->ResetCounterWhenRunEnds(false);
  }
    
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction());
  SetUserAction(new StackingAction());
}  
