
/// \file SYPEventAction.hh
/// \brief Definition of the SYPEventAction class

#ifndef SYPEventAction_h
#define SYPEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class SYPRunAction;

/// Event action class
///

class SYPEventAction : public G4UserEventAction
{
  public:
    SYPEventAction(SYPRunAction* runAction);
    virtual ~SYPEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
    SYPRunAction* fRunAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
