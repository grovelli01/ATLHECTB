//**************************************************
// \file ATLHECTBEventAction.cc
// \brief: Implementation of ATLHECTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includera from project files
//
#include "ATLHECTBEventAction.hh"
#include "ATLHECTBRunAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>

//Define constructor
//
ATLHECTBEventAction::ATLHECTBEventAction()
    : G4UserEventAction(),
    PDGID(0),
    vertexkenergy(0.),
    eleakage(0.)
{}

//Define deconstructor
//
ATLHECTBEventAction::~ATLHECTBEventAction() {}

//Define BeginOfEventAction() and EndOfEventAction() methods
//
void ATLHECTBEventAction::BeginOfEventAction( const G4Event* ){
    
    //Initialize variable per event
    //
    PDGID =  0;
    vertexkenergy = 0.;
    eleakage = 0.;

}

void ATLHECTBEventAction::EndOfEventAction( const G4Event* ) {

    //Accumulate statistics
    //
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, PDGID);
    analysisManager->FillNtupleDColumn(1, vertexkenergy); 
    analysisManager->FillNtupleDColumn(2, eleakage);
    analysisManager->AddNtupleRow();

}

//**************************************************
