#include <SgtSim/SolarPv.h>
#include <SgtSim/InverterBase.h>
#include <SgtSim/Model.h>
#include <SgtSim/Weather.h>

namespace SmartGridToolbox
{
   SolarPv::SolarPv(const std::string& name) :
      DcPowerSourceBase(name),
      weather_(nullptr),
      efficiency_(1.0),
      planeNormal_({0.0, 0.0}),
      planeArea_(0.0)
   {}

   void SolarPv::setWeather(Weather& weather)
   {
      weather_ = &weather;
      dependsOn(weather);
      weather.didUpdate().addAction([this](){needsUpdate().trigger();}, "Trigger SolarPv " + name() + " needs update");
   }

   double SolarPv::PDc() const
   {
      return weather_->solarPower(planeNormal_, planeArea_)*efficiency_;
   }
};