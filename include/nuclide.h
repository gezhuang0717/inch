#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>
#include <vector>
#include <cmath>
#include <cstdarg>
#include <algorithm>

#include "extractValue.h"

class Nuclide
{
public:
  Nuclide(std::string);
  ~Nuclide();

  bool own;

  int exp;
  int A;
  int Z;
  int N;
  int st;
  int pi;
  int pi_exp;
  int J_exp;
  int J_tent;
  int rich;
  int show;

  double NUBASE_ME;
  double NUBASE_dME;
  double AME_ME;
  double AME_dME;
  double s_n;
  double ds_n;
  double s_2n;
  double ds_2n;
  double s_p;
  double ds_p;
  double s_2p;
  double ds_2p;
  double dV_pn;
  double ddV_pn;
  double is_nrg;
  double dis_nrg;
  double hl;
  double J;

  std::string decay;
  std::string colour;
  std::string full_data;

  double errorQuadrature(const size_t x, ...);
  void stripHashes();

  inline void setA() {extractValue(full_data,0,3,A);}
  inline void setZ() {extractValue(full_data,4,7,Z);}
  inline void setOwn(bool val) {own=val;}
  inline void setN() {N = A-Z;}
  inline void setState() {extractValue(full_data,7,8,st);}
  inline void setNubaseMassExcess() {extractValue(full_data,18,29,NUBASE_ME);}
  inline void setNubaseMassExcessError() {extractValue(full_data,29,38,NUBASE_dME);}
  inline void setAMEMassExcess(std::string line) {extractValue(line,29,41,AME_ME);}
  inline void setAMEMassExcessError(std::string line) {extractValue(line,42,53,AME_dME);}

  void setExperimental();
  inline void setExperimental(int val) {exp=val;}

  void setSpinParity();
  void setSeparationEnergies(std::vector<Nuclide> &nuc);
  void setIsomerEnergy();
  void setHalfLife();
  void setDecayMode(std::vector<bool> &pnSide);
  void setNeutronOrProtonRich(std::vector<bool> &pnSide);
};

#endif
