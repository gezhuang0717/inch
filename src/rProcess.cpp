#include "inch/rProcess.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>


bool rProcess::readData() const
{
  data.reserve(150);

  fmt::print("Reading {} for the r-process nuclei", file);

  std::ifstream rp(file, std::ios::binary);

  if (!rp)
    {
      fmt::print(stderr, "***ERROR***: {} couldn't be opened to read the r-process path\n", file);
      return false;
    }

  std::string line;

  while (std::getline(rp, line))
    {
      if (line.empty() || line.front() == '#')
        {
          continue;
        }

      int n{ 0 };
      int z{ 0 };

      std::istringstream rData(line);
      rData >> n >> z;

      data.emplace_back(n, z);
    }

  rp.close();

  fmt::print(" - done\n");

  return true;
}


std::string rProcess::EPSPathSetup(const bool shaded) const
{
  return shaded ? fmt::format("\n%r-process -- shaded path\n"
                              "gs\n"
                              "0.9 setgray\n")
                : fmt::format("\n%r-process -- path outline\n"
                              "black rgb\n"
                              "0.1 u div sl\n");
}


void rProcess::EPSWritePath(std::ofstream& outFile, const bool shaded) const
{
  fmt::print(outFile, "{}", EPSPathSetup(shaded));

  bool initial{ true };

  for (const auto& it : data)
    {
      // it.first = N
      // it.second = Z
      if (it.second >= limits.Zmin && it.second <= limits.Zmax && it.first >= limits.Nmin && it.first <= limits.Nmax)
        {
          fmt::print(outFile,
                     "{:>3d} {:>3d} {}\n",
                     (it.first - limits.Nmin),
                     (it.second - limits.Zmin),
                     (initial ? 'm' : 'l'));

          initial = false;
        }
    }

  fmt::print(outFile, "{}", EPSPathTearDown(shaded));
}


std::string rProcess::EPSPathTearDown(const bool shaded) const
{
  return shaded ? fmt::format("fill\n"
                              "gr\n")
                : fmt::format("st\n");
}
