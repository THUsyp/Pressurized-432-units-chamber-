
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
//  G4String postProcessName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    G4Track* thisTrack = step->GetTrack();
    G4String particleName = thisTrack->GetParticleDefinition()->GetParticleName();
    G4String creatorProcess = thisTrack->GetCreatorModelName();
  //
  G4int nofunits = 36;
  G4int noflayer = 12;
    if(particleName=="e-"&&creatorProcess=="eIoni"&&(volumeName=="EC"||volumeName=="Partition"||volumeName=="Half_HE"||volumeName=="side"))
    {
        thisTrack->SetTrackStatus(fKillTrackAndSecondaries);
    }
  if( volumeName == "Chamber" )
  {
      G4int parentID = thisTrack->GetParentID();
      G4int copyNo = touchableHandle->GetCopyNumber();
      G4int motherCopyNo = touchableHandle->GetCopyNumber(1);
      G4ThreeVector initial_pos = step->GetPreStepPoint()->GetPosition();
      auto posZ = initial_pos.getZ();

      if( parentID == 0 && fabs(posZ-(1e-7)) <= 1e-9 )
      {
          fRunAction->countPhoton[motherCopyNo][copyNo]++;
      }
      if( particleName == "e-" && creatorProcess != "eIoni" )
      {
          fRunAction->count[motherCopyNo][copyNo]++;
      }

      if(particleName=="gamma"&&creatorProcess=="eBrem")
      {
          thisTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }

      if(particleName=="e-")
      {
          thisTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }
  }



}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

