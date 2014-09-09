#include "SimYyTransformerParser.h"

#include "SimNetwork.h"
#include "Simulation.h"

#include <SgtCore/YyTransformer.h>
#include <SgtCore/YyTransformerParser.h>

#include <memory>

namespace SmartGridToolbox
{
   void SimYyTransformerParser::parse(const YAML::Node& nd, Simulation& sim) const
   {
      SGT_DEBUG(debug() << "YyTransformer : parse." << std::endl);

      YyTransformerParser transParser;
      auto trans = std::make_shared<SimBranch<YyTransformer>>(transParser.parseYyTransformer(nd));
      
      assertFieldPresent(nd, "network_id");
      assertFieldPresent(nd, "bus_0_id");
      assertFieldPresent(nd, "bus_1_id");

      string netwId = nd["network_id"].as<std::string>();
      std::string bus0Id = nd["bus_0_id"].as<std::string>();
      std::string bus1Id = nd["bus_1_id"].as<std::string>();

      auto netw = sim.simComponent<SimNetwork>(netwId);

      netw->addArc(trans, bus0Id, bus1Id);
   }
}
