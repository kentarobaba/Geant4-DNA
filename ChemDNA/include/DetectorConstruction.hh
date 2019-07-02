#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPrimitiveScorer.hh"
#include "globals.hh"
#include "ScoreSpecies.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
};

#endif

