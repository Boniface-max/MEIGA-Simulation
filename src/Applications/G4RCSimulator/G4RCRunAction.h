// definition of the G4RCRunAction class

#ifndef G4RCRunAction_h
#define G4RCRunAction_h 1
// Geant4 headers
#include "globals.hh"
#include "G4UserRunAction.hh"
#include "g4root.hh"
// Meiga headers
#include "Event.h"
#include "G4RCSimulator.h"
// C++ headers
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


class G4Run;


class G4RCRunAction : public G4UserRunAction
{
	public:
		G4RCRunAction(Event& theEvent);
		virtual ~G4RCRunAction();

		virtual void BeginOfRunAction(const G4Run* aRun);
		virtual void EndOfRunAction(const G4Run* aRun);

	private:

		std::ofstream fOutFile;
		Event& fEvent;


	friend class G4RCSimulator;

};
#endif 
