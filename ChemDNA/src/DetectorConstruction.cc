#include "DetectorConstruction.hh"
#include "ScoreSpecies.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager * man = G4NistManager::Instance();
    G4Material* water = man->FindOrBuildMaterial("G4_WATER");
    double world_sizeXYZ = 1. * kilometer;
    
    G4Box* solidWorld =
    new G4Box("World",
              0.5*world_sizeXYZ,
              0.5*world_sizeXYZ,
              0.5*world_sizeXYZ);
    
    G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,
                        water,
                        "World");
    
    G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //its position at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //checking overlaps
    
    G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(.5, 1.0, .5));
    worldVisAtt->SetVisibility(true);
    logicWorld->SetVisAttributes(worldVisAtt);
    
    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
    G4MultiFunctionalDetector* WaterMFDet = new G4MultiFunctionalDetector("WaterMFDet");
    
    //G4VPrimitiveScorer* primitiv = new G4PSEnergyDeposit("edep");
    G4VPrimitiveScorer* primitivSpecies = new ScoreSpecies("Species");
    
    //WaterMFDet->RegisterPrimitive(primitiv);
    WaterMFDet->RegisterPrimitive(primitivSpecies);
    
    G4SDManager::GetSDMpointer()->AddNewDetector(WaterMFDet);
    SetSensitiveDetector("World", WaterMFDet);
    
}
