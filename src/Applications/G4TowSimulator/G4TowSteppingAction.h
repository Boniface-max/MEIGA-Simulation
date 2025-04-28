// definition of the G4TowSteppingAction class

#ifndef G4TowSteppingAction_h
#define G4TowSteppingAction_h 1
#include <iostream>
#include "G4UserSteppingAction.hh"
#include "G4String.hh"
#include <fstream> //nuevo
#include "Event.h"


class G4TowDetectorConstruction;
class G4TowEventAction;

/* Stepping action class.
	 UserSteppingAction collects info at a step level
*/

class G4TowSteppingAction : public G4UserSteppingAction
{
	public:
		G4TowSteppingAction(G4TowEventAction* event, Event& theEvent);
		virtual ~G4TowSteppingAction();
		virtual void UserSteppingAction(const G4Step* step);

	private:
		G4TowEventAction* fEventAction;
		Event& fEvent;

		//std::vector<int> fBarsX1;
		//std::vector<int> fBarsY1;

//############# Nuevo
//	public:
//    		G4TowSteppingAction() {
        	// Abrir un archivo de registro
//        	logFile.open("registro_simulacion.txt");
 //   						}

//    	~G4TowSteppingAction() {
        	// Cerrar el archivo de registro al finalizar la simulación
//        	logFile.close();
//   						 }
//FIN
};

#endif
