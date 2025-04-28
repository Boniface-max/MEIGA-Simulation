// definition of the G4RCSteppingAction class

#ifndef G4RCSteppingAction_h
#define G4RCSteppingAction_h 1
#include <iostream>
#include "G4UserSteppingAction.hh"
#include "G4String.hh"
#include <fstream> //nuevo
#include "Event.h"


class G4RCDetectorConstruction;
class G4RCEventAction;

/* Stepping action class.
	 UserSteppingAction collects info at a step level
*/

class G4RCSteppingAction : public G4UserSteppingAction
{
	public:
		G4RCSteppingAction(G4RCEventAction* event, Event& theEvent);
		virtual ~G4RCSteppingAction();
		virtual void UserSteppingAction(const G4Step* step);

	private:
		G4RCEventAction* fEventAction;
		Event& fEvent;

		//std::vector<int> fBarsX1;
		//std::vector<int> fBarsY1;

//############# Nuevo
//	public:
//    		G4RCSteppingAction() {
        	// Abrir un archivo de registro
//        	logFile.open("registro_simulacion.txt");
 //   						}

//    	~G4RCSteppingAction() {
        	// Cerrar el archivo de registro al finalizar la simulación
//        	logFile.close();
//   						 }
//FIN
};

#endif
