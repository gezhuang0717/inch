#include "functions.h"

void writeSVG(const std::vector<Nuclide> &in, inputs *draw)
{
  std::ofstream out_file(draw->outfile.c_str());
  std::vector<Nuclide>::const_iterator nuc_it;
  std::string colour;
  //int scale=100;

  if (out_file.is_open())
    {
      out_file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
	       << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
	       << "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
	       << "width=\"" << (2+draw->Nmax-draw->Nmin)*draw->size << "px\" height=\"" << (2+draw->Zmax-draw->Zmin)*draw->size << "px\"\n"
	       << "viewbox=\"0 0 " << (2+draw->Nmax-draw->Nmin)*draw->size << " " << (2+draw->Zmax-draw->Zmin)*draw->size << "\">\n\n"
	       << "<title></title>\n"
	       << "<desc></desc>\n"
	       << "<rect x=\"0\" y=\"0\" width=\"" << (2+draw->Nmax-draw->Nmin)*draw->size << "\" height=\"" << (2+draw->Zmax-draw->Zmin)*draw->size << "\" style=\"fill:#BBFFFF\"/>\n"
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
	       << "<clipPath id=\"box\"> <rect id=\"curve\" class=\"Outline\" rx=\"" << draw->curve << "\" ry=\"" << draw->curve << "\" width=\"1\" height=\"1\" style=\"fill:none\"/> </clipPath>\n\n"
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
	       << "</defs>\n\n"
	       << "<g transform=\"translate(" << 0.5*draw->size << "," << 0.5*draw->size << ") scale(" << draw->size << "," << draw->size << ")\">\n";

      std::vector<float> n(7,0);
      std::vector<std::string> kcol(11);
      std::vector<bool> k(12,0);

      setColours(kcol,n,draw);

      drawNuclei(in,kcol,n,k,draw,out_file);

      out_file << "</g>\n</svg>" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}