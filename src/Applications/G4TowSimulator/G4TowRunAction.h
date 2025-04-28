// definition of the G4TowRunAction class

#ifndef G4TowRunAction_h
#define G4TowRunAction_h 1
// Geant4 headers
#include "globals.hh"
#include "G4UserRunAction.hh"
#include "g4root.hh"
// Meiga headers
#include "Event.h"
#include "G4TowSimulator.h"
// C++ headers
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


class G4Run;


class G4TowRunAction : public G4UserRunAction
{
	public:
		G4TowRunAction(Event& theEvent);
		virtual ~G4TowRunAction();

		virtual void BeginOfRunAction(const G4Run* aRun);
		virtual void EndOfRunAction(const G4Run* aRun);

	private:

		std::ofstream fOutFile;
		Event& fEvent;


	friend class G4TowSimulator;

};
#endif 
