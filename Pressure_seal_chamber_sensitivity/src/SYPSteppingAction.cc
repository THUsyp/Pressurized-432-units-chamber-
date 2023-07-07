
/// \file SYPSteppingAction.cc
/// \brief Implementation of the SYPSteppingAction class

#include "SYPSteppingAction.hh"
#include "SYPEventAction.hh"
#include "SYPRunAction.hh"
#include "SYPDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "math.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPSteppingAction::SYPSteppingAction(SYPRunAction* fRunAction)
: G4UserSteppingAction(),
  fRunAction(fRunAction)
  //ScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPSteppingAction::~SYPSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPSteppingAction::UserSteppingAction(const G4Step* step)
{
  //
  G4TouchableHandle touchableHandle = step->GetPreStepPoint()->GetTouchableHandle();
  G4LogicalVolume* volume 
    = touchableHandle->GetVolume()->GetLogicalVolume();
    G4String volumeName = volume->GetName();
    G4Track* thisTrack = step->GetTrack();
    G4String creatorProcess = thisTrack->GetCreatorModelName();
    G4String particleName = thisTrack->GetParticleDefinition()->GetParticleName();

    if(particleName=="e-"&&creatorProcess=="eIoni"&&(volumeName=="Half_HE"||volumeName=="EC"||volumeName=="side"||volumeName=="Partition"))
    {
        thisTrack->SetTrackStatus(fKillTrackAndSecondaries);
    }
//  G4String postProcessName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  //

  if( volumeName == "Chamber" )
  {
      G4String postProcessName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      G4int copyNo = touchableHandle->GetCopyNumber();
      G4int motherCopyNo = touchableHandle->GetCopyNumber(1);
      G4ThreeVector initial_pos = step->GetPreStepPoint()->GetPosition();

      G4int parentID = thisTrack->GetParentID();
      G4double posZ = step->GetPreStepPoint()->GetPosition().getZ();
      if( parentID == 0 && fabs(posZ-(1e-7)) <= 1e-9 )
      {
          fRunAction->countPhoton[motherCopyNo][copyNo]++;
      }
      if( particleName=="e-" )
      {
          G4double edep = step->GetTotalEnergyDeposit();
          fRunAction->AddEdep(edep,motherCopyNo,copyNo);
      }
      if(particleName=="gamma"&&creatorProcess=="eBrem")
      {
          thisTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

