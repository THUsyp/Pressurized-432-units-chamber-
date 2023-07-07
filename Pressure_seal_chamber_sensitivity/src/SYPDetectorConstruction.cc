
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "SYPDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "CADMesh.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPDetectorConstruction::SYPDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SYPDetectorConstruction::~SYPDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* SYPDetectorConstruction::Construct()
{
  // Material definition
  G4NistManager* nistManager = G4NistManager::Instance();

  // Air definition using NIST Manager
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Parameter
  G4double  z, a, fractionmass, density;
  G4String name, symbol;
  G4int ncomponents;

  // 95WNiFe
  G4Element* elW  = new G4Element(name="Tungsten", symbol = "W", z= 74., a=183.84*g/mole);
  G4Element* elNi = new G4Element(name="Nickle",symbol="Ni",z=28.,a=58.69*g/mole);
  G4Element* elFe = new G4Element(name="Ferrum",symbol="Fe",z=26.,a=55.845*g/mole);
  density = 18.75*g/cm3;
  G4Material* W95NiFe = new G4Material(name="95WNiFe",density,ncomponents=3);
  W95NiFe->AddElement(elW, fractionmass=95*perCent);
  W95NiFe->AddElement(elNi,fractionmass=2.5*perCent);
  W95NiFe->AddElement(elFe,fractionmass=2.5*perCent);

  // Stainless Steel
  G4Element* elCr = new G4Element(name="chromium",symbol="Cr",z=24,a=51.996*g/mole);
  G4Element* elMn = new G4Element(name="Manganese",symbol="Mn",z=25,a=54.938*g/mole);
  density = 7.93*g/cm3;
  G4Material* SS = new G4Material(name="StainlessSteel",density,ncomponents=4);
  SS->AddElement(elFe,fractionmass=68*perCent);
  SS->AddElement(elCr,fractionmass=20*perCent);
  SS->AddElement(elNi,fractionmass=10*perCent);
  SS->AddElement(elMn,fractionmass= 2*perCent);

  // Kr-20atm
  new G4Material("Kr_20atm", z=36., a=83.798*g/mole,density= 0.0861*g/cm3,
                 kStateGas, 2.93*kelvin, 20*atmosphere);
  // Xe-48atm
  new G4Material("Xe_48atm", z=54., a=131.2*g/mole,density= 0.3723*g/cm3,
                 kStateGas, 2.93*kelvin, 48*atmosphere);
  // Xe-48atm
  new G4Material("Xe_20atm", z=54., a=131.2*g/mole,density= 0.1199*g/cm3,
                   kStateGas, 2.93*kelvin, 20*atmosphere);

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // Parameter
  G4double layer_sizeX = 110.8*mm, layer_sizeY = 3.5*mm, layer_sizeZ = 111.0000004*mm;
  G4double cham_sizeX = 3*mm, cham_sizeY = 3.5*mm, cham_sizeZ = 111.0000002*mm;
  G4double HE_sizeX = 0.2*mm, HE_sizeY = 3.5*mm, HE_sizeZ = 101.*mm;
  G4double EC_sizeX = HE_sizeX, EC_sizeY = HE_sizeY, EC_sizeZ = 111*mm;
  G4double HalfHE_sizeSX = 0.5*HE_sizeX;
  G4double parti_sizeX = 110.8*mm, parti_sizeY = 1*mm, parti_sizeZ = 111*mm;
//  G4double insu_sizeX = 110.8*mm, insu_sizeY = 0.25*mm, insu_sizeZ = 111*mm;
  G4double side_sizeX = 1*mm, side_sizeY = 86*mm, side_sizeZ = 95*mm;

  //
  G4int nofUnits = 36;
  G4int nofLayer = 12;
  G4int nofPartition = nofLayer+1;
  G4double world_sizeX = 1.2*cham_sizeX*nofUnits; G4double world_sizeY = 1.5 * side_sizeY; G4double world_sizeZ = 10*m;
  //
  G4Material* world_mat = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* cham_mat = G4Material::GetMaterial("Xe_20atm");
  //G4Material* electrodeSlice_mat = nistManager->FindOrBuildMaterial("G4_Cu");
  G4Material* electrodeSlice_mat = nistManager->FindOrBuildMaterial("95WNiFe");
  G4Material* structure_mat = nistManager->FindOrBuildMaterial("StainlessSteel");

  //
  // World
  //

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  //
  // Layer
  //
  G4Box* solidLayer = new G4Box( "Layer", 0.5*layer_sizeX, 0.5*layer_sizeY, 0.5*layer_sizeZ );
  G4LogicalVolume* logicLayer = new G4LogicalVolume( solidLayer,
                                                     cham_mat,
                                                     "Layer" );

  //
  // Chamber-workingGas
  //

  G4Box* solidCham =
    new G4Box("Chamber",                    //its name
        0.5*cham_sizeX, 0.5*cham_sizeY, 0.5*cham_sizeZ); //its size
  G4LogicalVolume* logicCham =
    new G4LogicalVolume(solidCham,            //its solid
                        cham_mat,             //its material
                        "Chamber");         //its name

  //
  // High Voltage Electrode Slice ( half thickness )
  //

  G4Box* solidHalfHE = new G4Box("Half_HE",
                                0.5*HalfHE_sizeSX,0.5*HE_sizeY,0.5*HE_sizeZ);
  G4LogicalVolume* logicHalfHE = new G4LogicalVolume(solidHalfHE,
                                                     electrodeSlice_mat,
                                                     "Half_HE");
  for( G4int i = 0; i < 2; i++ )
  {
      G4ThreeVector HalfHE_pos = G4ThreeVector ((-0.5+i)*2.9*mm,0,5*mm);
      new G4PVPlacement(0,
                        HalfHE_pos,
                        logicHalfHE,
                        "Half_HE",
                        logicCham,
                        false,
                        0,
                        checkOverlaps);

      G4ThreeVector sideHE_pos = G4ThreeVector ( (-0.5+i)*108.2*mm,0,5*mm);
      new G4PVPlacement(0,
                        sideHE_pos,
                        logicHalfHE,
                        "Half_HE",
                        logicLayer,
                        false,
                        0,
                        checkOverlaps);
  }

  //
  // Electron Collector
  //

  G4Box* solidEC = new G4Box("EC",
                             0.5*EC_sizeX,0.5*EC_sizeY,0.5*EC_sizeZ);
  G4LogicalVolume* logicEC = new G4LogicalVolume(solidEC,
                                                 electrodeSlice_mat,
                                                 "EC");
  //
  G4ThreeVector EC_pos = G4ThreeVector (0,0,0);
  new G4PVPlacement(0,
                    EC_pos,
                    logicEC,
                    "EC",
                    logicCham,
                    false,
                    0,
                    checkOverlaps);

  // place chamber in layer

  for( G4int i = 0; i < nofUnits; i++ )
  {
      G4ThreeVector cham_pos = G4ThreeVector ((-17.5+i)*3*mm,0,0*mm);
      new G4PVPlacement(0,
                        cham_pos,
                        logicCham,
                        "Chamber",
                        logicLayer,
                        false,
                        i,
                        checkOverlaps);
  }

  // place layer in world

  for( G4int i = 0; i < nofLayer; i++ )
  {
      G4ThreeVector layer_pos = G4ThreeVector ( 0,(-5.5+i)*5*mm,-55.5*mm);
      new G4PVPlacement(0,
                        layer_pos,
                        logicLayer,
                        "Layer",
                        logicWorld,
                        false,
                        i,
                        checkOverlaps);
  }

  // Partition
  G4Box* solidParti = new G4Box( "Partition", 0.5*parti_sizeX, 0.5*parti_sizeY, 0.5*parti_sizeZ);
  G4LogicalVolume* logicParti = new G4LogicalVolume (
          solidParti,
          structure_mat,
          "Partition");

  for ( G4int i = 0; i < nofPartition; i++ )
  {
      G4ThreeVector Parti_pos = G4ThreeVector (0,(-6+i)*5*mm,-55.5*mm);
      new G4PVPlacement(0,
                        Parti_pos,
                        logicParti,
                        "Partition",
                        logicWorld,
                        false,
                        0,
                        checkOverlaps);
  }

  // side plate
  G4Box* solidside = new G4Box ( "side", 0.5*side_sizeX, 0.5*side_sizeY, 0.5*side_sizeZ );
  G4LogicalVolume* logicside = new G4LogicalVolume(
          solidside,
          structure_mat,
          "side");
  for( G4int i = 0; i < 2; i++ )
  {
      G4ThreeVector side_pos = G4ThreeVector ( (-0.5+i)*111.8*mm,12*mm,-47.5*mm);
      new G4PVPlacement(0,
                        side_pos,
                        logicside,
                        "side",
                        logicWorld,
                        false,
                        0,
                        checkOverlaps);
  }




  // Set working gas as scoring volume
  //
  fScoringVolume = logicCham;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
