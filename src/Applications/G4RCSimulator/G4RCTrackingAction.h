#ifndef G4RCTrackingAction_h
#define G4RCTrackingAction_h 1

#include "Event.h"

#include <G4UserTrackingAction.hh>

class G4Track;

class G4RCTrackingAction : public G4UserTrackingAction {
	public:
		G4RCTrackingAction(Event& theEvent);
		virtual ~G4RCTrackingAction() { }

		virtual void PreUserTrackingAction(const G4Track*  track) override;
    virtual void PostUserTrackingAction(const G4Track* track) override;


  private:

  	Event& fEvent;

};

#endif

