#ifndef G4TowTrackingAction_h
#define G4TowTrackingAction_h 1

#include "Event.h"

#include <G4UserTrackingAction.hh>

class G4Track;

class G4TowTrackingAction : public G4UserTrackingAction {
	public:
		G4TowTrackingAction(Event& theEvent);
		virtual ~G4TowTrackingAction() { }

		virtual void PreUserTrackingAction(const G4Track*  track) override;
    virtual void PostUserTrackingAction(const G4Track* track) override;


  private:

  	Event& fEvent;

};

#endif

