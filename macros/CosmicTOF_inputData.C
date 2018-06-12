#ifndef _COSMICTOF_INPUTDATA_H_
#define _COSMICTOF_INPUTDATA_H_

TFile *fin = NULL;
TTree *tin = NULL;
Int_t nEvents = 0;
Float_t fXv[2], fYv[2], fZv[2]; // [cm]
Float_t fP[2], fPt[2], fEta[2], fPhi[2]; // [GeV/c] [rad]
Float_t fNcls[2], fChi2[2], fdEdx[2];
Float_t fTexp[2], fLength[2]; // [ps] [cm]
Float_t fIndex[2], fTime[2], fRaw[2], fTOT[2]; // TOF info [ps]
Float_t fDx[2], fDz[2], fNmatch[2]; // TOF info [cm]

Bool_t
CosmicTOF_inputData(TString finName)
{
  /** open input data **/
  fin = TFile::Open(finName);
  if (!fin || !fin->IsOpen()) {
    printf("cannot open input file: %s \n", finName.Data());
    return kFALSE;
  }
  tin = (TTree *)fin->Get("CosmicTOF/CosmicTOF");
  if (!tin) {
    printf("cannot find input tree: CosmicTOF/CosmicTOF \n");
    return kFALSE;
  }
  nEvents = tin->GetEntries();
  printf("input data successfully opened: %d events \n", nEvents);
  /** connect input **/
  tin->SetBranchAddress("xv", &fXv);
  tin->SetBranchAddress("yv", &fYv);
  tin->SetBranchAddress("zv", &fZv);
  tin->SetBranchAddress("p", &fP);
  tin->SetBranchAddress("pt", &fPt);
  tin->SetBranchAddress("eta", &fEta);
  tin->SetBranchAddress("phi", &fPhi);
  tin->SetBranchAddress("ncls", &fNcls);
  tin->SetBranchAddress("chi2", &fChi2);
  tin->SetBranchAddress("dedx", &fdEdx);
  tin->SetBranchAddress("texp", &fTexp);
  tin->SetBranchAddress("length", &fLength);
  tin->SetBranchAddress("index", &fIndex);
  tin->SetBranchAddress("time", &fTime);
  tin->SetBranchAddress("raw", &fRaw);
  tin->SetBranchAddress("tot", &fTOT);
  tin->SetBranchAddress("dx", &fDx);
  tin->SetBranchAddress("dz", &fDz);
  tin->SetBranchAddress("nmatch", &fNmatch);

  /** success **/
  return kTRUE;
}

#endif /* _COSMICTOF_INPUTDATA_H_ */

