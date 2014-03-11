#include "include/functions.h"

#include "extractValue.cpp"

bool readAME(const std::string &table,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << table << " for updated mass excess values [--";

  if(!checkFileExists(table))
    {
      std::cout << "\nERROR: Mass table " << table << " couldn't be opened." << std::endl;
      return 1;
    }

  std::ifstream file(table.c_str());

  bool rValue=0;
  int i, exp, A, Z;
  std::string *line = new std::string;
  std::vector<Nuclide>::iterator nuc_it;

  if (file.is_open())
    {
      i=0;

      while (getline(file,*line))
	{
	  if (i>38)
	    {
	      if (line->find("#") == std::string::npos)
		exp=0;
	      else
		{
		  exp=1;

		  do line->replace(line->find("#"),1," ");
		  while (line->find("#") != std::string::npos);
		}

	      extractValue(line,16,19,A);

	      extractValue(line,11,14,Z);

	      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
		{
		  if (   nuc_it->exp == exp
		      && nuc_it->A   == A
		      && nuc_it->Z   == Z
		      )
		    {
		      //-Store mass excess in member AME_ME
		      extractValue(line,29,41,nuc_it->AME_ME);

		      //-Store error on mass excess in member AME_dME
		      extractValue(line,42,53,nuc_it->AME_dME);

		      break;
		    }
		}
	    }
	  ++i;
	}
      file.close();
    }
  else
    {
      std::cout << "\n\nERROR: " << table << " couldn't be opened, does it exist?\n" << std::endl;
      rValue=1;
    }

  delete line;

  std::cout << "--] done" << std::endl;
  return rValue;
}
