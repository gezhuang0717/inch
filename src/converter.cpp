#include "converter.hpp"

std::string Converter::FloatToNdp(const double number, const int numDP) const
{
  std::string value = std::to_string(number);

  const int digits = (numDP < 1) ? -1 : numDP+1;

  return value.substr(0, value.find('.') + digits);
}


std::tuple<std::string, std::string, std::string> Converter::FloatToExponent(const double in) const
{
  /// Force the number to follow the regex: -?\d*\.?\d+e[+-]?\d+
  std::ostringstream num;
  num << std::scientific << in;

  const std::regex pieces_regex(R"((-?\d*\.?\d+)e([+-]?)(\d+))");
  std::smatch matches;

  /// Need to convert before passing to regex_match
  const std::string number = num.str();

  /// Always get 1dp from the first number
  /// If it's negative take an extra char for the sign
  const int digits = ( in < 0.0 ) ? 4 : 3;

  return ( std::regex_match(number, matches, pieces_regex) )
    //                coefficient(1dp)                          exponent sign           exponent
    ? std::make_tuple(std::string(matches[1]).substr(0, digits),std::string(matches[2]),std::string(matches[3]))
    : std::make_tuple(std::string(), std::string(), std::string());
}


std::string Converter::IsomerEnergyToHuman(const double in, const int numDP) const
{
  return ( in < 1.0e3 )
    ? FloatToNdp(in, numDP) + " keV"
    : FloatToNdp(in/1.0e3, numDP) + " MeV";
}


std::string Converter::SecondsToHuman(const double number, const int numDP) const
{
  std::string value;

  /// Don't forget the space at the front of the units string
  /// Ranges in conditions come from wanting eg 1ms rather than 1000us
  /// but still allowing eg 0.5ms
  if ( number < 1.0e-10 )
    {
      value = FloatToNdp(number*1.0e12, numDP) + " ps";
    }
  else if ( number < 1.0e-7 && number >= 1.0e-10 )
    {
      value = FloatToNdp(number*1.0e9, numDP) + " ns";
    }
  else if ( number < 1.0e-4 && number >= 1.0e-7 )
    {
      value = FloatToNdp(number*1.0e6, numDP) + " 1 S (u) tw sh";
    }
  else if ( number < 0.1 && number >= 1.0e-4 )
    {
      value =  FloatToNdp(number*1.0e3, numDP) + " ms";
    }
  else if ( number < static_cast<double>(TimeInSeconds::minutes) && number >= 0.1 )
    {
      value = FloatToNdp(number, numDP) + " s";
    }
  else if (    number < static_cast<double>(TimeInSeconds::hours)
           && number >= static_cast<double>(TimeInSeconds::minutes) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::minutes), numDP) + " mins";
    }
  else if (    number < static_cast<double>(TimeInSeconds::days)
           && number >= static_cast<double>(TimeInSeconds::hours) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::hours), numDP) + " hrs";
    }
  else if (    number < static_cast<double>(TimeInSeconds::years)
           && number >= static_cast<double>(TimeInSeconds::days) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::days), numDP) + " days";
    }
  else
    {
      const double years = number/static_cast<double>(TimeInSeconds::years);

      if ( years >= 1.0e9 )
        {
          value = FloatToNdp(years/1.0e9, numDP) + " Gyrs";
        }
      else if ( years >= 1.0e6 )
        {
          value = FloatToNdp(years/1.0e6, numDP) + " Myrs";
        }
      else
        {
          value = FloatToNdp(years, numDP) + " yrs";
        }
    }

  return value;
}
