#ifndef StackingAction_h
#define StackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class StackingAction : public G4UserStackingAction
{    
 
public:
    StackingAction();
    virtual ~StackingAction();
    virtual void NewStage();
};

#endif
