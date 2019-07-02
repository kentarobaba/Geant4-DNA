#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4EmDNAChemistry.hh"
#include "G4EmDNAChemistry_option1.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option3.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option5.hh"
#include "G4EmDNAPhysics_option6.hh"
#include "G4EmDNAPhysics_option7.hh"
#include "G4EmDNAPhysics_option8.hh"
#include "G4PhysicsConstructorRegistry.hh"
#include "G4EmParameters.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList(),
fEmDNAPhysicsList(nullptr),fEmDNAChemistryList(nullptr)
,fPhysDNAName("")
{
    G4double currentDefaultCut = 1.*nanometer;
    G4ProductionCutsTable::GetProductionCutsTable()->
    SetEnergyRange(100*eV, 1*GeV);
    SetDefaultCutValue(currentDefaultCut);
    SetVerboseLevel(1);
    
    //RegisterConstructor("G4EmDNAPhysics");
    RegisterConstructor("G4EmDNAPhysics_option1");
    RegisterConstructor("G4EmDNAChemistry");
    
    G4EmParameters::Instance()->SetDNAeSolvationSubType(fRitchie1994eSolvation);
    G4EmParameters::Instance()->Dump();
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{
    if(fEmDNAPhysicsList)    { fEmDNAPhysicsList->ConstructParticle(); }
    if(fEmDNAChemistryList)  { fEmDNAChemistryList->ConstructParticle(); }
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    if(fEmDNAPhysicsList)    { fEmDNAPhysicsList->ConstructProcess(); }
    if(fEmDNAChemistryList)  { fEmDNAChemistryList->ConstructProcess(); }
}

void PhysicsList::RegisterConstructor(const G4String& name)
{
    if(name == fPhysDNAName) { return; }
    G4cout << "PhysicsList::RegisterConstructor: <" << name << ">"
    << G4endl;
    if(name == "G4EmDNAPhysics_option1") {
        delete fEmDNAPhysicsList;
        fEmDNAPhysicsList = new G4EmDNAPhysics(verboseLevel);
        fPhysDNAName = name;
    } else if(name == "G4EmDNAChemistry") {
        if(fEmDNAChemistryList) { return; }
        fEmDNAChemistryList = new G4EmDNAChemistry();
    } else {
        G4cout << "PhysicsList::RegisterConstructor: <" << name << ">"
        << " fails - name is not defined"
        << G4endl;
    }
}
