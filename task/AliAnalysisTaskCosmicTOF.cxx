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
  fOutputTree(NULL)
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
  fOutputTree->Branch("xv", &fXv, "xv[2]/F");
  fOutputTree->Branch("yv", &fYv, "yv[2]/F");
  fOutputTree->Branch("zv", &fZv, "zv[2]/F");
  //
  fOutputTree->Branch("p", &fP, "p[2]/F");
  fOutputTree->Branch("pt", &fPt, "pt[2]/F");
  fOutputTree->Branch("eta", &fEta, "eta[2]/F");
  fOutputTree->Branch("phi", &fPhi, "phi[2]/F");
  //
  fOutputTree->Branch("ncls", &fNcls, "ncls[2]/F");
  fOutputTree->Branch("chi2", &fChi2, "chi2[2]/F");
  fOutputTree->Branch("dedx", &fdEdx, "dedx[2]/F");
  //
  fOutputTree->Branch("texp", &fTexp, "texp[2]/F");
  fOutputTree->Branch("length", &fLength, "tength[2]/F");
  //
  fOutputTree->Branch("index", &fIndex, "index[2]/F");
  fOutputTree->Branch("time", &fTime, "time[2]/F");
  fOutputTree->Branch("raw", &fRaw, "raw[2]/F");
  fOutputTree->Branch("tot", &fTOT, "tot[2]/F");
  //
  fOutputTree->Branch("dx", &fDx, "dx[2]/F");
  fOutputTree->Branch("dz", &fDz, "dz[2]/F");
  fOutputTree->Branch("nmatch", &fNmatch, "nmatch[2]/F");

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

  /** loop over ESD tracks **/
  Int_t ntracks = esd->GetNumberOfTracks();
  Int_t ntpctracks = 0;
  AliESDtrack *tpctracks[1024];
  for (Int_t itrack = 0; itrack < ntracks; itrack++) {
    AliESDtrack *track = esd->GetTrack(itrack);
    if (!(track->GetStatus() & AliESDtrack::kTPCrefit)) continue;
    tpctracks[ntpctracks++] = track;
    if (ntpctracks > 2) break;
  }
  if (ntpctracks != 2) return;

  /** save info of selected event **/
  Double_t times[10];
  for (Int_t itrack = 0; itrack < ntpctracks; itrack++) {
    //
    fXv[itrack] = tpctracks[itrack]->Xv();
    fYv[itrack] = tpctracks[itrack]->Yv();
    fZv[itrack] = tpctracks[itrack]->Zv();
    //
    fP[itrack] = tpctracks[itrack]->P();
    fPt[itrack] = tpctracks[itrack]->Pt();
    fEta[itrack] = tpctracks[itrack]->Eta();
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

  }

  fOutputTree->Fill();
  PostData(1, fOutputTree);
}
