#include "YyTransformerParser.h"

#include "YyTransformer.h"
#include "Network.h"

namespace SmartGridToolbox
{
   void YyTransformerParser::parse(const YAML::Node& nd, Network& netw, const ParserState& state) const
   {
      auto trans = parseYyTransformer(nd, state);

      assertFieldPresent(nd, "bus_0_id");
      assertFieldPresent(nd, "bus_1_id");

      std::string bus0Id = state.expandName(nd["bus_0_id"].as<std::string>());
      std::string bus1Id = state.expandName(nd["bus_1_id"].as<std::string>());

      netw.addArc(std::move(trans), bus0Id, bus1Id);
   }

   std::unique_ptr<YyTransformer> YyTransformerParser::parseYyTransformer(const YAML::Node& nd,
         const ParserState& state) const
   {
      assertFieldPresent(nd, "id");
      assertFieldPresent(nd, "complex_turns_ratio_01");
      assertFieldPresent(nd, "leakage_impedance");

      const std::string id = state.expandName(nd["id"].as<std::string>());
      Complex a = nd["complex_turns_ratio_01"].as<Complex>();
      Complex ZL = nd["leakage_impedance"].as<Complex>();
      auto ndYm = nd["magnetizing_admittance"];
      Complex YM = ndYm ? ndYm.as<Complex>() : czero;

      std::unique_ptr<YyTransformer> trans(new YyTransformer(id, a, ZL, YM));

      return trans;
   }
}
