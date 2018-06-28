const Char_t *AliPhysicsVersion = "v5-09-24-01-1";

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

void run(TString pluginMode = "test", 
	 TString dataset = "test", 
	 TString runlist = "test")
{

  /* include path for ACLic */
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include");
  /* load libraries */
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  //
  TString trainName = "PWGLF";
  //
  TString analysisTag = "CosmicTOF"; 
  analysisTag += "_"; 
  analysisTag += dataset; 
  analysisTag += "_"; 
  analysisTag += runlist;
  //  
  AliAnalysisGrid *alienHandler = CreateAlienHandler(pluginMode, dataset, runlist, analysisTag);
  //
  AliAnalysisManager *mgr  = new AliAnalysisManager("manager", "manager");
  mgr->SetDebugLevel(0);
  mgr->SetGridHandler(alienHandler);
  //
  AliESDInputHandler *inputHandler = new AliESDInputHandler("handler", "ESD handler");
  inputHandler->SetNeedField();
  mgr->SetInputEventHandler(inputHandler);

  /****************************************/
  /* ADD TASK                             */
  /****************************************/
  
  gROOT->LoadMacro("AliAnalysisTaskCosmicTOF.cxx++g");
  gROOT->LoadMacro("AddAnalysisTaskCosmicTOF.C");
  AddAnalysisTaskCosmicTOF();
  
  /****************************************/
  /* RUN ANALYSIS                         */
  /****************************************/

  if(!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("grid");
  return;

}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

AliAnalysisGrid* CreateAlienHandler(TString pluginmode,
				    TString dataset,
				    TString runlist,
				    TString analysisTag)
{

    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    plugin->SetRunMode(pluginmode.Data());
    plugin->SetAPIVersion("V1.1x");
    plugin->SetAliPhysicsVersion(AliPhysicsVersion);
    // config dataset
    ConfigDataset(plugin, dataset, runlist);
    //
    plugin->SetGridWorkingDir(analysisTag.Data());
    plugin->SetExecutable("CosmicTOF.sh");
    plugin->SetAnalysisMacro("CosmicTOF.C");    
    TString exe = analysisTag; exe += ".sh";
    TString mac = analysisTag; mac += ".C";
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    //
    plugin->SetAnalysisSource("AliAnalysisTaskCosmicTOF.cxx");
    plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include -I$ALICE_ROOT/STEER -I/$ALICE_ROOT/ANALYSIS -I/$ALICE_ROOT/ANALYSISalice");
    //
    plugin->SetAdditionalLibs("AliAnalysisTaskCosmicTOF.h AliAnalysisTaskCosmicTOF.cxx AddAnalysisTaskCosmicTOF.C libGui.so libProof.so libMinuit.so libANALYSIS.so libOADB.so libANALYSISalice.so");
    //
    plugin->SetKeepLogs(kTRUE);
    plugin->SetDefaultOutputs(kTRUE);
    plugin->SetMergeViaJDL(kTRUE);
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(1);
    //
    plugin->SetSplitMaxInputFileNumber(100);    
    plugin->SetNtestFiles(1);

    return plugin;
}

ConfigDataset(AliAnalysisAlien *plugin,
	      TString dataset,
	      TString runlist)
{

  // configure dataset
  //
  if (dataset.EqualTo("test")) {
    plugin->SetRunPrefix("000");
    plugin->SetGridDataDir("/alice/data/2017/LHC17k");
    plugin->SetDataPattern("cosmics_pass1/*/AliESDs.root");
  }
  //
  if (dataset.EqualTo("LHC17k")) {
    plugin->SetRunPrefix("000");
    plugin->SetGridDataDir("/alice/data/2017/LHC17k");
    plugin->SetDataPattern("cosmics_pass1/*/AliESDs.root");
  }
  //
  if (dataset.EqualTo("LHC15o")) {
    plugin->SetRunPrefix("000");
    plugin->SetGridDataDir("/alice/data/2015/LHC15o");
    plugin->SetDataPattern("cosmics_pass1/*/AliESDs.root");
  }

  // configure runlist
  //
  Int_t *_runlist = NULL;
  Int_t _nruns = 0;
  //
  Int_t runlist_test[] = {
    275878
  };
  //
  Int_t runlist_LHC17k[] = {
    275878, 275023, 275532, 275882
  };
  //
  Int_t runlist_LHC15o[] = {
    246469
  };
  //
  if (runlist.EqualTo("test")) {
    _runlist = runlist_test;
    _nruns = sizeof(runlist_test)/4;
  }
  //
  if (runlist.EqualTo("LHC17k")) {
    _runlist = runlist_LHC17k;
    _nruns = sizeof(runlist_LHC17k)/4;
  }
  //
  if (runlist.EqualTo("LHC15o")) {
    _runlist = runlist_LHC15o;
    _nruns = sizeof(runlist_LHC15o)/4;
  }
       
  // add the runs
  Int_t nruns = 0;
  for(Int_t irun = 0; irun < _nruns; irun++) {
    plugin->AddRunNumber(_runlist[irun]);
    nruns++;
  }
  printf(">>> ConfigDataset: added %d runs from runlist %s\n", nruns, runlist.Data()); 
  plugin->SetNrunsPerMaster(1);  
  plugin->SetOutputToRunNo(1);

  return;
  
}

