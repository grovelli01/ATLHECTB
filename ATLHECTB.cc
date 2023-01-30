//**************************************************
// \file ATLHECTB.cc
// \brief: main() of ATLHECTB project
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 11 May 2021
//**************************************************

//includers from project files
//
#include "ATLHECTBDetectorConstruction.hh"
#include "ATLHECTBActionInitialization.hh"

//Includers from Geant4
//
//#include "G4RunManagerFactory.hh" //G4RunManagerFactory is only available from 10.7
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"   
#include "G4StepLimiterPhysics.hh"
//#include "G4NeutronTrackingCut.hh" //for neutron time limit cut
//#include "G4SystemOfUnits.hh"      //for neutron time limit cut
#include "G4Version.hh"
#if G4VERSION_NUMBER >= 1110          // >= Geant4-11.1.0
#include "G4FTFTunings.hh"
#endif

//G4err output for usage error
//
namespace PrintUsageError {
    void UsageError() {
    G4cerr << "->ATLHECTB usage: " << G4endl;
    G4cerr << "ATLHECTB [-m macro] [-u UIsession] [-t nThreads] [-pl PhysicsList] [-tune FTFTuneName (optional)]" 
        << G4endl;
    }
}

//G4err output for FTFTune usage error
//
namespace PrintFTFTuneUsageError {
    void FTFTuneUsageError() {
    G4cerr << "Wrong FTF Alternative Tune Name selected. " << G4endl;
    G4cerr << "Geant4-11.1.0 valid names are: " << G4endl;
    G4cerr << "default\nbaryon-tune2022-v0\npion-tune2022-v0\ncombined-tune2022-v0" << G4endl;
    }
}

//main function
//
int main( int argc, char** argv ) {
   
    //Error in argument numbers
    //
    if ( argc > 11 ){
        PrintUsageError::UsageError();
        return 1;
    }

    //Convert arguments to G4string and G4int
    //
    G4String macro;
    G4String session;
    G4String custom_pl = "FTFP_BERT"; //default physics list
    #ifdef G4MULTITHREADED
    G4int nthreads = 0;
    #endif
    G4bool UseFTFTune = false; //default not using FTF alternative tunes
    G4String FTFTuneName;

    for ( G4int i=1; i<argc; i=i+2 ) {
        if ( G4String( argv[i] ) == "-m" ) macro = argv[i+1];
        else if ( G4String( argv[i] ) == "-u" ) session = argv[i+1];
        else if ( G4String( argv[i] ) == "-pl" ) custom_pl = argv[i+1];
        #ifdef G4MULTITHREADED
        else if ( G4String( argv[i] ) == "-t" ) {
            nthreads = G4UIcommand::ConvertToInt(argv[i+1]);} 
        #endif 
        else if ( G4String( argv[i] ) == "-tune" ) {
            UseFTFTune = true; FTFTuneName = argv[i+1];}
        else {
        PrintUsageError::UsageError();
            return 1;
        }
    } //end of converting arguments
    
    //Activate interaction mode if no macro card is provided and define UI session
    //
    G4UIExecutive* ui = nullptr;
    if ( ! macro.size() ) { //if macro card is not passed
        ui = new G4UIExecutive(argc, argv, session);
    }

    //Construct run manager (default type)
    /*uncomment this part for G4RunManagerFactory usage (10.7 on)
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    #ifdef G4MULTITHREADED
    if ( nthreads > 0 ) runManager->SetNumberOfThreads(nthreads);
    #endif
    */
    //Construct run manager (works also for versions before 10.7)
    #ifdef G4MULTITHREADED
    auto runManager = new G4MTRunManager;
    if ( nthreads > 0 ) { 
        runManager->SetNumberOfThreads(nthreads);
    }  
    #else
    auto runManager = new G4RunManager;
    #endif

    //Set mandatory classes (DetConstruction, PhysicsList, ActionInitialization)
    //
    auto DetConstruction = new ATLHECTBDetectorConstruction();
    runManager->SetUserInitialization( DetConstruction );

    auto physListFactory = new G4PhysListFactory;
    auto physList = physListFactory->GetReferencePhysList( custom_pl );
    physList->RegisterPhysics( new G4StepLimiterPhysics() );
    //auto nCut = new G4NeutronTrackingCut("neutronTrackingCut", 1);
    //nCut->SetTimeLimit(290.*ns);
    //physList->RegisterPhysics(nCut);
    runManager ->SetUserInitialization(physList);

    //Set FTF tunings (only => Geant4-11.1.0)
    //
    #if G4VERSION_NUMBER >= 1110          // => Geant4-11.1.0
    auto FTFTunings = G4FTFTunings::Instance();
    if (UseFTFTune){
        G4cout << "----------> Using FTF alternative tune: "<<FTFTuneName<< " <----------" << G4endl;
        if(FTFTuneName == "default")                   FTFTunings->SetTuneApplicabilityState(0,1); 
        else if(FTFTuneName == "baryon-tune2022-v0")   FTFTunings->SetTuneApplicabilityState(1,1); 
        else if(FTFTuneName == "pion-tune2022-v0")     FTFTunings->SetTuneApplicabilityState(2,1); 
        else if(FTFTuneName == "combined-tune2022-v0") FTFTunings->SetTuneApplicabilityState(3,1); 
        else {
            PrintFTFTuneUsageError::FTFTuneUsageError();
            return 1;
        }
    }    
    #endif

    //ActionInitialization part
    //
    auto ActInitialization = new ATLHECTBActionInitialization( /*DetConstruction*/ );
    runManager->SetUserInitialization( ActInitialization );

    //Initialize visualization
    //
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    //Get pointer to User Interface manager
    //
    auto UImanager = G4UImanager::GetUIpointer();

    //Process macro in macro mode or start UI session
    //
    if ( macro.size() ) { //macro card mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand("/process/em/verbose 0"); //avoid printing em processes
        UImanager->ApplyCommand("/process/had/verbose 0");//avoid printing had processes
        UImanager->ApplyCommand(command+macro);
    }
    else {                //start UI session
        UImanager->ApplyCommand( "/control/execute ATLHECTB_init_vis.mac" );
        if ( ui->IsGUI() ) {
            UImanager->ApplyCommand( "/control/execute ATLHECTB_gui.mac" );
        } 
        ui->SessionStart();
        delete ui;
    }

    //Program termination (user actions deleted by run manager)
    //
    delete visManager;
    delete runManager;
}

//**************************************************
