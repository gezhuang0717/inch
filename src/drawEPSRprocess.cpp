#include "include/functions.h"

void drawEPSRprocess(inputs *draw,
		     std::ofstream &out_file,
		     const bool shaded
		     )
{
  //r-process data starts at Z=26.
  //no need to do anything if we aren't displaying above that.
  if (draw->Zmax < 26)
    return;

  if (shaded)
    {
      readRProcessData(draw);

      out_file << "\n%----------------------------\n"
	       << "%- r-process -- shaded path -\n"
	       << "%----------------------------\n"
	       << "gs\n"
	       << "0.9 setgray" << std::endl;
    }
  else
    {
      out_file << "\n%-----------------------------\n"
	       << "%- r-process -- path outline -\n"
	       << "%-----------------------------\n"
	       << "black rgb\n"
	       << "0.1 u div sl" << std::endl;
    }

  bool initial=true;
  std::vector<std::pair<int,int> >::iterator it;

  for (it=draw->rProcessData.begin(); it!=draw->rProcessData.end(); ++it)
    {
      //it->first = N
      //it->second = Z
      if (   it->second >= draw->Zmin
	  && it->second <= draw->Zmax
	  && it->first  >= draw->Nmin
	  && it->first  <= draw->Nmax
	  )
	{
	  out_file << std::setw(3) << it->first-draw->Nmin << " "
		   << std::setw(3) << it->second-draw->Zmin << " "
		   << (initial ? "m" : "l") << "\n";

	  initial=false;
	}
    }

  if (shaded)
    {
      out_file << "fill\n"
	       << "gr\n" << std::endl;
    }
  else
    {
      out_file << "st" << std::endl;
    }
}
