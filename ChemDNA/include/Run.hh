#ifndef Run_h
#define Run_h 1

#include "ScoreSpecies.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "globals.hh"

class G4VPrimitiveScorer;
class DetectorConstruction;
class Run : public G4Run
{
public:
  Run();
  virtual ~Run();
  
  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);
  
  G4double GetSumEdep()   const { return fSumEdep; }
  G4VPrimitiveScorer* GetPrimitiveScorer() const { return fScorerRun;}
  
private:
  G4double fSumEdep;
  G4VPrimitiveScorer* fScorerRun;
};

#endif
