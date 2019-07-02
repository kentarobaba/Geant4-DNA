#include "ScoreSpecies.hh"

#include "G4UnitsTable.hh"
#include <G4MolecularConfiguration.hh>
#include <G4Molecule.hh>
#include <G4MoleculeCounter.hh>
#include "G4Event.hh"
#include <G4SystemOfUnits.hh>
#include <globals.hh>
#include <G4EventManager.hh>
#include "g4analysis_defs.hh"
#include <iomanip>

using namespace std;

ScoreSpecies::ScoreSpecies(G4String name, G4int depth)
: G4VPrimitiveScorer(name,depth),
  fEdep(0),
  fHCID(-1),
  fEvtMap(0)
{
  fNEvent = 0;
  G4double tMin = 1.0 * CLHEP::picosecond;
  G4double tMax = 999999 * CLHEP::picosecond;
  G4double tLogMin = log10(tMin);
  G4double tLogMax = log10(tMax);
  G4int tBins = 50;
  for ( int i = 0; i <= tBins; i++ ){
      AddTimeToRecord(std::pow(10., tLogMin + i*(tLogMax-tLogMin)/tBins));
  }
  fEdep = 0;
}

ScoreSpecies::~ScoreSpecies()
{;}

G4bool ScoreSpecies::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if ( edep == 0. ) return FALSE;

  edep = aStep->GetTotalEnergyDeposit();
  G4int index = GetIndex(aStep);
  fEvtMap->add(index,edep);
  fEdep+=edep;

  return TRUE;
}

void ScoreSpecies::Initialize(G4HCofThisEvent* HCE)
{
  fEvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),GetName());
  if(fHCID < 0){fHCID = GetCollectionID(0);}
  HCE->AddHitsCollection(fHCID, (G4VHitsCollection*)fEvtMap);
}


void ScoreSpecies::EndOfEvent(G4HCofThisEvent*)
{
  if(G4EventManager::GetEventManager()->GetConstCurrentEvent()->IsAborted())
  {
    fEdep = 0.;
    G4MoleculeCounter::Instance()->ResetCounter();
    return;
  }
  
  auto species = G4MoleculeCounter::Instance()->GetRecordedMolecules();
  
  for(auto molecule: *species)
  {
    for(auto time_mol: fTimeToRecord)
    {
      double n_mol = G4MoleculeCounter::Instance()->GetNMoleculesAtTime(molecule,time_mol);

      SpeciesInfo& molInfo = fSpeciesInfoPerTime[time_mol][molecule];
      molInfo.fNumber += n_mol;
      double gValue = (n_mol/(fEdep/eV)) * 100.;
      molInfo.fG += gValue;
      molInfo.fG2 += gValue*gValue;
    }
  }
  ++fNEvent;
  fEdep = 0.;
  G4MoleculeCounter::Instance()->ResetCounter();
}

void
ScoreSpecies::AbsorbResultsFromWorkerScorer(G4VPrimitiveScorer* workerScorer)
{
  ScoreSpecies* right =
  dynamic_cast<ScoreSpecies*>(dynamic_cast<G4VPrimitiveScorer*>(workerScorer));

  if(right == 0)
  {
    return;
  }
  if(right == this)
  {
    return;
  }

  SpeciesMap::iterator it_map1 = right->fSpeciesInfoPerTime.begin();
  SpeciesMap::iterator end_map1 = right->fSpeciesInfoPerTime.end();

  for(; it_map1 != end_map1; ++it_map1)
  {
    InnerSpeciesMap& map2 = it_map1->second;
    InnerSpeciesMap::iterator it_map2 = map2.begin();
    InnerSpeciesMap::iterator end_map2 = map2.end();

    for(; it_map2 != end_map2; ++it_map2)
    {
      SpeciesInfo& molInfo =
      fSpeciesInfoPerTime[it_map1->first][it_map2->first] ;
      molInfo.fNumber  += it_map2->second.fNumber;
      molInfo.fG += it_map2->second.fG;
      molInfo.fG2 += it_map2->second.fG2;
    }
  }
  
  fNEvent += right->fNEvent;
  right->fNEvent = 0;
  right->fEdep = 0.;
}

void ScoreSpecies::clear()
{
  fEvtMap->clear();
  fNEvent = 0;
  fEdep = 0;
  fSpeciesInfoPerTime.clear();
}

void ScoreSpecies::DrawAll()
{;}

void ScoreSpecies::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of events " << fNEvent << G4endl;
   G4cout << " Number of energy deposition recorded "
          << fEvtMap->entries() << G4endl;
  
  for(auto itr : *fEvtMap->GetMap()) {
     G4cout << "  copy no.: " << itr.first
     << "  energy deposit: "
     << *(itr.second)/GetUnitValue()
     << " [" << GetUnit()<<"]"
     << G4endl;
   }
}

void ScoreSpecies::ASCII()
{
  std::ofstream out("Species.txt");
  if(!out) return;

  out << "# Time [ps]  G-value (/100 eV)  RMS   Molecule" << G4endl;

  std::map<G4String, std::map<G4double, std::pair<G4double,G4double>>> mol;
 
  for(auto it_map1: fSpeciesInfoPerTime)
  {
    InnerSpeciesMap& map2 = it_map1.second;
    G4double time = it_map1.first/ps;
    for(auto it_map2: map2)
    {
      G4double G = it_map2.second.fG;
      G4double G2 = it_map2.second.fG2;
      G4double N = fNEvent;
      G /= N;
      G2 = std::sqrt( N/(N-1) * ( G2/N - G*G) );
      mol[it_map2.first->GetName()][time]=std::make_pair(G,G2);
    }
  }

  for ( auto it1 : mol )
    for ( auto it2 : it1.second )
      out << std::setw(12) << it2.first << std::setw(12) << it2.second.first 
          << std::setw(12) << it2.second.second << std::setw(12) 
          << std::setw(12) << it1.first << G4endl; 

  out.close();
}

void ScoreSpecies::OutputAndClear()
{
  if(G4Threading::IsWorkerThread()) return;

  ASCII();
  fNEvent = 0;
  fSpeciesInfoPerTime.clear();
}

