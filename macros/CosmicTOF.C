#include "CosmicTOF_inputData.C"
#include "CosmicTOF_geometry.C"

void
CosmicTOF(TString finName = "CosmicTOF.root", TString xyzName = "xyztof.root", TString foutName = "CosmicTOF.out.root")
{

  /** open and connect input data **/
  if (!CosmicTOF_inputData(finName)) return;

  /** histograms **/
  TH2F *hBetaP = new TH2F("hBetaP", ";p (GeV/c);v/c", 1000, 0., 100., 500, 0.6, 1.1); 

  TH1 *hTOF = new TH1F("hTOF", "", 1000, -500000., 500000.);
  
  /** loop over data **/
  for (Int_t iev = 0; iev < nEvents; iev++) {

    /** get entry **/
    tin->GetEntry(iev);
    
    /** define top/bottom track index **/
    Int_t top, bottom;
    if (fPhi[0] < TMath::Pi() && fPhi[1] > TMath::Pi()) {
      top = 0;
      bottom = 1;
    }
    else if (fPhi[1] < TMath::Pi() && fPhi[0] > TMath::Pi()) {
      top = 1;
      bottom = 0;
    }
    else continue;

    /** cosmic-ray selection **/
    
    /** compute momentum **/
    Float_t p = 0.5 * (fP[top] + fP[bottom]);
    
    /** TOF match requirements **/
    if (fNmatch[top] < 1 || fNmatch[bottom] < 1) continue;
    
    /** get time-of-flight **/
    Float_t TOF = fTime[bottom] - fTime[top]; // [ps]

    /** compute length (reconstructed) **/
    Float_t length = fLength[bottom] + fLength[top]; // [cm]
    /** compute speed (reconstructed) **/
    Float_t beta = length / TOF / 2.99792458e-2;
    
    /** compute TOF length (straight line approximation) **/
    Float_t xyz_top[3], xyz_bottom[3];
    CosmicTOF_xyz(fIndex[top], xyz_top);
    CosmicTOF_xyz(fIndex[bottom], xyz_bottom);
    Float_t TOFlength = 0.; // [cm]
    for (Int_t i = 0; i < 3; i++)
      TOFlength += (xyz_bottom[i] - xyz_top[i]) * (xyz_bottom[i] - xyz_top[i]);
    TOFlength = TMath::Sqrt(TOFlength);
    /** compute speed (straight line approximation) **/
    Float_t TOFbeta = TOFlength / TOF / 2.99792458e-2;
    
    hBetaP->Fill(p, TOFbeta);
    hTOF->Fill(TOF);
    
  } /** end of loop over data **/

  hBetaP->Draw("colz");
  //  hTOF->Draw();
}			  


