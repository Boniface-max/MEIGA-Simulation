#include "G4RCTrackingAction.h"
#include "G4RCSimulator.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4TrackStatus.hh>
#include <G4OpticalPhoton.hh>

#include "SimData.h"
#include "Particle.h"

using namespace std;

G4RCTrackingAction::G4RCTrackingAction(Event &theEvent) :
	fEvent(theEvent) 
{

}

void
G4RCTrackingAction::PreUserTrackingAction(const G4Track* track)
{ 

}


void
G4RCTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  
}
