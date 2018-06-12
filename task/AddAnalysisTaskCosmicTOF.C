AliAnalysisTaskCosmicTOF * 
AddAnalysisTaskCosmicTOF(const char *suffix = "")
{  
  //
  // -- INITIALIZATION ----------------------------------------------------------------------------
  // retrieve analysis manager
  //
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddAnalysisTaskCosmicTOF", "No analysis manager to connect to.");
    return NULL;
  } 
  
  // create the task and configure 
  TString taskName = "CosmicTOF";
  if (strlen(suffix) > 0) taskName += Form("_%s", suffix);
  AliAnalysisTaskCosmicTOF *task = new AliAnalysisTaskCosmicTOF(taskName.Data());
  //  task->SelectCollisionCandidates(triggerMask);
  mgr->AddTask(task);
  
  /* create output data container */
  TString outputFileName = AliAnalysisManager::GetCommonFileName();
  outputFileName += ":CosmicTOF";
  AliAnalysisDataContainer *output = mgr->CreateContainer("CosmicTOF", TTree::Class(), AliAnalysisManager::kOutputContainer, outputFileName);
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output);
  
  return task;
}
