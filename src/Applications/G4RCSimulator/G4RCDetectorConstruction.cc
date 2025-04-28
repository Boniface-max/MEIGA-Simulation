// implementation of the G4RCDetectorConstruction class
#include <iostream>

#include "G4RCDetectorConstruction.h"
#include "Materials.h"

#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

using namespace std;


G4RCDetectorConstruction::G4RCDetectorConstruction(Event& theEvent) : 
	G4VUserDetectorConstruction(),
	fEvent(theEvent)
{ 
}

G4RCDetectorConstruction::~G4RCDetectorConstruction() 
	{ }

G4VPhysicalVolume*
G4RCDetectorConstruction::CreateDetector()
{

	CreateWorld();
	//CreateGround();
	PlaceDetector(fEvent);
	return physWorld;
}

void
G4RCDetectorConstruction::CreateWorld()
{

	// size definitions
	const Event::Config &cfg = fEvent.GetConfig();	

	// double fGroundSizeX = 5 * CLHEP::m;
	// double fGroundSizeY = 5 * CLHEP::m;
	// double fGroundSizeZ = 1 * CLHEP::m;

	double fWorldSizeX = 10 * CLHEP::m;
	double fWorldSizeY = 14 * CLHEP::m;
	double fWorldSizeZ = 24 * CLHEP::m;

	solidWorld  = new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);
	logicWorld = new G4LogicalVolume(solidWorld, Materials().Air, "World");
	physWorld  =  new G4PVPlacement(nullptr, G4ThreeVector(), "World", logicWorld, 0, false, 0, fCheckOverlaps);




//#####
//###################--------OBJETIVO CON INTERFASE------------#################################
        // A PARTIR DE AQUÌ EDITO - PEDRO ARANGO EDIT

	//desde aquì mi ediciòn (Pedro)
        double innerRadius = 0 * CLHEP::cm;
        double outerRadius = 300 * CLHEP::cm;
        double startAngle =0;  // Starting angle (usually 0)
        double spanningAngle = 360; // Spanning angle in degrees (a full circle)
        G4RotationMatrix* rotationCyl = new G4RotationMatrix();
        rotationCyl->rotateX(90 * deg);// aquì se coloca la orientaciòn
	double Dist = 1000 * CLHEP::cm;// distancia horizontal a la cual està el centro del cilindro
        double CoorYc = -200 * CLHEP::cm;// coordenada vertical del centro de la base del cilindro
        double HeightCyl01 = 150 * CLHEP::cm;
        double HeightCyl02 = 100 * CLHEP::cm;
        double HeightCyl03 = 150 * CLHEP::cm;
     //   double HeightCyl04 = 30 * CLHEP::cm;
     //   double HeightCyl05 = 200 * CLHEP::cm;
     //   double HeightCyl06 = 20 * CLHEP::cm;
     //   double HeightCyl07 = 200 * CLHEP::cm;
        double HeightCyl =HeightCyl01+HeightCyl02+HeightCyl03; // +HeightCyl04+HeightCyl05+HeightCyl06+HeightCyl07;
        // G4Colour customColor1(0.5, 0.5, 0.0); // Olive


        //Cilindro 01 - Agua
//	double CoorY01 =CoorYc+HeightCyl-HeightCyl01/2;
//	solidBrick01=new G4Tubs("LeadBrick01", innerRadius, outerRadius,HeightCyl01/2 , startAngle, spanningAngle);
//        G4VisAttributes Apar01(G4Colour::Blue());
//	logicBrick01 = new G4LogicalVolume(solidBrick01, Materials().Water, "LeadBrick01");
//	logicBrick01->SetVisAttributes(Apar01);
//        G4ThreeVector positionCyl01(0.,CoorY01 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
//     	physBrick01 = new G4PVPlacement(rotationCyl,positionCyl01, logicBrick01, "LeadBrick01", logicWorld, false, 0, cfg.fCheckOverlaps);
      
        //Cilindro 02 - Plato de Destilaciòn
//	double CoorY02 =CoorY01-HeightCyl01/2-HeightCyl02/2;
//        solidBrick02=new G4Tubs("LeadBrick02", innerRadius, outerRadius,HeightCyl02/2 , startAngle, spanningAngle);
//        G4VisAttributes Apar02(G4Colour::Cyan());
//        logicBrick02 = new G4LogicalVolume(solidBrick02, Materials().Lead, "LeadBrick02");
//        logicBrick02->SetVisAttributes(Apar02);
//        G4ThreeVector positionCyl02(0.,CoorY02 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
//        physBrick02 = new G4PVPlacement(rotationCyl,positionCyl02, logicBrick02, "LeadBrick02", logicWorld, false, 0, cfg.fCheckOverlaps);

        //Cilindro 03 - Agua
//	double CoorY03 =CoorY02-HeightCyl02/2-HeightCyl03/2;
//        solidBrick03=new G4Tubs("LeadBrick03", innerRadius, outerRadius,HeightCyl03/2 , startAngle, spanningAngle);
//        G4VisAttributes Apar03(G4Colour::Blue());
//        logicBrick03 = new G4LogicalVolume(solidBrick03, Materials().Water, "LeadBrick03");
 //       logicBrick03->SetVisAttributes(Apar03);
//        G4ThreeVector positionCyl03(0.,CoorY03 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
//        physBrick03 = new G4PVPlacement(rotationCyl,positionCyl03, logicBrick03, "LeadBrick03", logicWorld, false, 0, cfg.fCheckOverlaps);
//###################################################################

  //      //Cilindro 04 - PLato de destilaciòn
//	double CoorY04 =CoorY03-HeightCyl03/2-HeightCyl04/2;
  //      solidBrick04=new G4Tubs("LeadBrick04", innerRadius, outerRadius,HeightCyl04/2 , startAngle, spanningAngle);
    //    G4VisAttributes Apar04(G4Colour::Cyan());
    //    logicBrick04 = new G4LogicalVolume(solidBrick04, Materials().Steel, "LeadBrick04");
    //    logicBrick04->SetVisAttributes(Apar04);
    //    G4ThreeVector positionCyl04(0.,CoorY04 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
   //  physBrick04 = new G4PVPlacement(rotationCyl,positionCyl04, logicBrick04, "LeadBrick04", logicWorld, false, 0, cfg.fCheckOverlaps);

        //Cilindro 05 - Aire
//	double CoorY05 =CoorY04-HeightCyl04/2-HeightCyl05/2;
//        solidBrick05=new G4Tubs("LeadBrick05", innerRadius, outerRadius,HeightCyl05/2 , startAngle, spanningAngle);
//        G4VisAttributes Apar05(G4Colour::Green());
//        logicBrick05 = new G4LogicalVolume(solidBrick05, Materials().Air, "LeadBrick05");
//        logicBrick05->SetVisAttributes(Apar05);
//        G4ThreeVector positionCyl05(0.,CoorY05 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
//     physBrick05 = new G4PVPlacement(rotationCyl,positionCyl05, logicBrick05, "LeadBrick05", logicWorld, false, 0, cfg.fCheckOverlaps);

        //Cilindro 06 - Plato de destilaciòn
//	double CoorY06 =CoorY05-HeightCyl05/2-HeightCyl06/2;
     //   solidBrick06=new G4Tubs("LeadBrick06", innerRadius, outerRadius,HeightCyl06/2 , startAngle, spanningAngle);
     //   G4VisAttributes Apar06(G4Colour::Cyan());
       // logicBrick06 = new G4LogicalVolume(solidBrick06, Materials().Steel, "LeadBrick06");
      //  logicBrick06->SetVisAttributes(Apar06);
    //    G4ThreeVector positionCyl06(0.,CoorY06 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
  //   physBrick06 = new G4PVPlacement(rotationCyl,positionCyl06, logicBrick06, "LeadBrick06", logicWorld, false, 0, cfg.fCheckOverlaps);

        //Cilindro 07 - Aire
//	double CoorY07 =CoorY06-HeightCyl06/2-HeightCyl07/2;
//        solidBrick07=new G4Tubs("LeadBrick07", innerRadius, outerRadius,HeightCyl07/2 , startAngle, spanningAngle);
//        G4VisAttributes Apar07(G4Colour::Green());
//        logicBrick07 = new G4LogicalVolume(solidBrick07, Materials().Air, "LeadBrick07");
//        logicBrick07->SetVisAttributes(Apar07);
//        G4ThreeVector positionCyl07(0.,CoorY07 ,Dist-500 * CLHEP::cm );// coordenadas del Centro del cilindro
//     physBrick07 = new G4PVPlacement(rotationCyl,positionCyl07, logicBrick07, "LeadBrick07", logicWorld, false, 0, cfg.fCheckOverlaps);

// HASTA AQUÌ
//
//
// Tamaño de los bloques
//double fBrickSizeX = 8.0 * CLHEP::cm;
//double fBrickSizeY = 20.0 * CLHEP::cm;
//double fBrickSizeZ = 40.0 * CLHEP::cm;

//double altura = 10.0 * CLHEP::cm;

// Crear el primer bloque
//G4Box* solidBrick1 = new G4Box("LeadBrick1", fBrickSizeX, fBrickSizeY, fBrickSizeZ);
//G4LogicalVolume* logicBrick1 = new G4LogicalVolume(solidBrick1, Materials().Oil, "LeadBrick1");
//G4VPhysicalVolume* physBrick1 = new G4PVPlacement(nullptr, G4ThreeVector(-fBrickSizeX * 2, 0.,altura+ fBrickSizeZ), logicBrick1, "LeadBrick1", logicWorld, false, 0, cfg.fCheckOverlaps);

// Asignar un atributo visual (color) al primer bloque
//G4VisAttributes* azul = new G4VisAttributes(G4Colour::Blue());
//logicBrick1->SetVisAttributes(azul);

// Crear el segundo bloque al lado del primero
//G4Box* solidBrick2 = new G4Box("LeadBrick2", fBrickSizeX, fBrickSizeY, fBrickSizeZ);
//G4LogicalVolume* logicBrick2 = new G4LogicalVolume(solidBrick2, Materials().Water, "LeadBrick2");
//G4VPhysicalVolume* physBrick2 = new G4PVPlacement(nullptr, G4ThreeVector(0., 0.,altura+ fBrickSizeZ), logicBrick2, "LeadBrick2", logicWorld, false, 0, cfg.fCheckOverlaps);

//G4VisAttributes* cyan = new G4VisAttributes(G4Colour::Cyan());
//logicBrick2->SetVisAttributes(cyan);


// Crear el tercer bloque al lado del segundo
//G4Box* solidBrick3 = new G4Box("LeadBrick3", fBrickSizeX, fBrickSizeY, fBrickSizeZ);
//G4LogicalVolume* logicBrick3 = new G4LogicalVolume(solidBrick3, Materials().Oil, "LeadBrick3");
//G4VPhysicalVolume* physBrick3 = new G4PVPlacement(nullptr, G4ThreeVector(fBrickSizeX*2, 0.,altura+ fBrickSizeZ), logicBrick3, "LeadBrick3", logicWorld, false, 0, cfg.fCheckOverlaps);

//logicBrick3->SetVisAttributes(azul);













//########################## Esructura
        double Y_centro_panelI = 0 * CLHEP::cm; //ubicaciòn del centro del primer panel (con esto se ubican todos los blindajes)
        double Z_centro_panelI = -500 * CLHEP::cm;


	double coversize = 32.0 * CLHEP::cm;
	// lead bricks I (CAPA DE PLOMO ATENUADORA)
        double fBrickSizeXI = 28.5 * CLHEP::cm;
        double fBrickSizeYI = 28.5 * CLHEP::cm;
        double fBrickSizeZI = 1.0 * CLHEP::cm;

        solidBrickI = new G4Box("LeadBrickI", coversize, coversize, fBrickSizeZI);
        logicBrickI = new G4LogicalVolume(solidBrickI, Materials().Lead, "LeadBrickI");
	physBrickI = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI,Z_centro_panelI+5.0*CLHEP::cm), logicBrickI, "LeadBrickI", logicWorld, false, 0, cfg.fCheckOverlaps); 

	G4VisAttributes* negro = new G4VisAttributes(G4Colour::Black());
	logicBrickI->SetVisAttributes(negro);

//########################################## RECORDAR QUE EL PANEL I ESTA EN (0,Y_centro_panelI,Z_centro_panelI)
	// Recubrimeinto de Hierro I
	double fIronCoverSizeX = 25.5 * CLHEP::cm;
	double fIronCoverSizeY = 25.5 * CLHEP::cm;
	double fIronCoverSizeZ = 0.2 * CLHEP::cm;

	solidIronCoverI = new G4Box("IronCoverI", coversize, coversize, fIronCoverSizeZ);
	logicIronCoverI = new G4LogicalVolume(solidIronCoverI, Materials().Iron, "IronCoverI");
	physIronCoverI = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI ,Z_centro_panelI+3.0 * CLHEP::cm), logicIronCoverI, "IronCoverI", logicWorld, false, 0, cfg.fCheckOverlaps);

        G4VisAttributes* rojo = new G4VisAttributes(G4Colour::Red());
        logicIronCoverI->SetVisAttributes(rojo);

//##################### II 
	double fIronCoverSizeXII = 25.5 * CLHEP::cm;
	double fIronCoverSizeYII = 25.5 * CLHEP::cm;
	double fIronCoverSizeZII = 0.2 * CLHEP::cm;

	solidIronCoverII = new G4Box("IronCoverII", coversize, coversize, fIronCoverSizeZII);
	logicIronCoverII = new G4LogicalVolume(solidIronCoverII, Materials().Iron, "IronCoverII");
	physIronCoverII = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI ,Z_centro_panelI-3.0 * CLHEP::cm), logicIronCoverII, "IronCoverII", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverII->SetVisAttributes(rojo);


//######################################### EL PANEL II ESTA EN (0,Y_centro_panelI,Z_centro_panelI-200.0 CM)
//#################### III

	double posicionsegundopanel = -200.0 * CLHEP::cm;

	double fIronCoverSizeXIII = 25.5 * CLHEP::cm;
	double fIronCoverSizeYIII = 25.5 * CLHEP::cm;
	double fIronCoverSizeZIII = 0.2 * CLHEP::cm;

	solidIronCoverIII = new G4Box("IronCoverIII", coversize, coversize, fIronCoverSizeZIII);
	logicIronCoverIII = new G4LogicalVolume(solidIronCoverIII, Materials().Iron, "IronCoverIII");
	physIronCoverIII = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI,Z_centro_panelI+posicionsegundopanel+3), logicIronCoverIII, "IronCoverIII", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverIII->SetVisAttributes(rojo);

//33333333333333333333 IV

	double fIronCoverSizeXIV = 25.5 * CLHEP::cm;
	double fIronCoverSizeYIV = 25.5 * CLHEP::cm;
	double fIronCoverSizeZIV = 0.2 * CLHEP::cm;

	solidIronCoverIV = new G4Box("IronCoverIV", coversize, coversize, fIronCoverSizeZIV);
	logicIronCoverIV = new G4LogicalVolume(solidIronCoverIV, Materials().Iron, "IronCoverIV");
	physIronCoverIV = new G4PVPlacement(nullptr, G4ThreeVector(0., Y_centro_panelI,Z_centro_panelI+posicionsegundopanel-3), logicIronCoverIV, "IronCoverIV", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverIV->SetVisAttributes(rojo);




}


void
G4RCDetectorConstruction::PlaceDetector(Event& theEvent)
{
	
	const Event::Config &cfg = theEvent.GetConfig();
	// loop in detector vector
	for (auto detIt = theEvent.DetectorRange().begin(); detIt != theEvent.DetectorRange().end(); detIt++) {
		auto& currentDet = detIt->second;
		BuildDetector(logicWorld, currentDet, theEvent, cfg.fCheckOverlaps);
	}

}

G4VPhysicalVolume* 
G4RCDetectorConstruction::Construct() 
{

	if (!physWorld) {
		//CreateElements();
		return CreateDetector();
	}
	return physWorld;

}
