#ifndef ALIANALYSISTASKCOSMICTOF_H
#define ALIANALYSISTASKCOSMICTOF_H

/*
 * CosmicTOF Analysis
 * author: Roberto Preghenella (R+)
 * email:  preghenella@bo.infn.it
 *
 */

#include "AliAnalysisTaskSE.h"

class AliESDtrackCuts;
class TList;
class TTree;

class AliAnalysisTaskCosmicTOF : public AliAnalysisTaskSE
{
  
 public:
  
  AliAnalysisTaskCosmicTOF(const Char_t *name = "CosmicTOF"); // default constructor
  virtual ~AliAnalysisTaskCosmicTOF(); // default destructor
  
  void UserCreateOutputObjects();
  void UserExec(Option_t *option);
  
 private:
  
  AliAnalysisTaskCosmicTOF(const AliAnalysisTaskCosmicTOF &); // not implemented
  AliAnalysisTaskCosmicTOF &operator=(const AliAnalysisTaskCosmicTOF &); // not implemented

  TTree *fOutputTree;
  Int_t fNtrk;
  UInt_t fTstamp;
  ULong64_t fTmask;
  //
  Float_t fXv[1024], fYv[1024], fZv[1024];
  Float_t fP[1024], fPt[1024], fTheta[1024], fPhi[1024];
  Int_t fNcls[1024];
  Float_t fChi2[1024], fdEdx[1024];
  Float_t fTexp[1024], fLength[1024];
  //
  Int_t fIndex[1024];
  Float_t fTime[1024], fRaw[1024], fTOT[1024];
  Float_t fDx[1024], fDz[1024];
  Int_t fNmatch[1024];

  ClassDef(AliAnalysisTaskCosmicTOF, 1);
};

#endif /* ALIANALYSISTASKCOSMICTOF_H */
