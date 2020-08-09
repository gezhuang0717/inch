#include "inch/chart.hpp"

#include <catch2/catch.hpp>

// Create a concrete class so we can test the non-virtual functions
// This seems to be better than picking a 'random' derived class
class ChartForTest : public Chart
{
public:
  ChartForTest(Options& _options) : Chart(_options){};
  void write(const std::vector<Nuclide>& massTable, const Partition& part) const;

  std::string prolog() const;

  std::string setup() const;

  std::string teardown() const;

  std::string KeySetup(const int ZRange) const;

  std::string KeyTearDown() const;
};

Options options;

TEST_CASE("Canvas Height", "[Chart]")
{
  ChartForTest theChart(options);

  SECTION("Z range is large enough to not use a relatively placed chart")
  {
    theChart.key_relative = false;
    const int ZRange{ 50 };
    const double scale{ 1.5 };
    const double height{ 6.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Approx(52.0));
    REQUIRE(theChart.key_relative == false);
  }

  SECTION("Z range means that the chart will be placed relative to the key")
  {
    theChart.key_relative = false;

    const int ZRange{ 1 };
    const double scale{ 2.0 };
    const double height{ 5.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Approx(12.0));
    REQUIRE(theChart.key_relative == true);
  }
}


TEST_CASE("Canvas Width", "[Chart]")
{
  ChartForTest theChart(options);

  SECTION("Full chart does not have additional width")
  {
    theChart.options.limits.ResetAllLimits();
    theChart.options.chart_selection = ChartSelection::FULL_CHART;

    const double scale{ 2.0 };
    REQUIRE(theChart.calculateCanvasWidth(scale) == Approx(179.0));
  }

  SECTION("Partial chart has additional canvas width")
  {
    theChart.options.limits.Nmin     = 20;
    theChart.options.limits.Nmax     = 50;
    theChart.options.limits.Zmin     = 30;
    theChart.options.limits.Zmax     = 70;
    theChart.options.chart_selection = ChartSelection::SUB_CHART;

    const double scale{ 1.5 };
    REQUIRE(theChart.calculateCanvasWidth(scale) == Approx(53.75));
  }
}


TEST_CASE("Canvas size", "[Chart]")
{
  ChartForTest theChart(options);

  theChart.options.limits.Nmin     = 20;
  theChart.options.limits.Nmax     = 50;
  theChart.options.limits.Zmin     = 30;
  theChart.options.limits.Zmax     = 70;
  theChart.options.chart_selection = ChartSelection::SUB_CHART;

  const double scale{ 1.5 };
  const double height{ 5.0 };

  theChart.setCanvasSize(scale, height);

  REQUIRE(theChart.width == Approx(53.75));
  REQUIRE(theChart.height == Approx(42.0));
}
