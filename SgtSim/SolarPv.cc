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

#include "SolarPv.h"

#include "Inverter.h"
#include "Weather.h"

namespace Sgt
{
    SolarPv::SolarPv(const std::string& id) :
        Component(id),
        weather_(nullptr),
        efficiency_(1.0),
        planeNormal_( {0.0, 0.0}),
                  planeArea_(0.0)
    {}

    void SolarPv::setWeather(std::shared_ptr<Weather> weather)
    {
        weather_ = weather;
        dependsOn(weather);
        weather->didUpdate().addAction([this]() {dcPowerChanged().trigger();},
        std::string("Trigger ") + sComponentType() + " " + id() + " DC power changed");
    }

    double SolarPv::PDc() const
    {
        // Note: convert from SI to MW.
        return 1e-6*weather_->solarPower(planeNormal_, planeArea_) * efficiency_;
    }
};
