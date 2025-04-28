#include "G4TowTrackingAction.h"
#include "G4TowSimulator.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4TrackStatus.hh>
#include <G4OpticalPhoton.hh>

#include "SimData.h"
#include "Particle.h"

using namespace std;

G4TowTrackingAction::G4TowTrackingAction(Event &theEvent) :
	fEvent(theEvent) 
{

}

void
G4TowTrackingAction::PreUserTrackingAction(const G4Track* track)
{ 

}


void
G4TowTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  
}
