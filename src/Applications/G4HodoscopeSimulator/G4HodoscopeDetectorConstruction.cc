// implementation of the G4HodoscopeDetectorConstruction class
#include <iostream>

#include "G4HodoscopeDetectorConstruction.h"
#include "Materials.h"

#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

using namespace std;


G4HodoscopeDetectorConstruction::G4HodoscopeDetectorConstruction(Event& theEvent) : 
	G4VUserDetectorConstruction(),
	fEvent(theEvent)
{ 
}

G4HodoscopeDetectorConstruction::~G4HodoscopeDetectorConstruction() 
	{ }

G4VPhysicalVolume*
G4HodoscopeDetectorConstruction::CreateDetector()
{

	CreateWorld();
	//CreateGround();
	PlaceDetector(fEvent);
	return physWorld;
}

void
G4HodoscopeDetectorConstruction::CreateWorld()
{

	// size definitions
	const Event::Config &cfg = fEvent.GetConfig();	

	double fWorldSizeX = 7 * CLHEP::m;  //10 con torre 7 para cubo
	double fWorldSizeY = 10 * CLHEP::m;  //14          
	double fWorldSizeZ = 10 * CLHEP::m;  //14

	solidWorld  = new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);
	logicWorld = new G4LogicalVolume(solidWorld, Materials().Air, "World");
	physWorld  =  new G4PVPlacement(nullptr, G4ThreeVector(), "World", logicWorld, 0, false, 0, fCheckOverlaps);


//#################### Cubo de Plomo

// Dimensiones del cubo de plomo
//double fLeadCubeSize = 30.0 * CLHEP::cm;  // Lado del cubo

// Crear el sólido cúbico de plomo
//solidLeadCube = new G4Box("LeadCube", fLeadCubeSize/2, fLeadCubeSize/2, fLeadCubeSize/2);

// Crear el volumen lógico del cubo de plomo
//logicLeadCube = new G4LogicalVolume(solidLeadCube, Materials().Lead, "LeadCube");

// Crear una matriz de rotación y rotar respecto al eje Z
//G4RotationMatrix* rotZ = new G4RotationMatrix();
//rotZ->rotateZ(45.0 * CLHEP::deg);  // Rotar 45 grados alrededor del eje Z

// Colocar el cubo de plomo en el mundo o en otro volumen, aplicando la rotación
//physLeadCube = new G4PVPlacement(rotZ, G4ThreeVector(0., 0.0 * CLHEP::cm, -75 * CLHEP::cm), logicLeadCube, "LeadCube", logicWorld, false, 0);

// Establecer los atributos de visualización (opcional)
//G4VisAttributes* gris = new G4VisAttributes(G4Colour::Grey());
//logicLeadCube->SetVisAttributes(gris);



//#####
//###################--------OBJETIVO CON INTERFASE------------#################################
        // A PARTIR DE AQUÌ EDITO - PEDRO ARANGO EDIT

	double startAngle =0;  // Starting angle (usually 0)
        double spanningAngle = 360; // Spanning angle in degrees (360 mean a full circle)
	double AngleY=0 * deg;// àngulo de inclinaciòn de la imagen en el Hodoscopio con respecto a la vertical.
	G4RotationMatrix* rotationCyl = new G4RotationMatrix();
	rotationCyl->rotateX(90 * deg);
	rotationCyl->rotateY(AngleY);
	double SenAng=-sin(AngleY);// signo menos porque el angulo apunta al X negativo
        double CosAng=cos(AngleY);
	double CoorZc = 0 * CLHEP::cm;// coordenada horizontal del centro del cilindro
        double CoorYc = 0 * CLHEP::cm;// coordenada vertical del centro del cilindro
	G4ThreeVector positionCyl(0,CoorYc,CoorZc);
	bool TipoTorre=true; // elegir "true" para torre de hidrotratamiento y "false" para torre de destilaciòn fraccionada
        double espesorI = 0.2 * CLHEP::cm;// espesor de la envolvente de acero 
        double espesorII = 0.3 * CLHEP::cm;// espesor de la envolvente de Calorifugado
        double espesorIII = 0.1 * CLHEP::cm;// espesor de la envolvente de Aluminio


	//++++++++ Datos de Torre de Hidrotratamiento de Lecho Fijo ++++++++++++++++++++++
	double RadioLecho = 50 * CLHEP::cm;// radio del cilindro catalizador
        double HeightIner01=12* CLHEP::cm; // altura de Seccion Inerte 01
        double HeightIner02=12* CLHEP::cm; // altura de Seccion Inerte 02
        double HeighLecho01=60* CLHEP::cm; // Altura de lecho fijo 01 ( densidad normal ) hacer 0 para incrementar densidad
        double HeighLecho02=60* CLHEP::cm; // Altura de lecho fijo 02 ( densidad incrementada ) hacer 1200 para incrementar densidad
	int incrLecho=4; // Incremento de densidad lecho2:    1 inc 15%;   2 inc 30%;    3 inc 45%;    4 inc 55%;
        bool Lecho2Incrementado=false; // "true" si consideramos porciòn con densidad aumentada y "false" si no lo consideramos
       	double HeightCylHidro = HeightIner01+HeightIner02+HeighLecho01+HeighLecho02; // altura total del cilindro de hidrotratamiento
	double HCoorY01 =CoorYc+(HeightCylHidro/2-HeightIner01/2)*CosAng;
        double HCoorY02 =HCoorY01-(HeightIner01/2+HeighLecho01/2)*CosAng;
        double HCoorY03 =HCoorY02-(HeighLecho01/2+HeighLecho02/2)*CosAng;
        double HCoorY04 =HCoorY03-(HeighLecho02/2+HeightIner02/2)*CosAng;
        double HCoorX01 =(HeightCylHidro/2-HeightIner01/2)*SenAng;
        double HCoorX02 =HCoorX01-(HeightIner01/2+HeighLecho01/2)*SenAng;
        double HCoorX03 =HCoorX02-(HeighLecho01/2+HeighLecho02/2)*SenAng;
        double HCoorX04 =HCoorX03-(HeighLecho02/2+HeightIner02/2)*SenAng;


        //++++++++++++ Definiendo si se considera o no ciertos materiales en el modelo +++++++++++
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//        bool PresMat01 = true; // si se considera o no Agua en el primer bloque 
//        bool PresMat03= true; // si se considera o no Agua en el segundo bloque
//        bool PresMat05 = true; // si se considera o no Aire en el tercer bloque
//        bool PresMat07 = true; // si se considera o no Aire en el cuarto bloque
        bool PresMat09 = true; // si se considera o no la envolvente de Calorifugado
        bool PresMat10 = true; // si se considera o no la envolvente de Aluminio
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//if (TipoTorre)
//{       
// //    Sección Inerte 01
//    HsolidBrick01 = new G4Tubs("HLeadBrick01", 0, RadioLecho, HeightIner01 / 2, startAngle, spanningAngle);
//    G4VisAttributes HApar01(G4Colour::Cyan());
//    HlogicBrick01 = new G4LogicalVolume(HsolidBrick01, Materials().Steel, "HLeadBrick01");
//    HlogicBrick01->SetVisAttributes(HApar01);
//    G4ThreeVector HpositionCyl01(HCoorX01, HCoorY01, CoorZc); // Coordenadas del centro del cilindro
//    HphysBrick01 = new G4PVPlacement(rotationCyl, HpositionCyl01, HlogicBrick01, "HLeadBrick01", logicWorld, false, 0, cfg.fCheckOverlaps);

    // Sección Lecho sin Residuos
//    HsolidBrick02 = new G4Tubs("HLeadBrick02", 0, RadioLecho, HeighLecho01 / 2, startAngle, spanningAngle);
//    G4VisAttributes HApar02(G4Colour::Grey());
//    HlogicBrick02 = new G4LogicalVolume(HsolidBrick02, Materials().LechoFijo_74porciento, "HLeadBrick02");
//    HlogicBrick02->SetVisAttributes(HApar02);
//    G4ThreeVector HpositionCyl02(HCoorX02, HCoorY02, CoorZc); // Coordenadas del centro del cilindro
//    HphysBrick02 = new G4PVPlacement(rotationCyl, HpositionCyl02, HlogicBrick02, "HLeadBrick02", logicWorld, false, 0, cfg.fCheckOverlaps);

//    if (Lecho2Incrementado)
//    {   
//        // Sección de Lecho con Residuos - Densidad Incrementada
//        HsolidBrick03 = new G4Tubs("HLeadBrick03", 0, RadioLecho, HeighLecho02 / 2, startAngle, spanningAngle);
//        G4VisAttributes HApar03(G4Colour::Red());
//        if (incrLecho == 1)
//        {
//            HlogicBrick03 = new G4LogicalVolume(HsolidBrick03, Materials().LechoFijo_20porciento, "HLeadBrick03");
//        }
//        else if (incrLecho == 2)
//        {
//            HlogicBrick03 = new G4LogicalVolume(HsolidBrick03, Materials().LechoFijo_20porciento, "HLeadBrick03");
//        }
//        else if (incrLecho == 3)
//        {
//            HlogicBrick03 = new G4LogicalVolume(HsolidBrick03, Materials().LechoFijo_20porciento, "HLeadBrick03");
//        }
//        else
//        {
//            HlogicBrick03 = new G4LogicalVolume(HsolidBrick03, Materials().LechoFijo_20porciento, "HLeadBrick03");
//        }
//        HlogicBrick03->SetVisAttributes(HApar03);
//        G4ThreeVector HpositionCyl03(HCoorX03, HCoorY03, CoorZc); // Coordenadas del centro del cilindro
//        HphysBrick03 = new G4PVPlacement(rotationCyl, HpositionCyl03, HlogicBrick03, "HLeadBrick03", logicWorld, false, 0, cfg.fCheckOverlaps);
//    }

//    // Sección Inerte 02
//    HsolidBrick04 = new G4Tubs("HLeadBrick04", 0, RadioLecho, HeightIner02 / 2, startAngle, spanningAngle);
//    G4VisAttributes HApar04(G4Colour::Cyan());
//    HlogicBrick04 = new G4LogicalVolume(HsolidBrick04, Materials().Steel, "HLeadBrick04");
//    HlogicBrick04->SetVisAttributes(HApar04);
//    G4ThreeVector HpositionCyl04(HCoorX04, HCoorY04, CoorZc); // Coordenadas del centro del cilindro
//    HphysBrick04 = new G4PVPlacement(rotationCyl, HpositionCyl04, HlogicBrick04, "HLeadBrick04", logicWorld, false, 0, cfg.fCheckOverlaps);

//    // Envolvente de Acero
//    HsolidBrick05 = new G4Tubs("HLeadBrick05", RadioLecho, RadioLecho + espesorI, HeightCylHidro / 2, startAngle, spanningAngle);
//    G4VisAttributes HApar05(G4Colour::Cyan());
//    HlogicBrick05 = new G4LogicalVolume(HsolidBrick05, Materials().Steel, "HLeadBrick05");
//    HlogicBrick05->SetVisAttributes(HApar05);
//    HphysBrick05 = new G4PVPlacement(rotationCyl, positionCyl, HlogicBrick05, "HLeadBrick05", logicWorld, false, 0, cfg.fCheckOverlaps);

    // Envolvente de Calorifugado
//    if (PresMat09)
//    {
//        HsolidBrick06 = new G4Tubs("HLeadBrick06", RadioLecho + espesorI, RadioLecho + espesorI + espesorII, HeightCylHidro / 2, startAngle, spanningAngle);
//        G4VisAttributes HApar06(G4Colour::Brown());
//        HlogicBrick06 = new G4LogicalVolume(HsolidBrick06, Materials().Air, "HLeadBrick06");
//        HlogicBrick06->SetVisAttributes(HApar06);
//        HphysBrick06 = new G4PVPlacement(rotationCyl, positionCyl, HlogicBrick06, "HLeadBrick06", logicWorld, false, 0, cfg.fCheckOverlaps);
//    }

//    // Envolvente de Aluminio
//    if (PresMat10)
//    {
//        HsolidBrick07 = new G4Tubs("HLeadBrick07", RadioLecho + espesorI + espesorII, RadioLecho + espesorI + espesorII + espesorIII, HeightCylHidro / 2, startAngle, spanningAngle);
//        G4VisAttributes HApar07(G4Colour::Gray());
//        HlogicBrick07 = new G4LogicalVolume(HsolidBrick07, Materials().Alum, "HLeadBrick07");
//        HlogicBrick07->SetVisAttributes(HApar07);
//        HphysBrick07 = new G4PVPlacement(rotationCyl, positionCyl, HlogicBrick07, "HLeadBrick07", logicWorld, false, 0, cfg.fCheckOverlaps);
//    }
//}






//########################## Estructura
        double Y_centro_panelI = 0 * CLHEP::cm; //ubicaciòn del centro del primer panel (con esto se ubican todos los blindajes)
        double Z_centro_panelI = -100 * CLHEP::cm;


	double coversize = 32.0 * CLHEP::cm;
	// lead bricks I (CAPA DE PLOMO ATENUADORA)
        double fBrickSizeXI = 28.5 * CLHEP::cm;
        double fBrickSizeYI = 28.5 * CLHEP::cm;
        double fBrickSizeZI = 3.0 * CLHEP::cm;

        solidBrickI = new G4Box("LeadBrickI", coversize, coversize, fBrickSizeZI);
        logicBrickI = new G4LogicalVolume(solidBrickI, Materials().Lead, "LeadBrickI");
	physBrickI = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI,Z_centro_panelI+9.0*CLHEP::cm), logicBrickI, "LeadBrickI", logicWorld, false, 0, cfg.fCheckOverlaps); 

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

	double posicionsegundopanel = -100.0 * CLHEP::cm;

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
G4HodoscopeDetectorConstruction::PlaceDetector(Event& theEvent)
{
	
	const Event::Config &cfg = theEvent.GetConfig();
	// loop in detector vector
	for (auto detIt = theEvent.DetectorRange().begin(); detIt != theEvent.DetectorRange().end(); detIt++) {
		auto& currentDet = detIt->second;
		BuildDetector(logicWorld, currentDet, theEvent, cfg.fCheckOverlaps);
	}

}

G4VPhysicalVolume* 
G4HodoscopeDetectorConstruction::Construct() 
{

	if (!physWorld) {
		//CreateElements();
		return CreateDetector();
	}
	return physWorld;

}
