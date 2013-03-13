///////////////////////////////////////////////////////////////
//                                                           //
// inch.cpp                                                  //
//                                                           //
///////////////////////////////////////////////////////////////
//                                                           //
// Description: This program will create a nuclear chart     //
//              containing the nuclei specified by the user, //
//              coloured by a user chosen property           //
//                                                           //
// Input:       Required - None                              //
//                                                           //
//              Optional - The name of the output file       //
//                       - An input file containing values   //
//                         used to create the chart          //
//                                                           //
//                                                           //
// Output:      An encapsulated postscript (eps) file        //
//              or a scalable vector graphics (svg) file     //
//                                                           //
///////////////////////////////////////////////////////////////

#include "inputs.h"
#include "nuclide.h"
#include "functions.h"

#include "extractValue.cpp"

int main(int argc, char *argv[])
{
  static inputs *draw = new inputs;
  static std::vector<Nuclide> nuc;
  std::vector<Nuclide>::iterator nuc_it;
  int i=0;

  std::cout << "\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~   Welcome to the Interactive Nuclear CHart   ~~\n"
	    << " ~~                 It is great                  ~~\n"
	    << " ~~               You are rubbish                ~~\n"
	    << " ~~        The Creator Ian Cullen is OK          ~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " " << std::setfill('~') << std::setw(50) << draw->version << "\n"
	    << "\n"
	    << "  USAGE: " << argv[0] << "\n"
	    << "     OR: " << argv[0] << " -i <input_file>\n"
	    << "     OR: " << argv[0] << " -o <outfile without extension>\n"
	    << "     OR: " << argv[0] << " -i <input_file> -o <outfile without extension>\n" << std::endl;

  std::string pwd = getenv("PWD");
  pwd.append("/");

  draw->path = LOCAL_PATH;
  draw->path.append("/");

  std::string input_data="data_files/";
  draw->path = draw->path.append(input_data);

  std::cout << "\nSetting the path to the required files as:\n"
	    << draw->path << "\n" << std::endl;

  draw->FRDM.insert(0,draw->path);

  draw->mass_table_AME.insert(0,draw->path);

  draw->mass_table_NUBASE.insert(0,draw->path);

  if (draw->AME)
    draw->mass_table = draw->mass_table_AME;
  else
    draw->mass_table = draw->mass_table_NUBASE;

  struct stat mass_t;

  if (stat(draw->mass_table_NUBASE.c_str(),&mass_t))
    {
      std::cout << "\nERROR: Mass table " << draw->mass_table_NUBASE << " couldn't be opened.\n"
		<< "\nExiting...\n" << std::endl;
      exit(-1);
    }

  if (draw->AME && stat(draw->mass_table_AME.c_str(),&mass_t))
    {
      std::cout << "\nERROR: Mass table " << draw->mass_table_AME << " couldn't be opened.\n"
		<< "\nExiting...\n" << std::endl;
      exit(-1);
    }

  //-------------------
  //- Read mass table -
  //-------------------
  std::cout << "Reading "
	    << draw->mass_table_NUBASE.substr(draw->path.length(),draw->mass_table_NUBASE.length()-draw->path.length())
	    << " for nuclear values <--";

  readNUBASE(draw->mass_table_NUBASE,nuc);

  if (draw->AME)
    {
      std::cout << "\nReading "
		<< draw->mass_table_AME.substr(draw->path.length(),draw->mass_table_AME.length()-draw->path.length())
		<< " for newer mass excess data [--";

      readAME(draw->mass_table_AME,nuc);

      std::cout << "--] updated\n";
    }

  std::cout << "--> done" << std::endl;

  //--------------------------------
  //- Check and validate arguments -
  //--------------------------------
  int arguments=argc;
  bool inputfile=false;

  if (arguments > 5)
    {
      char ignore;

      std::cout << "WARNING: Too many arguments given.\n"
		<< "Ignoring: ";

      for (i=5;i<arguments;++i)
	std::cout << argv[i] << " ";

      std::cout << "\nContinue ";

      do
	{
	  std::cout << "[y/n]: ";
	  std::cin  >> ignore;

	  if (ignore == 'y')
	    {
	      std::cout << "\nContinuing..." << std::endl;
	    }
	  else if (ignore == 'n')
	    {
	      std::cout << "\nExiting...\n" << std::endl;
	      exit(-1);
	    }
	  else
	    std::cout << "That wasn't y or n. Try again" << std::endl;
	}
      while (ignore != 'n' && ignore !='y');

      arguments=5;
    }

  if (arguments%2 != 1)
    {
      std::cout << "\n\n"
		<< "  ERROR: An odd number of arguments is not allowed\n\n"
		<< "  USAGE: " << argv[0] << "\n"
		<< "     OR: " << argv[0] << " -i <input_file>\n"
		<< "     OR: " << argv[0] << " -o <outfile without extension>\n"
		<< "     OR: " << argv[0] << " -i <input_file> -o <outfile without extension>\n\n" << std::endl;

      exit(-1);
    }
  else if (arguments == 1)
    {
      constructOutputFilename(draw,pwd);
    }
  else if (arguments != 1)
    {
      for (i=1;i<arguments;++i)
	{
	  if (!strcmp(argv[i],"-i"))
	    {
	      validateInputFile(nuc,draw,argv[i+1],inputfile);
	    }

	  if (!strcmp(argv[i],"-o"))
	    {
	      validateOutputFile(draw,argv[i+1],pwd);
	    }
	}
    }

  //----------------------------
  //- Read user defined nuclei -
  //----------------------------
  if (draw->own_nuclei)
    {
      draw->my_nuclei.insert(0,draw->path);

      std::cout << "Reading "
		<< draw->my_nuclei.substr(draw->path.length(),draw->my_nuclei.length()-draw->path.length())
		<< " for user selected nuclei (--";

      readOWN(draw->my_nuclei,nuc);

      std::cout << "--) done" << std::endl;
    }
  else
    {
      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
	nuc_it->own=false;

      std::cout << "Not drawing any user selected nuclei" << std::endl;
    }

  //================================================================================
  //
  // We now have a vector of nuclei, each element with members
  //
  //  int:    exp     = experiment(0) or theory/extrapolation(1)
  //          st      = state; gs=0, 1st isomer=1 etc
  //          A       = mass number
  //          Z       = proton number
  //          N       = neutron number
  //          pi      = +ve(0), -ve(1), unknown (2)
  //          pi_exp  = experimentally measured (0), extrapolated(1), unknown (2)
  //          J_exp   = experimentally measured (0), extrapolated(1), unknown (2)
  //          J_tent  = definitely assigned(0), tentatively(1), unknown (2)
  //          rich    = 'side' of the chart 6 (stable)
  //                                        2 (p_rich)
  //                                        3 (n_rich)
  //          own     = if user defined nuclei are plotted 0 (not in user file)
  //                                                       1 (in user file)
  //                                                       2 (user file is not used)
  //  float: NUBASE_ME  = mass excess from NUBASE
  //         NUBASE_dME = error on mass excess from NUBASE
  //         AME_ME     = mass excess from AME (optional)
  //         AME_dME    = error on mass excess from AME (optional)
  //         is_nrg  	= isomer energy
  //         dis_nrg 	= error on isomer energy
  //         J       	= spin of state (100 if unknown)
  //         hl      	= half-life of state (seconds)
  //
  //  string: symbol  = element symbol
  //          decay   = decay mode (gs only)
  //
  //================================================================================

  //-----------------------------------------
  //- Ask how the chart should be displayed -
  //-----------------------------------------
  if (!inputfile)
    displaySection(nuc,draw);

  std::cout << "\n===========================\n"
	    << "\nBetween Z = " << draw->Zmin << "(" << convertZToSymbol(draw->Zmin) << ") and Z = "
	    << draw->Zmax << "(" << convertZToSymbol(draw->Zmax) << ")";

  if (draw->section == "a" || (draw->section == "b" && draw->required == "a"))
    std::cout << ", with all relevant nuclei,\n";
  else if (draw->required == "b")
    std::cout << ", N = " << draw->Nmin << " and N = " << draw->Nmax << "\n";

  if      (draw->type == "a") std::cout << "experimentally measured";
  else if (draw->type == "b") std::cout << "theoretical/extrapolated";
  else                        std::cout << "both experimental and theoretical";

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if      (draw->choice == "a") std::cout << "error on mass-excess\n";
  else if (draw->choice == "b") std::cout << "relative error on mass-excess\n";
  else if (draw->choice == "c") std::cout << "major ground-state decay mode\n";
  else if (draw->choice == "d") std::cout << "ground-state half-life\n";
  else                          std::cout << "first isomer energy\n";

  //-------------------
  //- Write the chart -
  //-------------------
  std::cout << "\nCreating " << draw->outfile << "\n|--";

  if (draw->file_type == 0)
    writeEPS(nuc,draw);
  else if (draw->file_type == 1)
    writeSVG(nuc,draw);
  else if (draw->file_type == 2)
    writeTIKZ(nuc,draw);

  std::cout << "--| done\n" << std::endl;

  //-----------------------------------------------------
  //- Write chart parameters to file that can be resued -
  //-----------------------------------------------------
  draw->options.insert(0,pwd);
  std::ofstream opts(draw->options.c_str());

  if (opts)
    {
      std::cout << "Writing user choices to " << draw->options;

      opts << "section=" << draw->section << "\n";

      if (draw->section == "b")
	{
	  opts << "Zmin=" << draw->Zmin << "\n"
	       << "Zmax=" << draw->Zmax << "\n"
	       << "required=" << draw->required << "\n";

	  if (draw->required == "b")
	    opts << "Nmin=" << draw->Nmin << "\n"
		 << "Nmax=" << draw->Nmax << "\n";
	}

      opts << "type=" << draw->type << "\n"
	   << "choice=" << draw->choice << std::endl;
      opts.close();

      std::cout << " - done\n" << std::endl;
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->options << " to write the options." << std::endl;
      exit(-1);
    }

  std::cout << "Enjoy\n";

  if(!inputfile)
    std::cout << "\nTo run again with the same options: " << argv[0] << " -i options.in\n" << std::endl;

  delete draw;

  std::cout << std::endl;

  return 0;
}
