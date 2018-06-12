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
  //
  Float_t fXv[2], fYv[2], fZv[2];
  Float_t fP[2], fPt[2], fEta[2], fPhi[2];
  Float_t fNcls[2], fChi2[2], fdEdx[2];
  Float_t fTexp[2], fLength[2];
  //
  Float_t fIndex[2], fTime[2], fRaw[2], fTOT[2];
  Float_t fDx[2], fDz[2], fNmatch[2];

  ClassDef(AliAnalysisTaskCosmicTOF, 1);
};

#endif /* ALIANALYSISTASKCOSMICTOF_H */
