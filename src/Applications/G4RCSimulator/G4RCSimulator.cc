/* 
	
	Application for simulating a muon telescope

	author: alvaro taboada
	date: 23 Jan 2023

 */

// Headers of this particular application
#include "G4RCSimulator.h"
#include "G4RCDetectorConstruction.h"
#include "G4MPrimaryGeneratorAction.h"
#include "G4RCEventAction.h"
#include "G4RCRunAction.h"
#include "G4RCTrackingAction.h"
#include "G4RCSteppingAction.h"
#include "G4RCPhysicsList.h" 
// Geant4 headers
#include "FTFP_BERT.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

// Framework libraries
#include "ConfigManager.h"
#include "CorsikaUtilities.h"
#include "Event.h"
#include "SimData.h"
#include "OptDeviceSimData.h"
#include "Detector.h"
#include "OptDevice.h"
#include "G4MPhysicsList.h"
#include "DataWriter.h"

using namespace std;

Particle G4RCSimulator::currentParticle;
G4RCSimulator* fG4RCSimulator;
string fCfgFile;

G4RCSimulator::G4RCSimulator()
{

}

namespace 
{
	void ProgramUsage() 
	{
		cerr << " Program Usage: " << endl;
		cerr << " ./G4RCSimulator [ -c ConfigFile.json ] " << endl;
	}

}


int main(int argc, char** argv) 
{

	if (argc < 3) {
		ProgramUsage();
		throw invalid_argument("[ERROR] G4RCSimulator::main: A configuration file is needed!");
	}

	for (int i=1; i<argc; i=i+2) {
		string sarg(argv[i]);
		if (sarg == "-c")
			fCfgFile = argv[i+1];
	}

	// for program time calculation
	time_t start, end;
	time(&start);

	fG4RCSimulator = new G4RCSimulator();
	// Create Event object
	Event theEvent;
	fG4RCSimulator->Initialize(theEvent, fCfgFile);
	fG4RCSimulator->RunSimulation(theEvent);
	/*************************************************
		
		Geant4 simulation ended here!
		What happens next is up to you =)

	**************************************************/
	fG4RCSimulator->WriteEventInfo(theEvent);
	time(&end);
	
	// Calculating total time taken by the program.
	double time_taken = double(end - start);
	cout << "[INFO] G4RCSimulator: Time taken by program is : " << fixed
			 << time_taken << setprecision(5);
	cout << " sec " << endl;

	return 0;

}

void
G4RCSimulator::Initialize(Event& theEvent, string cfgFile)
{

	cout << "[INFO] G4RCSimulator::Initialize" << endl;
	cout << "[INFO] G4RCSimulator::Initialize: Reading configuration file " << cfgFile << endl;

	// Fill Event object from configuration file
	// Read Simulation configuration
	theEvent = ConfigManager::ReadConfigurationFile(cfgFile);
	// get simulation simulation settings
	const Event::Config &cfg = theEvent.GetConfig();
	ConfigManager::PrintConfig(cfg);
	// Read Detector Configuration
	ConfigManager::ReadDetectorList(cfg.fDetectorList, theEvent);
	
}            


bool
G4RCSimulator::RunSimulation(Event& theEvent)
{

	cout << "[INFO] G4RCSimulator::RunSimulation" << endl;

	const Event::Config &cfg = theEvent.GetConfig();
	SimData& simData = theEvent.GetSimData();
	const unsigned int NumberOfParticles = simData.GetTotalNumberOfParticles();
	cout << "[INFO] G4RCSimulator::RunSimulation: Number of particles to be simulated = " << NumberOfParticles << endl;

	
	if (!NumberOfParticles) {
		cerr << "[ERROR] G4RCSimulator::RunSimulation: No Particles in the Event! Exiting." << endl;
		return false;
	}
	
	
	/***************

		Geant4 Setup    

	*****************/

	G4long myseed = time(NULL);
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeed(myseed);
	cout << "Seed for random generation: " << myseed << endl;

	G4VisManager* fVisManager = nullptr;
	
	// construct the default run manager
	auto fRunManager = G4RunManagerFactory::CreateRunManager();

	// set mandatory initialization classes
	auto fDetConstruction = new G4RCDetectorConstruction(theEvent);
	fRunManager->SetUserInitialization(fDetConstruction);
	
	fRunManager->SetUserInitialization(new G4RCPhysicsList(fPhysicsName));  
 
	G4MPrimaryGeneratorAction *fPrimaryGenerator = new G4MPrimaryGeneratorAction(theEvent);
	fRunManager->SetUserAction(fPrimaryGenerator);
	
	G4RCRunAction *fRunAction = new G4RCRunAction(theEvent);
	fRunManager->SetUserAction(fRunAction);
	
	G4RCEventAction *fEventAction = new G4RCEventAction(theEvent);
	fRunManager->SetUserAction(fEventAction);

	fRunManager->SetUserAction(new G4RCTrackingAction(theEvent));

	G4RCSteppingAction *fSteppingAction = new G4RCSteppingAction(fEventAction, theEvent);
	fRunManager->SetUserAction(fSteppingAction);
	

	// initialize G4 kernel
	fRunManager->Initialize();

	// initialize visualization
	if ((cfg.fGeoVis || cfg.fTrajVis) && !fVisManager)
		fVisManager = new G4VisExecutive;

	// get the pointer to the UI manager and set verbosities
	G4UImanager* fUImanager = G4UImanager::GetUIpointer();
	switch (cfg.fVerbosity) {
		case 1:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 0");
			fUImanager->ApplyCommand("/tracking/verbose 0");
			break;
		case 2:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 1");
			fUImanager->ApplyCommand("/tracking/verbose 0");
			break;
		case 3:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 1");
			fUImanager->ApplyCommand("/tracking/verbose 1");
			break;
		default:
			fUImanager->ApplyCommand("/run/verbose 0");
			fUImanager->ApplyCommand("/event/verbose 0");
			fUImanager->ApplyCommand("/tracking/verbose 0");
		}
	
	if (cfg.fGeoVis || cfg.fTrajVis) {
		fVisManager->Initialize();
		fUImanager->ApplyCommand(("/vis/open " + fRenderFile).c_str());
		fUImanager->ApplyCommand("/vis/scene/create");
		fUImanager->ApplyCommand("/vis/sceneHandler/attach");
		fUImanager->ApplyCommand("/vis/scene/add/volume");
		fUImanager->ApplyCommand("/vis/scene/add/axes");
		fUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 0. 0.");
		fUImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 0");
		fUImanager->ApplyCommand("/vis/viewer/zoom 1");
		fUImanager->ApplyCommand("/vis/viewero/set/style/wireframe");
		fUImanager->ApplyCommand("/vis/drawVolume");
		fUImanager->ApplyCommand("/vis/scene/notifyHandlers");
		fUImanager->ApplyCommand("/vis/viewer/update");
	}
	if (cfg.fTrajVis) {
		fUImanager->ApplyCommand("/tracking/storeTrajectory 1");
		fUImanager->ApplyCommand("/vis/scene/add/trajectories");
		fUImanager->ApplyCommand("/vis/filtering/trajectories/create/particleFilter");
		// for debugging purposes, gammas are not drawn
		fUImanager->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add opticalphoton");
		fUImanager->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/invert true");
	}


	// loop over particle vector
	for (auto it = simData.GetParticleVector().begin(); it != simData.GetParticleVector().end(); ++it) {
		G4RCSimulator::currentParticle = *it;
		simData.SetCurrentParticle(*it);
		// Run simulation
		fRunManager->BeamOn(1);

	}


	delete fVisManager;
	delete fRunManager;

	cout << "[INFO] G4RCSimulator::RunSimulation: Geant4 Simulation ended successfully. " << endl;


	return true;

}


void
G4RCSimulator::WriteEventInfo(Event& theEvent)
{

	cout << "[INFO] G4RCSimulator::WriteEventInfo" << endl;
	DataWriter::FileWriter(theEvent);	

	
	

	return;

}
