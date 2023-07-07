
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
: G4UserRunAction(),
  fEdep(0.)
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
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

  G4double edep  = fEdep.GetValue();

  G4double countSum[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  G4double  countPhotonSum[12] = { 0,0,0,0,0,0,0,0,0,0,0,0};
  G4double sum1 = 0, sum2 = 0;
  G4int noflayer = 12, nofunits = 36;

  for ( G4int i = 0; i < noflayer; i++ )
  {
      for( G4int j = 0; j < nofunits; j++ )
      {
          countSum[i] += count[i][j];
          countPhotonSum[i] += countPhoton[i][j];
      }
  }

  for ( G4int i = 0; i < noflayer; i++ )
  {
      sum1 += countSum[i];
      sum2 += countPhotonSum[i];
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
  
  G4cout
     << " Number of photon is: "
     << sum2 <<" "
     << " Number of secondary e- is: "
     << sum1 <<" "
     << G4endl
     << " Total detection efficiency is: "
     << sum1*100/sum2 << " %"
     << G4endl;

     for( G4int i = 0; i < 12; i++ )
     {
         for ( G4int j = 0; j < 36; j++ )
         {
             G4cout <<" Detection Efficiency is in Layer [" << i << "] " << "Chamber[" << j << "] is: " << count[i][j] << " " << countPhoton[i][j] << " " << (count[i][j])*100/(countPhoton[i][j]) << " % " << G4endl;
         }
         G4cout<<" Detection Efficiency is in Layer [" << i << "]  is: " << countSum[i] << " " << countPhotonSum[i] << " " << (countSum[i]*100)/(countPhotonSum[i]) << " %" <<G4endl;
     }

    std::fstream dataFile;
    dataFile.open("DetectionEfficienvy.txt",std::ios::app|std::ios::out);
    for( G4int i = 0; i < 12; i++ )
    {
        for ( G4int j = 0; j < 36; j++ )
        {
            dataFile << (count[i][j])*100/(countPhoton[i][j]) << G4endl;
        }
        dataFile <<" Detection Efficiency is in Layer [" << i << "] is: " << countSum[i] << " " << countPhotonSum[i] << " " << (countSum[i]*100)/(countPhotonSum[i]) << " %" <<G4endl;
    }

//     G4cout
//     << " Overall detection efficiency is: " << countSum*100/nofEvents <<" %"
//     << G4endl
//     << " Sensitivity is: "
//    << 3648.4*edep/nofEvents << " pA/(cGy/h)"
//     << G4endl
     G4cout
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPRunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

