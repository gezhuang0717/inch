#include "inch/prolog.hpp"

#include "inch/chartColour.hpp"
#include "inch/converter.hpp"
#include "inch/options.hpp"


void Prolog::EPSWriteProlog(std::ofstream& outFile, const Options& draw) const
{
  outFile << "%!PS-Adobe-3.0 EPSF-3.0\n"
          << "%%Title: Nuclear Chart - ";

  if (draw.chart_colour == ChartColour::MASSEXCESSERROR)
    {
      outFile << "Error on mass-excess -";
    }
  else if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR)
    {
      outFile << "Relative error on mass-excess -";
    }
  else if (draw.chart_colour == ChartColour::GS_DECAYMODE)
    {
      outFile << "Major ground-state decay mode -";
    }
  else if (draw.chart_colour == ChartColour::GS_HALFLIFE)
    {
      outFile << "Ground-state half-life -";
    }
  else
    {
      outFile << "First isomer energy -";
    }

  outFile << " Z=[" << draw.Zmin << "(" << Converter::convertZToSymbol(draw.Zmin) << ")," << draw.Zmax << "("
          << Converter::convertZToSymbol(draw.Zmax) << ")] - N=[" << draw.Nmin << "," << draw.Nmax << "]\n"
          << "%%BoundingBox: (atend)\n"
          << "%%Creator: The Interactive Nuclear CHart (INCH)\n"
          << "%%CreationDate: " << getTime() << "\n"
          << "%%DocumentFonts: Times-Roman Symbol\n"
          << "%%Page: 1\n"
          << "%%EndComments\n"
          << "\n"
          << "systemdict /setdistillerparams known {\n"
          << "<< /AutoFilterColorImages false /ColorImageFilter /FlateEncode >>\n"
          << "setdistillerparams } if\n"
          << "\n%%BeginProlog\n"
          << "gsave 45 dict begin\n\n"
          << "/bd {bind def} bind def\n"
          << "/ld {load def} bd\n"
          << "/u {" << chart_size << "} bd\n"
          << "/curve {" << curve << "} bd\n"
          << "/BoxWidth {0.5} bd\n"
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
          << "/CenteredText{\n"
          << "dup stringwidth pop\n"
          << "-2 div 0\n"
          << "rmoveto sh\n"
          << "} bd\n\n"
          << "/half{\n"
          << "rotate\n"
          << "black rgb\n"
          << "BoxWidth neg 0 m\n"
          << "0 BoxWidth neg rl\n"
          << "1 0 rl\n"
          << "0 BoxWidth rl\n"
          << "fill\n"
          << "} bd\n"
          << "\n"
          << "/wedge{\n"
          << "rotate\n"
          << "black rgb\n"
          << "BoxWidth neg BoxWidth neg m\n"
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
          << "/Background{\n"
          << "gs\n"
          << "x BoxWidth add y BoxWidth add translate\n"
          << "0 BoxWidth m\n"
          << "BoxWidth neg BoxWidth BoxWidth neg 0 CornerCurve arct\n"
          << "BoxWidth neg BoxWidth neg 0 BoxWidth neg CornerCurve arct\n"
          << "BoxWidth BoxWidth neg BoxWidth 0 CornerCurve arct\n"
          << "BoxWidth BoxWidth 0 BoxWidth CornerCurve arct\n"
          << "closepath clip\n"
          << "gs\n"
          << "BackgroundRed BackgroundGreen BackgroundBlue rgb\n"
          << "fill\n"
          << "} bd\n"
          << "\n"
          << "/Foreground{\n"
          << "gr\n"
          << "0.5 u div setlinewidth\n"
          << "black rgb\n"
          << "st\n"
          << "gr\n"
          << "} bd\n"
          << "\n"
          << "/WriteIsotope{\n"
          << "/num ed\n"
          << "/sym ed\n"
          << "3 3 roll rgb\n"
          << "0.6 TR\n"
          << "x 0.5 add y 0.2 add m\n"
          << "sym 4 string cvs CenteredText\n"
          << "0.35 TR\n"
          << "x 0.5 add y 0.65 add m\n"
          << "num 4 string cvs CenteredText\n"
          << "} bd\n"
          << "\n"
          << "/Nucleus{\n"
          << "/CornerCurve exch 2 div def\n"
          << "/y ed\n"
          << "/x ed\n"
          << "/BackgroundBlue ed\n"
          << "/BackgroundGreen ed\n"
          << "/BackgroundRed ed\n"
          << "count 1 eq\n"
          << "{\n"
          << "/h ed\n"
          << "h 0 eq {Background Foreground} if\n"
          << "h 1 eq {Background hbhalf Foreground} if\n"
          << "h 2 eq {Background hthalf Foreground} if\n"
          << "h 3 eq {Background vlhalf Foreground} if\n"
          << "h 4 eq {Background vrhalf Foreground} if\n"
          << "h 5 eq {Background trwedge Foreground} if\n"
          << "h 6 eq {Background tlwedge Foreground} if\n"
          << "h 7 eq {Background brwedge Foreground} if\n"
          << "h 8 eq {Background blwedge Foreground} if\n"
          << "h 8 gt {Background Foreground} if\n"
          << "}\n"
          << "{\n"
          << "count 5 eq\n"
          << "{Background Foreground WriteIsotope}\n"
          << "{\n"
          << "6 5 roll\n"
          << "/h ed\n"
          << "h 0 eq {Background Foreground WriteIsotope} if\n"
          << "h 1 eq {Background hthalf Foreground WriteIsotope} if\n"
          << "h 2 eq {Background hbhalf Foreground WriteIsotope} if\n"
          << "h 3 eq {Background vlhalf Foreground WriteIsotope} if\n"
          << "h 4 eq {Background vrhalf Foreground WriteIsotope} if\n"
          << "h 5 eq {Background trwedge Foreground WriteIsotope} if\n"
          << "h 6 eq {Background tlwedge Foreground WriteIsotope} if\n"
          << "h 7 eq {Background brwedge Foreground WriteIsotope} if\n"
          << "h 8 eq {Background blwedge Foreground WriteIsotope} if\n"
          << "h 8 gt {Background Foreground WriteIsotope} if\n"
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
          << "%==============================================\n"
          << std::endl;
}


void Prolog::TIKZWriteProlog(std::ofstream& outFile /*, std::unique_ptr<Options> &draw*/) const
{
  outFile << "\\documentclass{article}\n"
          << "\\usepackage{tikz}\n"
          << "\\usepackage[T1]{fontenc}\n"
          << "\\usepackage[active,tightpage]{preview}\n"
          << "\\PreviewEnvironment{tikzpicture}\n"
          << "\n"
          << "%Setup command to draw box and text\n"
          << R"(\newcommand{\nucleus}[6][)" << curve << "]{\n"
          << "\\pgfmathsetmacro{\\rc}{#1*sqrt(200)}\n"
          << "\\filldraw[draw=black,thick,fill=#2,rounded corners=\\rc] (#3,#4) rectangle +(1,1)\n"
          << "+(0.5,0.75) node[anchor=mid,text=black] {#5}\n"
          << "+(0.5,0.27) node[anchor=mid,text=black] {\\Large #6};\n"
          << "}\n"
          << std::endl;
}


void Prolog::SVGWriteProlog(std::ofstream& outFile, const Options& draw) const
{
  outFile << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" "
             "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
          << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
          << "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
          << R"(width=")" << (2 + draw.Nmax - draw.Nmin) * chart_size << R"(px" height=")"
          << (2 + draw.Zmax - draw.Zmin) * chart_size << "px\"\n"
          << R"(viewbox="0 0 )" << (2 + draw.Nmax - draw.Nmin) * chart_size << " "
          << (2 + draw.Zmax - draw.Zmin) * chart_size << "\">\n"
          << "\n"
          << "<title></title>\n"
          << "<desc></desc>\n"
          << R"(<rect x="0" y="0" width=")" << (2 + draw.Nmax - draw.Nmin) * chart_size << R"(" height=")"
          << (2 + draw.Zmax - draw.Zmin) * chart_size << "\" style=\"fill:#BBFFFF\"/>\n"
          << "<style type=\"text/css\" >\n"
          << "<![CDATA[\n"
          << ".Red {fill:red}\n"
          << ".Green {fill:green}\n"
          << ".Blue {fill:blue}\n"
          << ".Black {fill:black}\n"
          << ".White {fill:white}\n"
          << ".Yellow {fill:yellow}\n"
          << ".Magenta {fill:magenta}\n"
          << ".Cyan {fill:cyan}\n"
          << ".Pink {fill:pink}\n"
          << ".Orange {fill:orange}\n"
          << ".Navy {fill:navy}\n"
          << ".Purple {fill:purple}\n"
          << ".DarkGreen {fill:darkgreen}\n"
          << ".Outline {stroke:black; stroke-width:0.1}\n"
          << ".Seri {font-weight:normal; font-family:serif; text-decoration:none}\n"
          << ".MidSymbol {font-size:0.6; text-anchor:middle}\n"
          << ".MidNumber {font-size:0.35; text-anchor:middle}\n"
          << ".Clip {clip-path:url(#box)}\n"
          << "]]>\n"
          << "</style>\n\n"
          << "<defs>\n"
          << R"(<clipPath id="box"> <rect id="curve" class="Outline" rx=")" << curve << R"(" ry=")" << curve
          << "\" width=\"1\" height=\"1\" style=\"fill:none\"/> </clipPath>\n\n"
          << "<g id=\"redNucleus\"> <use xlink:href=\"#curve\" class=\"Red Clip\"/> </g>\n"
          << "<g id=\"greenNucleus\"> <use xlink:href=\"#curve\" class=\"Green Clip\"/> </g>\n"
          << "<g id=\"blueNucleus\"> <use xlink:href=\"#curve\" class=\"Blue Clip\"/> </g>\n"
          << "<g id=\"yellowNucleus\"> <use xlink:href=\"#curve\" class=\"Yellow Clip\"/> </g>\n"
          << "<g id=\"cyanNucleus\"> <use xlink:href=\"#curve\" class=\"Cyan Clip\"/> </g>\n"
          << "<g id=\"magentaNucleus\"> <use xlink:href=\"#curve\" class=\"Magenta Clip\"/> </g>\n"
          << "<g id=\"orangeNucleus\"> <use xlink:href=\"#curve\" class=\"Orange Clip\"/> </g>\n"
          << "<g id=\"whiteNucleus\"> <use xlink:href=\"#curve\" class=\"White Clip\"/> </g>\n"
          << "<g id=\"blackNucleus\"> <use xlink:href=\"#curve\" class=\"Black Clip\"/> </g>\n"
          << "<g id=\"navyblueNucleus\"> <use xlink:href=\"#curve\" class=\"Navy Clip\"/> </g>\n"
          << "<g id=\"darkgreenNucleus\"> <use xlink:href=\"#curve\" class=\"DarkGreen Clip\"/> </g>\n"
          << "<g id=\"purpleNucleus\"> <use xlink:href=\"#curve\" class=\"Purple Clip\"/> </g>\n\n"
          << "<g id=\"TopHorizontalHalf\"> <path class=\"Black Clip\" d=\"M 0 0 h 1 v 0.5 h -1 Z\"/> </g>\n"
          << "<g id=\"BottomHorizontalHalf\"> <path class=\"Black Clip\" d=\"M 0 1 h 1 v -0.5 h -1 Z\"/> </g>\n"
          << "<g id=\"LeftVerticalHalf\"> <path class=\"Black Clip\" d=\"M0 0 h 0.5 v 1 h -0.5 Z\"/> </g>\n"
          << "<g id=\"RightVerticalHalf\"> <path class=\"Black Clip\" d=\"M 0.5 0 h 0.5 v 1 h -0.5 Z\"/> </g>\n\n"
          << "<g id=\"TopLeftWedge\"> <path class=\"Black Clip\" d=\"M 1 0 h -1 v 1 Z\"/> </g>\n"
          << "<g id=\"TopRightWedge\"> <path class=\"Black Clip\" d=\"M 0 0 h 1 v 1 Z\"/> </g>\n"
          << "<g id=\"BottomLeftWedge\"> <path class=\"Black Clip\" d=\"M 0 0 v 1 h 1 Z\"/> </g>\n"
          << "<g id=\"BottomRightWedge\"> <path class=\"Black Clip\" d=\"M 0 1 h 1 v -1 Z\"/> </g>\n"
          << "</defs>\n"
          << std::endl;
}
