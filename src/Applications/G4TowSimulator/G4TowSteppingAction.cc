// implementation of the G4TowSteppingAction class
#include "G4TowSimulator.h"
#include "G4TowSteppingAction.h"
#include "G4TowDetectorConstruction.h"
#include "G4TowEventAction.h"
#include "G4TowTrackingAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

#include "SimData.h"
#include "OptDeviceSimData.h"
#include "Detector.h"

#include <G4OpticalPhoton.hh>

using namespace std;

G4TowSteppingAction::G4TowSteppingAction(G4TowEventAction* G4event, Event& theEvent)
	: G4UserSteppingAction(),
		fEventAction(G4event),
    fEvent(theEvent)
{

	if (fEvent.GetSimData().GetSimulationMode() == SimData::SimulationMode::eFast) {
		cout << "[INFO] G4TowSteppingAction::G4TowSteppingAction: Running Simulation in Fast mode." << endl;
		cout << "[INFO] G4TowSteppingAction::G4TowSteppingAction: Optical photons will be killed! " << endl;
		
	}

}

G4TowSteppingAction::~G4TowSteppingAction()
{

}

void
G4TowSteppingAction::UserSteppingAction(const G4Step* step)
{

	G4Track *track = step->GetTrack();


//########Nuevo

    // Obtener la posición z actual de la partícula
//    G4ThreeVector position = track->GetPosition();
//    G4double z = position.z();

    // Definir la altura objetivo en la que deseas registrar las partículas
//    G4double altura_objetivo = 10.0;  // Cambiar a la altura deseada

//    if (z >= altura_objetivo) {
        // Obtener el tipo de partícula
//        G4String particleName = track->GetDefinition()->GetParticleName();
        
        // Obtener la energía de la partícula
//        G4double energy = track->GetKineticEnergy();
        
        // Registrar la información en el archivo o donde desees
        // Aquí puedes incluir la lógica para registrar los datos
        // por ejemplo, imprimirlos en la consola o escribir en un archivo
//        cout << "Partícula: " << particleName << ", Energía: " << energy << " MeV" << endl;
        
        // Puedes decidir si deseas continuar la propagación de la partícula o no
        // track->SetTrackStatus(fStopAndKill);  // Para detener y eliminar la partícula

//	}
//#########nuevo


	// if fast mode activated, kill optical photons
	if (fEvent.GetSimData().GetSimulationMode() == SimData::SimulationMode::eFast) {
		if (track->GetParticleDefinition() == G4OpticalPhoton::OpticalPhoton())
			track->SetTrackStatus(fStopAndKill);





	}

}
