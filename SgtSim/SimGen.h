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

#ifndef SIM_GEN_DOT_H
#define SIM_GEN_DOT_H

#include <SgtSim/SimComponent.h>
#include <SgtSim/SimNetwork.h>

#include <SgtCore/Gen.h>

#include <memory>

namespace Sgt
{
    /// @brief Abstract base class for SimGen.
    /// 
    /// Depending on how the derived class works, gen() could either be provided by containment or inheritance.
    class SimGenAbc : virtual public SimComponent
    {
        public:

            virtual std::shared_ptr<const GenAbc> gen() const = 0;
            virtual std::shared_ptr<GenAbc> gen() = 0;

            virtual void joinNetwork(SimNetwork& simNetwork, const std::string& busId);
    };

    /// @brief Simulation gen, corresponding to a GenAbc in a SimNetwork's network(). 
    /// 
    /// gen() is provided by containment which is enough for a normal network gen types. 
    class SimGen : public SimGenAbc
    {
        public:

            SimGen(std::shared_ptr<GenAbc> gen) :
                Component(gen->id()),
                gen_(gen)
            {
                // Empty.
            }

            std::shared_ptr<const GenAbc> gen() const
            {
                return gen_;
            }
            
            std::shared_ptr<GenAbc> gen()
            {
                return gen_;
            }

        private:

            std::shared_ptr<GenAbc> gen_;
    };
}

#endif // SIM_GEN_DOT_H
