// Copyright 2015 National ICT Australia Limited (NICTA)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "SinglePhaseDgyTransformerParserPlugin.h"

#include "SinglePhaseDgyTransformer.h"
#include "Network.h"
#include "YamlSupport.h"

namespace Sgt
{
    void SinglePhaseDgyTransformerParserPlugin::parse(const YAML::Node& nd, Network& netw, const ParserBase& parser) const
    {
        auto trans = parseSinglePhaseDgyTransformer(nd, parser);

        assertFieldPresent(nd, "bus_0_id");
        assertFieldPresent(nd, "bus_1_id");

        std::string bus0Id = parser.expand<std::string>(nd["bus_0_id"]);
        std::string bus1Id = parser.expand<std::string>(nd["bus_1_id"]);

        netw.addBranch(std::move(trans), bus0Id, bus1Id);
    }

    std::unique_ptr<SinglePhaseDgyTransformer> SinglePhaseDgyTransformerParserPlugin::parseSinglePhaseDgyTransformer(const YAML::Node& nd,
            const ParserBase& parser) const
    {
        assertFieldPresent(nd, "id");
        assertFieldPresent(nd, "nom_V_ratio_DY");
        assertFieldPresent(nd, "leakage_impedance");
        assertFieldPresent(nd, "phases_0");
        assertFieldPresent(nd, "phase_1");

        const std::string id = parser.expand<std::string>(nd["id"]);
        Complex nomVRatioDY = parser.expand<Complex>(nd["nom_V_ratio_DY"]);
        auto ndOffNomRatioDY = nd["off_nom_ratio_DY"];
        Complex offNomRatioDY = ndOffNomRatioDY ? parser.expand<Complex>(nd["off_nom_ratio_DY"]) : 1.0;
        const Phases phases0 = parser.expand<Phases>(nd["phases_0"]);
        const Phase phase1 = parser.expand<Phase>(nd["phase_1"]);

        Complex ZL = parser.expand<Complex>(nd["leakage_impedance"]);

        std::unique_ptr<SinglePhaseDgyTransformer> trans(new SinglePhaseDgyTransformer(id, nomVRatioDY, offNomRatioDY, ZL, phases0, phase1));

        return trans;
    }
}
