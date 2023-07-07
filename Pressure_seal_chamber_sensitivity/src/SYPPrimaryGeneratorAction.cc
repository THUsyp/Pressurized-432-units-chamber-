
/// \file SYPPrimaryGeneratorAction.cc
/// \brief Implementation of the SYPPrimaryGeneratorAction class

#include "SYPPrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPPrimaryGeneratorAction::SYPPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(3*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector (0,0,5000*mm));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPPrimaryGeneratorAction::~SYPPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SYPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // this function is called at the beginning of each event
  //
  G4double theta = atan(55.4/5000.);
  G4double alpha = atan(29.25/5000.);
  G4double momentum_sizeX = 5000*tan(2*theta*(G4UniformRand()-0.5));
  G4double momentum_sizeY = 5000*tan(2*alpha*(G4UniformRand()-0.5));
  G4double momentum_sizeZ = -5000;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(momentum_sizeX,momentum_sizeY,momentum_sizeZ));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

