
/// \file SYPRunAction.cc
/// \brief Implementation of the SYPRunAction class

#include "SYPRunAction.hh"
#include "SYPPrimaryGeneratorAction.hh"
#include "SYPDetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSourceData.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPRunAction::SYPRunAction()
: G4UserRunAction()
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPRunAction::~SYPRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPRunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  //
  G4double edepLayer[12];
  G4double totalEdep = 0;
  G4double countPhotonSum[12];
  G4double totalCountPhoton = 0;
  memset(edepLayer,0,sizeof (edepLayer));
  memset(countPhotonSum,0,sizeof(countPhotonSum));
  G4int noflayer = 12, nofunits = 36;
  for ( G4int i = 0; i < noflayer; i++ )
  {
      for ( G4int j = 0; j < nofunits; j++ )
      {
          edepLayer[i] += fEdep[i][j];
          countPhotonSum[i] += countPhoton[i][j];
      }
      totalEdep += edepLayer[i];
      totalCountPhoton += countPhotonSum[i];
  }


  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------"
     << G4endl;
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------"
     << G4endl;
  }
    G4cout.setf(std::ios_base::scientific,std::ios_base::floatfield);
    G4cout
      << " Total energy deposit in chamber is: "
      << totalEdep << " MeV " << G4endl
      << " Total photon entered into chamber is: "
      << totalCountPhoton << G4endl
      << " Overall sensitivity is: "
      << (totalEdep/totalCountPhoton)*(4.24*1e14) << " e/Gy"
      << G4endl;

  for ( G4int i = 0; i < noflayer; i++ )
  {
      for( G4int j = 0; j < nofunits; j++ )
      {
          G4cout <<  " Sensitivity in Layer [" << i << "], Chamber [" << j <<"] is: "
          << fEdep[i][j]  << "     " << countPhoton[i][j] << "     " <<(fEdep[i][j]/countPhoton[i][j])*(4.24*1e14) << " e/Gy" << G4endl;
      }
  }

  //
  std::fstream dataFile;
  dataFile.open("sensitivity.txt",std::ios::app|std::ios::out);
  for ( G4int i = 0; i < noflayer; i++ )
  {
      for( G4int j = 0; j < nofunits; j++ )
      {
          dataFile <<(fEdep[i][j]/countPhoton[i][j])*(4.24*1e14) << G4endl;
      }
  }

  G4cout
  << G4endl
  << "------------------------------------------------------------"
  << G4endl
  << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPRunAction::AddEdep( G4double edep, G4int motherCopyNo, G4int copyNo )
{
  fEdep[motherCopyNo][copyNo] += edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

