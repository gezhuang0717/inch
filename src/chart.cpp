#include "inch/chart.hpp"

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/dripline.hpp"
#include "inch/fileType.hpp"
#include "inch/grid.hpp"
#include "inch/key.hpp"
#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"
#include "inch/nuclide.hpp"
#include "inch/options.hpp"
#include "inch/prolog.hpp"
#include "inch/rProcess.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cmath>
#include <string>


void Chart::write(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const
{
  fmt::print("\nCreating {} \n|--\n", draw.outfile);

  switch (draw.filetype)
    {
      case FileType::EPS:
        writeEPS(nuc, draw, part);
        break;
      case FileType::SVG:
        writeSVG(nuc, draw);
        break;
      case FileType::TIKZ:
        writeTIKZ(nuc, draw);
        break;
    }

  fmt::print("\n--| done\n");
}


void Chart::setCanvasSize(const double key_scale, const double key_height, const Options& draw) const
{
  constexpr double BORDER{ 1.0 };
  height = draw.limits.getZRange() + 2 * BORDER;

  if (key_height * key_scale > height)
    {
      key_relative = true;
      height       = (key_height + BORDER) * key_scale;
    }

  // HACKS
  // When all nuclei are drawn, key is in top left.
  // The below stops extra space being created on the right.
  //
  // max_key_width*scale extends the width to fit the widest key
  // This should really be set as a function of the variable
  // used to colour the isotopes. Either way, this cannot be
  // set dynamically in the file so we need to use 'magic numbers'
  width = draw.limits.getNRange() + 2 * BORDER;

  if (draw.chart_selection != ChartSelection::FULL_CHART && draw.limits.getZRange() < Limits::MAX_Z)
    {
      width += (max_key_width * key_scale);
    }
}


void Chart::drawNuclei(const std::vector<Nuclide>& in, const Options& draw, std::ostream& outFile) const
{
  for (const auto& it : in)
    {
      if (it.show == 1)
        {
          if (draw.filetype == FileType::EPS)
            {
              // Set how the shape representing the isotope is displayed
              const int isotope_display = [&]() {
                return (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY && it.decay == "stable") ? 1
                                                                                                      : it.own ? 8 : 0;
              }();

              // Set the text, if it is displayed
              const std::string writing_colour = [&]() {
                if (draw.write_isotope)
                  {
                    std::string text_colour{ "black" };
                    // If the square is coloured black, change text colour to white
                    if (it.colour == "black")
                      {
                        // but if it's user defined, use red
                        text_colour = it.own ? "red" : "white";
                      }
                    else if (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY && it.decay == "stable")
                      {
                        text_colour = "white";
                      }

                    return fmt::format("{} ({}) ({})", text_colour, it.symbol, it.A);
                  }

                return std::string{ "" };
              }();

              // Everything is set, draw the isotope
              // Add comment in the eps file with isotope ID
              // This is for easy navigation if manually altering the file
              fmt::print(outFile,
                         "%{}{}\n{} {} {} {} {} curve Nucleus\n",
                         it.A,
                         it.symbol,
                         isotope_display,
                         writing_colour,
                         it.colour,
                         (it.N - draw.limits.Nmin),
                         (it.Z - draw.limits.Zmin));
            }
          else if (draw.filetype == FileType::SVG)
            {
              fmt::print(outFile,
                         "<!--{}{}-->\n<g transform=\"translate({} {})\"> <use xlink:href=\"#{}Nucleus\"/></g>\n",
                         it.A,
                         it.symbol,
                         (it.N - draw.limits.Nmin),
                         (draw.limits.Zmax - it.Z),
                         it.colour);
              //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << it.symbol << "</text> "
              //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << it.A << "</text></g>" << std::endl;
            }
          else if (draw.filetype == FileType::TIKZ)
            {
              // Watch out for matching {}'s
              // You need {{ to print { in fmt, but also {N} for the nth thing to print
              fmt::print(outFile,
                         "%{0}{1}\n\\nucleus{{{2}}}{{{3}}}{{{4}}}{{{0}}}{{{1}}}\n",
                         it.A,
                         it.symbol,
                         it.colour,
                         it.N,
                         it.Z);
            }
        }
      else if (it.show == 2)
        {
          if (it.decay == "stable")
            {
              it.colour = "black";
            }

          if (draw.filetype == FileType::EPS)
            {
              // Set the text, if it is displayed
              const std::string writing_colour = [&]() {
                if (draw.write_isotope)
                  {
                    // If the square is coloured black change the text to white
                    // unless it a user isotope, in which case red
                    const std::string text_colour = (it.colour == "black") ? it.own ? "red" : "white" : "black";

                    return fmt::format("{} ({}) ({})", text_colour, it.symbol, it.A);
                  }

                return std::string{ "" };
              }();

              fmt::print(outFile,
                         "%{}{}\n0 {} {} {} {} curve Nucleus\n",
                         it.A,
                         it.symbol,
                         writing_colour,
                         it.colour,
                         (it.N - draw.limits.Nmin),
                         (it.Z - draw.limits.Zmin));
            }
          // Comment back in when appropriate
          // else if (draw.filetype == FileType::SVG)
          //  {
          //  }
          // else if (draw.filetype == FileType::TIKZ)
          //  {
          //  }
        }
    }
}


void Chart::writeEPS(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const
{
  // Open the output file we are going to use
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart\n", draw.outfile);
      return;
    }

  // Create and write the prolog, setting up definitions
  // and functions that will be used
  const Prolog setup(size);
  fmt::print(outFile, "{}", setup.EPSWriteProlog(draw));

  // Set the scale and an outer border of half a unit.
  fmt::print(outFile,
             "u dup scale\n"
             "0.5 dup translate\n");

  // If key is taller than chart, shift chart to be centered in y.
  const Key theKey;
  theKey.setScale(draw, part);

  setCanvasSize(theKey.scale, theKey.height, draw);

  // For positioning and alignment,
  // draw a grid, default spacing is 5 units.
  if (draw.grid)
    {
      const Grid grid;
      fmt::print(
          outFile, "{}", grid.EPSDrawGrid(static_cast<int>(std::floor(width)), static_cast<int>(std::floor(height))));
    }

  if (key_relative)
    {
      fmt::print(outFile,
                 "\n%Shift coordinates so chart is vertically centered\n"
                 "gs\n"
                 "0 {} translate\n",
                 0.5 * (height - (draw.limits.getZRange() + 2)));
    }

  // r-process - shaded
  // Postscript doesn't support transparency so draw shaded
  // area of the r-process before nuclei and the outline after.
  //
  // We can't create the instance in the if condition below, as
  // it would then go out of scope and we would have to create it
  // again to draw the outline.
  const rProcess rProc(draw.limits);

  if (draw.r_process && draw.limits.Zmax > rProcess::min_Z)
    {
      rProc.setRProcessFile(draw.r_proc_path);
      rProc.readData();
      rProc.EPSWritePath(outFile, true);
    }

  //---------------
  //- Draw Nuclei -
  //---------------
  drawNuclei(nuc, draw, outFile);

  // Magic numbers
  if (draw.magic_numbers)
    {
      const MagicNumbers magic(draw.limits);

      fmt::print(outFile, "{}", magic.EPSSetup());

      for (const auto val : magic.numbers)
        {
          if (draw.limits.inZRange(val))
            {
              fmt::print(outFile, "{}", magic.EPSWriteProtonNumber(val));
            }

          if (draw.limits.inNRange(val))
            {
              fmt::print(outFile, "{}", magic.EPSWriteNeutronNumber(val));
            }
        }

      fmt::print(outFile, "{}", magic.EPSTearDown());
    }
  else
    {
      fmt::print("\nNot drawing the magic numbers");
    }

  // Drip lines
  // NUBASE has units of keV, we need MeV once we eventually use these values.
  // If we convert here then we can pass them as const
  if (draw.single_drip_lines > 0)
    {
      const std::string_view dripLineColour{ "purple" };
      if (draw.single_drip_lines != 2)
        {
          const DripLine snDrip(
              nuc[0].NUBASE_ME / 1.0e3, nuc[1].NUBASE_ME / 1.0e3, draw.limits, LineType::singleneutron, dripLineColour);

          if (snDrip.areMaxNandZmaxValuesHighEnough())
            {
              snDrip.EPSWriteLine(outFile);
            }
          else
            {
              fmt::print(
                  "**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                  snDrip.limits.Nmax,
                  snDrip.limits.Zmax);
            }
        }

      if (draw.single_drip_lines != 3)
        {
          const DripLine spDrip(
              nuc[0].NUBASE_ME / 1.0e3, nuc[1].NUBASE_ME / 1.0e3, draw.limits, LineType::singleproton, dripLineColour);

          if (spDrip.areMaxNandZmaxValuesHighEnough())
            {
              spDrip.EPSWriteLine(outFile);
            }
          else
            {
              fmt::print(
                  "**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                  spDrip.limits.Nmax,
                  spDrip.limits.Zmax);
            }
        }
    }
  else
    {
      fmt::print("Drawing neither of the single particle drip lines");
    }


  if (draw.double_drip_lines > 0)
    {
      const std::string_view dripLineColour = { "darkgreen" };
      if (draw.double_drip_lines != 2)
        {
          const DripLine dnDrip(
              nuc[0].NUBASE_ME / 1.0e3, nuc[1].NUBASE_ME / 1.0e3, draw.limits, LineType::doubleneutron, dripLineColour);

          if (dnDrip.areMaxNandZmaxValuesHighEnough())
            {
              dnDrip.EPSWriteLine(outFile);
            }
          else
            {
              fmt::print(
                  "**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                  dnDrip.limits.Nmax,
                  dnDrip.limits.Zmax);
            }
        }

      if (draw.double_drip_lines != 3)
        {
          const DripLine dpDrip(
              nuc[0].NUBASE_ME / 1.0e3, nuc[1].NUBASE_ME / 1.0e3, draw.limits, LineType::doubleproton, dripLineColour);

          if (dpDrip.areMaxNandZmaxValuesHighEnough())
            {
              dpDrip.EPSWriteLine(outFile);
            }
          else
            {
              fmt::print(
                  "**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                  dpDrip.limits.Nmax,
                  dpDrip.limits.Zmax);
            }
        }
    }
  else
    {
      fmt::print("Drawing neither of the double particle drip lines");
    }

  // r-process - outline
  if (draw.r_process && draw.limits.Zmax > rProcess::min_Z)
    {
      rProc.EPSWritePath(outFile, false);
    }
  else
    {
      fmt::print("Not drawing the r-process path");
    }

  if (key_relative)
    {
      fmt::print(outFile,
                 "\n%Put coordinates back now that chart is drawn\n"
                 "gr\n");
    }

  // Key
  if (draw.key)
    {
      fmt::print("Drawing the key ");

      fmt::print(outFile, "{}{}", theKey.EPSSetup(), theKey.EPSPlaceKey(draw));

      if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR || draw.chart_colour == ChartColour::GS_HALFLIFE)
        {
          fmt::print(outFile, "{}", theKey.EPSAdditionalFunctions(draw.chart_colour));
        }

      theKey.EPSSetText(draw, part);
      theKey.EPSWrite(outFile, part);

      fmt::print(outFile, "{}", theKey.EPSSurroundingBox());

      fmt::print("- done");
    }
  else
    {
      fmt::print("Not drawing the key");
    }

  // Reset the state and mark end of file
  // As we didn't know the full size during prolog, set it now
  fmt::print(outFile,
             "end grestore\n"
             "\n%%Trailer\n"
             "%%BoundingBox: 0 0 {} {}\n"
             "%%EOF\n",
             std::ceil(width * size),
             std::ceil(height * size));

  outFile.close();
}


void Chart::writeSVG(const std::vector<Nuclide>& nuc, const Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
      return;
    }

  const Prolog setup(size);
  fmt::print(outFile, "{}", setup.SVGWriteProlog(draw));

  fmt::print(outFile, "<g transform=\"translate({0},{0}) scale({1},{1})\">\n", 0.5 * size, size);

  drawNuclei(nuc, draw, outFile);

  fmt::print(outFile,
             "</g>\n"
             "</svg>\n");

  outFile.close();
}


void Chart::writeTIKZ(const std::vector<Nuclide>& nuc, const Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
      return;
    }

  const Prolog setup(size);
  fmt::print(outFile, "{}", setup.TIKZWriteProlog());

  fmt::print(outFile,
             "\\begin{{document}}\n"
             "\\begin{{tikzpicture}}\n");

  drawNuclei(nuc, draw, outFile);

  fmt::print(outFile,
             "\\end{{tikzpicture}}\n"
             "\\end{{document}}\n");

  outFile.close();
}
