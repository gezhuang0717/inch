#include "inputs.h"

/*N.B.
inputs::convertZToSymbol() definitions are in their own files.

inputs::convertSymbolToZ() definitions are in their own files.
 */


//-----------------------------------------------------------------------
//- If the file is to be converted into some other format (eg.jpg,png), -
//- without any resizing, and the whole chart is drawn. The following  --
//- values are the largest allowed for the given page size. -------------
//-----------------------------------------------------------------------
// A0 -> size = 18
// A1 -> size = 13
// A2 -> size =  9
// A3 -> size =  6
// A4 -> size =  4
// A5 -> size =  3
//-----------------

inputs::inputs():
  //bool
  grid(false),
  magic_numbers(true),
  write_isotope(true),
  r_process(true),
  key(true),
  own_nuclei(true),
  AME(false),
  key_relative(true),                  // Not an option, just default value
  //int
  Zmin(MAX_Z),
  Zmax(MIN_Z),
  Nmin(MAX_N),
  Nmax(MIN_N),
  size(4),                             // See comment above
  experimental(0),
  np_rich(1),                          // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  single_drip_lines(1),                // 0=none, 1=both, 2=p-only, 3=n-only
  double_drip_lines(1),
  file_type(0),                        // 0=eps, 1=svg, 2=tikz
  year(3),                              // 3, 12, 16
  //double
  curve(0.25),
  key_height(0.5),
  key_scale(0.0),
  chart_height(0.0),
  chart_width(0.0),
  //std::string
  path("./"),
  mass_table(""),
  mass_table_AME("mass.mas114"),
  mass_table_NUBASE("nubtab03.asc"),
  my_nuclei("my_nuclei.dat"),
  r_proc_path("r-process.dat"),
  neutron_drip("neutron.drip"),
  proton_drip("proton.drip"),
  two_neutron_drip("2neutron.drip"),
  two_proton_drip("2proton.drip"),
  choice(""),
  required(""),
  section(""),
  type(""),
  options("options.in"),
  outfile("chart"),                    // Without extension, this is added in the code
  FRDM("FRLDM_ME.tbl"),
  version("0.9.7"),
  pwd(""),
  r_process_data()
{
}


inputs::~inputs()
{
}


void inputs::showVersion() const
{
  std::string date = __DATE__;

  std::cout << "Interactive Nuclear CHart (INCH) version " << version << "\n"
            << "Copyright (C) " << date.substr(date.rfind(' ')) << " Me.\n"
            << "INCH comes with ABSOLUTELY NO WARRANTY.\n"
            << "You may redistribute copies of INCH\n"
            << "under the terms of the GNU General Public License\n"
            << "For more information about these matters, see the file named COPYING."
            << std::endl;
}


void inputs::showUsage(std::string exe) const
{
  showBanner();

  std::cout << "\n"
            << "Usage: " << exe.substr(exe.find_last_of("/")+1) << " [with any or non of the options below]\n"
            << "\n"
            << " -h | --help\n"
            << "         Print this help and exit\n"
            << "\n"
            << " -v | --version\n"
            << "         Print version information and exit\n"
            << "\n"
            << " -i | --infile <inputFilename>\n"
            << "         Use the provided file as options to create the chart\n"
            << "\n"
            << " -o | --outfile <outputName>\n"
            << "         Set the root of the output filename i.e. without extension (default:chart)\n"
            << "\n"
            << " -f | --filetype <filetype>\n"
            << "         Set the output file type [default:eps, svg, tikz]\n"
            << "\n"
            << " -y | --year <tableYear>\n"
            << "         Set the year from which data is taken [default:3, 12, 16]\n"
            << std::endl;
}


void inputs::showBanner() const
{
  std::cout << "\n"
            << "         +---+---+---+---+---+---+---+\n"
            << "         |In |Te |Ra | C |Ti | V | E |\n"
            << "         +---+---+---+---+---+---+---+\n"
            << "             | N | U |Cl | E |Ar |\n"
            << "         +---+---+---+---+---+---+\n"
            << "         | C | H |Ar | T |\n"
            << "         +---+---+---+---v" << version << "\n"
            << std::endl;
}


void inputs::constructFullyQualifiedPaths()
{
  path = LOCAL_PATH;
  path.append("/data_files/");

  std::cout << "\nSetting the path to the data files as:\n"
            << path << "\n" << std::endl;

  FRDM.insert(0,path);

  my_nuclei.insert(0,path);

  mass_table_AME.insert(0,path);

  mass_table_NUBASE.insert(0,path);

  mass_table = AME ? mass_table_AME : mass_table_NUBASE;

  neutron_drip.insert(0,path);

  two_neutron_drip.insert(0,path);

  proton_drip.insert(0,path);

  two_proton_drip.insert(0,path);

  r_proc_path.insert(0,path);

  pwd = getenv("PWD");
  pwd.append("/");

  options.insert(0,pwd);
}


void inputs::setTableYear(const int _year)
{
  switch (_year)
    {
    default:
    case 3:
      year = 3;
      mass_table_NUBASE = "nubtab03.asc";
      mass_table_AME    = "mass.mas114";
      break;
    case 12:
      year = _year;
      mass_table_NUBASE = "nubase.mas12";
      mass_table_AME    = "mass.mas12";
      break;
    case 16:
      year = _year;
      mass_table_NUBASE = "nubase2016.txt";
      mass_table_AME    = "mass16.txt";
      break;
    }
}


bool inputs::checkInputOptions(std::map<std::string, std::string> &values)
{
  int linesRead=0;

  for ( const auto &it : values )
    {
      if ( it.first == "section" )
        {
          section = it.second;
          linesRead++;
        }
      else if ( it.first == "type" )
        {
          type=it.second;
          linesRead++;

          if ( type == "a" )
            {
              experimental = 1;
            }
          else if ( type == "b" )
            {
              experimental = 0;
            }
          else if ( type == "c" )
            {
              experimental = 2;
            }
          else
            {
              --linesRead;
              std::cout << "***ERROR***: " << type
                        << " is not a valid choice for 'type'" << std::endl;
            }
        }
      else if ( it.first == "choice" )
        {
          choice = it.second;
          linesRead++;
        }
      else if ( it.first == "required" )
        {
          required = it.second;
        }
      else if ( it.first == "Zmin" )
        {
          Zmin = atoi(it.second.c_str());

          if (   (!atoi(it.second.c_str()) && it.second!="0")
              || (Zmin<MIN_Z || Zmin>MAX_Z)
              )
            {
              std::cout << "***ERROR***: " << it.second
                        << " is not a valid choice for 'Zmin'" << std::endl;
              return false;
            }
          else
            {
              linesRead++;
            }
        }
      else if ( it.first == "Zmax" )
        {
          Zmax = atoi(it.second.c_str());

          if (   (!atoi(it.second.c_str()) && it.second!="0")
              || (Zmax<MIN_Z || Zmax>MAX_Z)
              )
            {
              std::cout << "***ERROR***: " << it.second
                        << " is not a valid choice for 'Zmax'" << std::endl;
              return false;
            }
          else
            {
              linesRead++;
            }
        }
      else if ( it.first == "Nmin" )
        {
          Nmin = atoi(it.second.c_str());

          if (   (!atoi(it.second.c_str()) && it.second!="0")
              || (Nmin<MIN_N || Nmin>MAX_N)
              )
            {
              std::cout << "***ERROR***: " << it.second
                        << " is not a valid choice for 'Nmin'" << std::endl;
              return false;
            }
          else
            {
              linesRead++;
            }
        }
      else if ( it.first == "Nmax" )
        {
          Nmax = atoi(it.second.c_str());

          if (   (!atoi(it.second.c_str()) && it.second != "0")
              || (Nmax<MIN_N || Nmax>MAX_N)
              )
            {
              std::cout << "***ERROR***: " << it.second
                        << " is not a valid choice for 'Nmax'" << std::endl;
              return false;
            }
          else
            {
              linesRead++;
            }
        }
      else
        {
          std::cout << "**WARNING**: " << it.first
                    <<" is not a valid option. Ignoring." << std::endl;
        }
    }

  if ( linesRead < 3 )
    {
      std::cout << "Not enough inputs have been read from the file." << std::endl;
      return false;
    }

  return true;
}


void inputs::setCanvasSize()
{
  if ( key_height*key_scale > (Zmax-Zmin+2) )
    {
      key_relative = true;
      chart_height = (key_height+1.0)*key_scale;
    }
  else
    {
      chart_height = Zmax-Zmin+2;
    }

  //HACKS
  //- When all nuclei are drawn, key is in top left.
  //Below stops extra space being created on the right.
  //- 14.5*key_scale extends the width to fit the widest key
  //this should really be set as a function of the variable
  //used to colour the isotopes. Either way, this cannot be
  //set dynamically in the file so we need to use 'magic numbers'
  if ( section == "a" || (Zmax-Zmin) == MAX_Z )
    {
      chart_width = Nmax-Nmin+2;
    }
  else
    {
      chart_width = (Nmax-Nmin+2) + (14.5*key_scale);
    }
}


void inputs::setExtreme(const std::string &limit)
{
  if ( limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax" )
    {
      std::cout << "**WARNING** - " << limit << " is not a valid input\n"
                << "              choose either Zmin, Zmax, Nmin or Nmax\n"
                << "Setting limits to maxima values." << std::endl;

      Zmin=MIN_Z;
      Zmax=MAX_Z;
      Nmin=MIN_N;
      Nmax=MAX_N;

      return;
    }

  bool valid=false;

  do
    {
      int number=0;
      std::string in;

      std::cout << limit << ": ";
      std::cin >> in;

      //Read the entered value and convert symbol->Z if necessary
      if ( in == "0" )
        {
          number = 0;
        }
      else
        {
          if ( limit.at(0) == 'Z' )
            {
              number = atoi(in.c_str()) ? atoi(in.c_str()) : convertSymbolToZ(in);
            }
          else if ( !atoi(in.c_str()) )
            {
              number = -1;
            }
          else
            {
              number = atoi(in.c_str());
            }
        }

      //Validate the number
      if ( limit == "Zmin" )
        {
          Zmin = number;
          valid = true;
          if ( Zmin < MIN_Z || Zmin > MAX_Z )
            {
              std::cout << "\n"
                        << "Zmin must be in the range " << MIN_Z << "<Z<" << MAX_Z
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Zmax" )
        {
          Zmax = number;
          valid = true;
          if ( Zmax < Zmin || Zmax > MAX_Z )
            {
              std::cout << "\n"
                        <<"Zmax must be in the range " << Zmin << "<Z<" << MAX_Z
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Nmin" )
        {
          Nmin = number;
          valid = true;
          if ( Nmin < MIN_N || Nmin > MAX_N )
            {
              std::cout << "\n"
                        << "Nmin must be in the range " << MIN_N << "<N<" << MAX_N
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Nmax" )
        {
          Nmax = number;
          valid = true;
          if ( Nmax < Nmin || Nmax > MAX_N )
            {
              std::cout << "\n"
                        << "Nmax must be in the range " << Nmin << "<N<" << MAX_N
                        << "\n" << std::endl;
              valid=false;
            }
        }
    }
  while ( !valid );
}


void inputs::showChartOptions() const
{
  std::cout << "===========================\n"
            << "\nBetween Z = " << Zmin << "(" << convertZToSymbol(Zmin)
            << ") and Z = " << Zmax << "(" << convertZToSymbol(Zmax) << ")";

  if ( section == "a" || (section == "b" && required == "a") )
    {
      std::cout << ", with all relevant nuclei,\n";
    }
  else if ( required == "b" )
    {
      std::cout << ", N = " << Nmin << " and N = " << Nmax << "\n";
    }

  if      ( type == "a" ) std::cout << "experimentally measured";
  else if ( type == "b" ) std::cout << "theoretical/extrapolated";
  else                    std::cout << "both experimental and theoretical";

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if      ( choice == "a" ) std::cout << "error on mass-excess\n";
  else if ( choice == "b" ) std::cout << "relative error on mass-excess\n";
  else if ( choice == "c" ) std::cout << "major ground-state decay mode\n";
  else if ( choice == "d" ) std::cout << "ground-state half-life\n";
  else                      std::cout << "first isomer energy\n";
}


void inputs::constructOutputFilename()
{
  //-Remove the extension if given
  if (   outfile.find(".") == outfile.length()-4
      && outfile.find(".") != std::string::npos
      )
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      outfile.erase(outfile.rfind("."),4);
    }

  //-Remove the CWD if given
  if ( outfile.find(pwd) != std::string::npos )
    {
      std::cout << "\nThe current working directory is added\n";

      outfile.erase(0,outfile.rfind("/")+1);
    }

  //-Check output file is not a directory.
  if (   outfile.empty()
      || outfile.at(outfile.length()-1) == '/'
      )
    {
      std::cout << "\n"
                << "***ERROR***: "
                << outfile << " is a directory, can't use that as a file name\n"
                << std::endl;

      outfile = "chart";
    }

  std::cout << "Using \"" << outfile << "\" as the base of the file name." << std::endl;

  //-Add the necessary extension
  switch (file_type)
    {
    case 0:
      outfile.append(".eps");
      break;
    case 1:
      outfile.append(".svg");
      break;
    case 2:
      outfile.append(".tex");
      break;
    }
}


void inputs::writeOptionFile()
{
  //Match the options filename to that of the output chart
  if ( outfile.find("chart") == std::string::npos )
    {
      options = outfile.substr(0,outfile.rfind("."))+ ".in";
    }

  std::ofstream opts(options.c_str());

  std::cout << "Writing user choices to " << options;

  if ( !opts )
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << options << " to write the options.\n"
                << "             Not creating an option file." << std::endl;

      return;
    }

  opts << "section=" << section << "\n";

  if ( section == "b" )
    {
      opts << "Zmin=" << Zmin << "\n"
           << "Zmax=" << Zmax << "\n"
           << "required=" << required << "\n";

      if ( required == "b" )
        {
          opts << "Nmin=" << Nmin << "\n"
               << "Nmax=" << Nmax << "\n";
        }
    }

  opts << "type=" << type << "\n"
       << "choice=" << choice << std::endl;

  opts.close();

  std::cout << " - done\n" << std::endl;
}
