#ifndef _COSMICTOF_GEOMETRY_H_
#define _COSMICTOF_GEOMETRY_H_

AliTOFGeometry TOFgeometry;

/* 
   TOF geometry:
   18 sectors (azimuthal)
   91 strips per sector (along z direction)
   96 channels per strip
*/

void
CosmicTOF_xyz(Int_t index, Float_t *pos)
{
  Int_t det[5];
  TOFgeometry.GetVolumeIndices(index, det);
  TOFgeometry.GetPosPar(det, pos);
}

#endif /* _COSMICTOF_GEOMETRY_H_ */
