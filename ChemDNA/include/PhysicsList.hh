#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class G4EmDNAChemistry;

class PhysicsList: public G4VModularPhysicsList
{
public:
    explicit PhysicsList();
    ~PhysicsList() override;
    
    void ConstructParticle() override;
    void ConstructProcess() override;
    
    void RegisterConstructor(const G4String& name);
    
private:
    G4VPhysicsConstructor*    fEmDNAPhysicsList;
    G4EmDNAChemistry*         fEmDNAChemistryList;
    G4String                  fPhysDNAName;
};
#endif
