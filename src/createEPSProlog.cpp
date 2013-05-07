#include "functions.h"

void createEPSProlog(const inputs *draw,
		     std::ofstream &out_file
		     )
{
  //-Get time/date from system
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  out_file << "%!PS-Adobe-3.0 EPSF-3.0\n"
	   << "%%Title: Nuclear Chart-";

  if      (draw->choice == "a") out_file << "Error on mass-excess-";
  else if (draw->choice == "b") out_file << "Relative error on mass-excess-";
  else if (draw->choice == "c") out_file << "Major ground-state decay mode-";
  else if (draw->choice == "d") out_file << "Ground-state half-life-";
  else                          out_file << "First isomer energy-";

  out_file << "Z=[" << draw->Zmin << "(" << convertZToSymbol(draw->Zmin) << ")," << draw->Zmax << "(" << convertZToSymbol(draw->Zmax) << ")]-N=[" << draw->Nmin << "," << draw->Nmax << "]\n"
	   << "%%BoundingBox: " << "0 0 " << ceil(draw->chart_width*draw->size) << " " << ceil(draw->chart_height*draw->size) << "\n"
	   << "%%Creator: The Interactive Nuclear CHart (INCH)\n"
	   << "%%CreationDate: " << asctime(timeinfo)
	   << "%%DocumentFonts: Times-Roman Symbol\n"
	   << "%%Page: 1\n"
	   << "%%EndComments\n"
	   << "\nsystemdict /setdistillerparams known {\n"
	   << "<< /AutoFilterColorImages false /ColorImageFilter /FlateEncode >>\n"
	   << "setdistillerparams } if\n"
	   << "\n%%BeginProlog\n"
	   << "gsave 45 dict begin\n\n"
	   << "/bd {bind def} bind def\n"
	   << "/ld {load def} bd\n"
	   << "/u {" << draw->size << "} bd\n"
	   << "/curve {" << draw->curve << "} bd\n"
	   << "/ed {exch def} bd\n"
	   << "/TR {/Times-Roman exch selectfont} bd\n"
	   << "/S {/Symbol exch selectfont} bd\n\n"
	   << "/gs /gsave ld\n"
	   << "/gr /grestore ld\n"
	   << "/m /moveto ld\n"
	   << "/l /lineto ld\n"
	   << "/rl /rlineto ld\n"
	   << "/st /stroke ld\n"
	   << "/sh /show ld\n"
	   << "/rgb /setrgbcolor ld\n"
	   << "/sl /setlinewidth ld\n\n"
	   << "/c{\n"
	   << "dup stringwidth pop\n"
	   << "-2 div 0\n"
	   << "rmoveto sh\n"
	   << "} bd\n\n"
	   << "/A{\n"
	   << "gs\n"
	   << "x sw add y sw add translate\n"
	   << "0 sw m\n"
	   << "sw neg sw sw neg 0 r arct\n"
	   << "sw neg sw neg 0 sw neg r arct\n"
	   << "sw sw neg sw 0 r arct\n"
	   << "sw sw 0 sw r arct\n"
	   << "closepath clip\n"
	   << "gs\n"
	   << "sr sg sb rgb\n"
	   << "fill\n"
	   << "} bd\n"
	   << "\n"
	   << "/B{\n"
	   << "gr\n"
	   << "0.5 u div setlinewidth\n"
	   << "black rgb\n"
	   << "st\n"
	   << "gr\n"
	   << "} bd\n"
	   << "\n"
	   << "/half{\n"
	   << "rotate\n"
	   << "black rgb\n"
	   << "-0.5 0 m\n"
	   << "0 -0.5 rl\n"
	   << "1 0 rl\n"
	   << "0 0.5 rl\n"
	   << "fill\n"
	   << "} bd\n"
	   << "\n"
	   << "/wedge{\n"
	   << "rotate\n"
	   << "black rgb\n"
	   << "-0.5 -0.5 m\n"
	   << "1 0 rl\n"
	   << "0 1 rl\n"
	   << "fill\n"
	   << "} bd\n"
	   << "\n"
	   << "/hbhalf{ gs 0 half gr } bd\n"
	   << "\n"
	   << "/vrhalf{ gs 90 half gr } bd\n"
	   << "\n"
	   << "/hthalf{ gs 180 half gr } bd\n"
	   << "\n"
	   << "/vlhalf{ gs 270 half gr } bd\n"
	   << "\n"
	   << "/brwedge{ gs 0 wedge gr } bd\n"
	   << "\n"
	   << "/trwedge{ gs 90 wedge gr } bd\n"
	   << "\n"
	   << "/tlwedge{ gs 180 wedge gr } bd\n"
	   << "\n"
	   << "/blwedge{ gs 270 wedge gr } bd\n"
	   << "\n"
	   << "/w{\n"
	   << "/num ed\n"
	   << "/sym ed\n"
	   << "3 3 roll rgb\n"
	   << "/Times-Roman 0.6 selectfont\n"
	   << "x 0.5 add y 0.2 add m\n"
	   << "sym 4 string cvs c\n"
	   << "/Times-Roman 0.35 selectfont\n"
	   << "x 0.5 add y 0.65 add m\n"
	   << "num 4 string cvs c\n"
	   << "} bd\n"
	   << "\n"
	   << "/n{\n"
	   << "/r exch 2 div def\n"
	   << "/y ed\n"
	   << "/x ed\n"
	   << "/sb ed\n"
	   << "/sg ed\n"
	   << "/sr ed\n"
	   << "/sw 1 2 div def\n"
	   << "count 1 eq\n"
	   << "{\n"
	   << "/h ed\n"
	   << "h 0 eq {A B} if\n"
	   << "h 1 eq {A hbhalf B} if\n"
	   << "h 2 eq {A hthalf B} if\n"
	   << "h 3 eq {A vlhalf B} if\n"
	   << "h 4 eq {A vrhalf B} if\n"
	   << "h 5 eq {A trwedge B} if\n"
	   << "h 6 eq {A tlwedge B} if\n"
	   << "h 7 eq {A brwedge B} if\n"
	   << "h 8 eq {A blwedge B} if\n"
	   << "h 8 gt {A B} if\n"
	   << "}\n"
	   << "{\n"
	   << "count 5 eq\n"
	   << "{A B w}\n"
	   << "{\n"
	   << "6 5 roll\n"
	   << "/h ed\n"
	   << "h 0 eq {A B w} if\n"
	   << "h 1 eq {A hthalf B w} if\n"
	   << "h 2 eq {A hbhalf B w} if\n"
	   << "h 3 eq {A vlhalf B w} if\n"
	   << "h 4 eq {A vrhalf B w} if\n"
	   << "h 5 eq {A trwedge B w} if\n"
	   << "h 6 eq {A tlwedge B w} if\n"
	   << "h 7 eq {A brwedge B w} if\n"
	   << "h 8 eq {A blwedge B w} if\n"
	   << "h 8 gt {A B w} if\n"
	   << "} ifelse\n"
	   << "} ifelse\n"
	   << "} bd\n\n"
	   << "/black     {0 0 0} bd\n"
	   << "/white     {1 1 1} bd\n"
	   << "/red       {1 0 0} bd\n"
	   << "/green     {0 1 0} bd\n"
	   << "/blue      {0 0 1} bd\n"
	   << "/yellow    {1 1 0} bd\n"
	   << "/magenta   {1 0 1} bd\n"
	   << "/cyan      {0 1 1} bd\n"
	   << "/orange    {1 0.6471 0} bd\n"
	   << "/darkgreen {0 0.3922 0} bd\n"
	   << "/navyblue  {0 0 0.5020} bd\n"
	   << "/purple    {0.6275 0.1255 0.9412} bd\n"
	   << "%%EndProlog\n"
	   << "\n%==The different ways to draw an isotope=======\n"
	   << "%\n"
	   << "%\n"
	   << "% Empty box\n"
	   << "% ---------\n"
	   << "% box_colour x y n\n"
	   << "%\n"
	   << "%\n"
	   << "% Box with the nuclei written inside\n"
	   << "% ----------------------------------\n"
	   << "% writing_colour (El) (num) box x y n\n"
	   << "%\n"
	   << "%\n"
	   << "% Empty box with half of it black\n"
	   << "% -------------------------------\n"
	   << "% x box_colour x y n\n"
	   << "%\n"
	   << "%\n"
	   << "% One black half with the nuclei written inside\n"
	   << "% ---------------------------------------------\n"
	   << "% x writing_colour (El) (num)  box_colour x y n\n"
	   << "%\n"
	   << "% Where x can take the values below to alter the\n"
	   << "% square as shown (x=0 creates a blank square).\n"
	   << "%\n"
	   << "% x    1      2      3      4\n"
	   << "%     ___    ___    ___    ___\n"
	   << "%    |###|  |   |  |#| |  | |#|\n"
	   << "%    |---|  |---|  |#| |  | |#|\n"
	   << "%    |___|  |###|  |#|_|  |_|#|\n"
	   << "%\n"
	   << "% x    5      6      7      8\n"
	   << "%     ___    ___    ___    ___\n"
	   << "%    |\\##|  |##/|  |  /|  |\\  |\n"
	   << "%    | \\#|  |#/ |  | /#|  |#\\ |\n"
	   << "%    |__\\|  |/__|  |/##|  |##\\|\n"
	   << "%\n"
	   << "%==============================================\n\n" << std::endl;
}
