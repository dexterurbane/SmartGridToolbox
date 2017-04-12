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

#include "VvTransformerParserPlugin.h"

#include "VvTransformer.h"
#include "Network.h"
#include "YamlSupport.h"

namespace Sgt
{
    void VvTransformerParserPlugin::parse(const YAML::Node& nd, Network& netw, const ParserBase& parser) const
    {
        auto trans = parseVvTransformer(nd, parser);

        assertFieldPresent(nd, "bus_0_id");
        assertFieldPresent(nd, "bus_1_id");

        std::string bus0Id = parser.expand<std::string>(nd["bus_0_id"]);
        std::string bus1Id = parser.expand<std::string>(nd["bus_1_id"]);

        netw.addBranch(std::move(trans), bus0Id, bus1Id);
    }

    std::unique_ptr<VvTransformer> VvTransformerParserPlugin::parseVvTransformer(const YAML::Node& nd,
            const ParserBase& parser) const
    {
        assertFieldPresent(nd, "id");
        assertFieldPresent(nd, "nom_ratio");
        assertFieldPresent(nd, "leakage_impedance");

        const std::string id = parser.expand<std::string>(nd["id"]);
        Complex nomRatio = parser.expand<Complex>(nd["nom_ratio"]);

        auto ndOffNomRatio10 = nd["off_nom_ratio_10"];
        Complex offNomRatio10 = ndOffNomRatio10 ? parser.expand<Complex>(nd["off_nom_ratio_10"]) : 1.0;
        
        auto ndOffNomRatio12 = nd["off_nom_ratio_12"];
        Complex offNomRatio12 = ndOffNomRatio12 ? parser.expand<Complex>(nd["off_nom_ratio_12"]) : 1.0;

        Complex ZL10 = parser.expand<Complex>(nd["leakage_impedance_10"]);
        Complex ZL12 = parser.expand<Complex>(nd["leakage_impedance_12"]);

        auto ndTieAdmittance = nd["tie_admittance"];
        Complex YTie = ndTieAdmittance ? parser.expand<Complex>(ndTieAdmittance) : Complex{0.0, 0.0};
        
        auto ndGndAdmittance = nd["ground_admittance"];
        Complex YGnd = ndGndAdmittance ? parser.expand<Complex>(ndGndAdmittance) : Complex{0.0, 0.0};
        
        std::unique_ptr<VvTransformer> trans(
                new VvTransformer(id, nomRatio, offNomRatio10, offNomRatio12, ZL10, ZL12, YTie, YGnd));

        return trans;
    }
}
