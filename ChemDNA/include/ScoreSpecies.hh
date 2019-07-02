#ifndef CHEM5_ScoreSpecies_h
#define CHEM5_ScoreSpecies_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <set>

class G4MolecularConfiguration;

class ScoreSpecies : public G4VPrimitiveScorer
{
public:
  ScoreSpecies(G4String name, G4int depth=0);

  virtual ~ScoreSpecies();
  
  inline void AddTimeToRecord(double time){
    fTimeToRecord.insert(time);
  }
  inline void ClearTimeToRecord(){
    fTimeToRecord.clear();
  }
  inline int GetNumberOfRecordedEvents() const{
    return fNEvent;
  }

  void ASCII();
  
  struct SpeciesInfo
  {
    SpeciesInfo()
    {
      fNumber = 0;
      fG = 0.;
      fG2 = 0.;
    }
    SpeciesInfo(const SpeciesInfo& right)
    {
      fNumber = right.fNumber;
      fG = right.fG;
      fG2 = right.fG2;
    }
    SpeciesInfo& operator=(const SpeciesInfo& right) // A = B
    {
      if(&right == this) return *this;
      fNumber = right.fNumber;
      fG = right.fG;
      fG2 = right.fG2;
      return *this;
    }
    int fNumber;
    double fG;
    double fG2;
  };
  
private:
  typedef G4MolecularConfiguration* Species;
  typedef std::map<Species, SpeciesInfo>  InnerSpeciesMap;
  typedef std::map<double, InnerSpeciesMap> SpeciesMap;
  SpeciesMap fSpeciesInfoPerTime;

  std::set<G4double> fTimeToRecord;
  
  int fNEvent; // number of processed events
  double fEdep; // total energy deposition

protected:
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

public:
  virtual void Initialize(G4HCofThisEvent*);
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();
  virtual void AbsorbResultsFromWorkerScorer(G4VPrimitiveScorer* );
  virtual void OutputAndClear();
  
  SpeciesMap GetSpeciesInfo() {return fSpeciesInfoPerTime;}

private:
  G4int fHCID;
  G4THitsMap<G4double>* fEvtMap;
};
#endif
