#include "AliAnalysisTaskCosmicTOF.h"
#include "AliAnalysisManager.h"
#include "AliESDInputHandler.h"
#include "AliESDtrackCuts.h"
#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "TLorentzVector.h"
#include "TTree.h"

/*
 * CosmicTOF Analysis
 * author: Roberto Preghenella (R+)
 * email:  preghenella@bo.infn.it
 *
 */

ClassImp(AliAnalysisTaskCosmicTOF)

//________________________________________________________________________


//________________________________________________________________________

AliAnalysisTaskCosmicTOF::AliAnalysisTaskCosmicTOF(const Char_t *name) :
  AliAnalysisTaskSE(name),
  fOutputTree(NULL),
  fNtrk(0),
  fTstamp(0),
  firedTrigger("")
{
  
  /*
   * default constructor
   */

  DefineOutput(1, TTree::Class());
}

//________________________________________________________________________

AliAnalysisTaskCosmicTOF::~AliAnalysisTaskCosmicTOF()
{
    
  /*
   * default destructor
   */
    
}

//________________________________________________________________________

void 
AliAnalysisTaskCosmicTOF::UserCreateOutputObjects()
{
    
  /*
   * user create output objects
   */
  
  fOutputTree = new TTree("CosmicTOF", "R+CosmicTOF tree with cosmic rays");
  //
  fOutputTree->Branch("ntrk", &fNtrk, "ntrk/I");
  fOutputTree->Branch("tstamp", &fTstamp, "tstamp/i");
  fOutputTree->Branch("firedTrigger", &firedTrigger);
   
  //
  fOutputTree->Branch("xv", &fXv, "xv[ntrk]/F");
  fOutputTree->Branch("yv", &fYv, "yv[ntrk]/F");
  fOutputTree->Branch("zv", &fZv, "zv[ntrk]/F");
  //
  fOutputTree->Branch("p", &fP, "p[ntrk]/F");
  fOutputTree->Branch("pt", &fPt, "pt[ntrk]/F");
  fOutputTree->Branch("theta", &fTheta, "theta[ntrk]/F");
  fOutputTree->Branch("phi", &fPhi, "phi[ntrk]/F");
  //
  fOutputTree->Branch("ncls", &fNcls, "ncls[ntrk]/I");
  fOutputTree->Branch("chi2", &fChi2, "chi2[ntrk]/F");
  fOutputTree->Branch("dedx", &fdEdx, "dedx[ntrk]/F");
  //
  fOutputTree->Branch("texp", &fTexp, "texp[ntrk]/F");
  fOutputTree->Branch("length", &fLength, "tength[ntrk]/F");
  //
  fOutputTree->Branch("index", &fIndex, "index[ntrk]/I");
  fOutputTree->Branch("time", &fTime, "time[ntrk]/F");
  fOutputTree->Branch("raw", &fRaw, "raw[ntrk]/F");
  fOutputTree->Branch("tot", &fTOT, "tot[ntrk]/F");
  //
  fOutputTree->Branch("dx", &fDx, "dx[ntrk]/F");
  fOutputTree->Branch("dz", &fDz, "dz[ntrk]/F");
  fOutputTree->Branch("nmatch", &fNmatch, "nmatch[ntrk]/I");
  fOutputTree->Branch("isTOFout", &isTOFout, "isTOFout[ntrk]/O");
 
  PostData(1, fOutputTree);
}

//________________________________________________________________________

void 
AliAnalysisTaskCosmicTOF::UserExec(Option_t *)
{

  /*
   * user exec
   */

  /** get the ESD event **/
  AliESDEvent *esd = dynamic_cast<AliESDEvent *>(InputEvent());
  if (!esd) return;

  fTstamp = esd->GetTimeStamp();
  firedTrigger = esd->GetFiredTriggerClasses();

  /** loop over ESD tracks **/
  Int_t ntracks = esd->GetNumberOfTracks();
  fNtrk = 0;
  AliESDtrack *tpctracks[1024];
  for (Int_t itrack = 0; itrack < ntracks; itrack++) {
    AliESDtrack *track = esd->GetTrack(itrack);
    if (!(track->GetStatus() & AliESDtrack::kTPCrefit)) continue;
    tpctracks[fNtrk++] = track;
    if (fNtrk == 1024) break;
  }
  if (fNtrk < 2) return;

  /** save info of selected event **/
  Double_t times[10];
  for (Int_t itrack = 0; itrack < fNtrk; itrack++) {
    //
    fXv[itrack] = tpctracks[itrack]->Xv();
    fYv[itrack] = tpctracks[itrack]->Yv();
    fZv[itrack] = tpctracks[itrack]->Zv();
    //
    fP[itrack] = tpctracks[itrack]->P();
    fPt[itrack] = tpctracks[itrack]->Pt();
    fTheta[itrack] = tpctracks[itrack]->Theta();
    fPhi[itrack] = tpctracks[itrack]->Phi();
    //
    fNcls[itrack] = tpctracks[itrack]->GetTPCncls();
    fChi2[itrack] = tpctracks[itrack]->GetTPCchi2();
    fdEdx[itrack] = tpctracks[itrack]->GetTPCsignal();
    //
    tpctracks[itrack]->GetIntegratedTimes(times);
    fTexp[itrack] = times[1];
    fLength[itrack] = tpctracks[itrack]->GetIntegratedLength();
    //
    fIndex[itrack] = tpctracks[itrack]->GetTOFCalChannel();
    fTime[itrack] = tpctracks[itrack]->GetTOFsignal();
    fRaw[itrack] = tpctracks[itrack]->GetTOFsignalRaw();
    fTOT[itrack] = tpctracks[itrack]->GetTOFsignalToT();
    //
    fDx[itrack] = tpctracks[itrack]->GetTOFsignalDx();
    fDz[itrack] = tpctracks[itrack]->GetTOFsignalDz();
    fNmatch[itrack] = tpctracks[itrack]->GetTOFclusterN();
    isTOFout[itrack] = tpctracks[itrack]->GetStatus() & AliVTrack::kTOFout;
    
  }

  fOutputTree->Fill();
  PostData(1, fOutputTree);
}
