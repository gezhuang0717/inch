/**
 *
 * \class SVGChart
 *
 * \brief
 */

#ifndef SVGCHART_HPP
#define SVGCHART_HPP

#include "inch/chart.hpp"
//#include "inch/svg_dripline.hpp"

#include <vector>

class Nuclide;
class Options;

class SVGChart : public Chart
{
public:
  explicit SVGChart(Options& _options) : Chart(_options){};

  SVGChart(const SVGChart& SVGChart)     = default;
  SVGChart(SVGChart&& SVGChart) noexcept = default;

  SVGChart& operator=(const SVGChart& SVGChart) = delete;
  SVGChart& operator=(SVGChart&& SVGChart) noexcept = delete;

  ~SVGChart() noexcept override = default;

  /// Container for the drip lines that will actually be drawn
  // mutable std::vector<SVGDripLine> drip_lines;

  /**
   */
  void write(const std::vector<Nuclide>& massTable, const Partition& /*part*/) const override;

  std::string prolog() const override;

  std::string setup() const override;

  std::string teardown() const override;

  inline std::string KeySetup(const int /*ZRange*/) const override { return std::string(); }

  inline std::string KeyTearDown() const override { return std::string(); };
};

#endif // SVGCHART_HPP
