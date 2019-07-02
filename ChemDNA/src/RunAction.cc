#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
: G4UserRunAction()
{
}

RunAction::~RunAction()
{}

G4Run* RunAction::GenerateRun()
{
    Run* run = new Run();
    return run;
}

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
    G4cout << "### Run " << run->GetRunID() << " starts." << G4endl;
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
    
  const Run* ChemDNARun = static_cast<const Run*>(run);
  G4double sumEdep = ChemDNARun->GetSumEdep();
    
  ScoreSpecies* masterScorer=
  dynamic_cast<ScoreSpecies*>(ChemDNARun->GetPrimitiveScorer());
  masterScorer->OutputAndClear();

  G4cout
     << "------------------------------------------------------------"
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << " " << sumEdep << " MeV deposit."
     << G4endl
     << "------------------------------------------------------------"
     << G4endl;
}

