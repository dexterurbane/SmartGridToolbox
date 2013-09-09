#ifndef WEATHER_DOT_H
#define WEATHER_DOT_H

#include "Parser.h"
#include "RegularUpdateComponent.h"
#include "Sun.h"
#include "TimeSeries.h"

namespace SmartGridToolbox
{
   class WeatherParser : public ComponentParser
   {
      public:
         static constexpr const char * componentName()
         {
            return "weather";
         }

      public:
         virtual void parse(const YAML::Node & nd, Model & mod) const override;
   };

   struct SolarIrradiance
   {
      Array<double, 3> direct;
      double horizontalDiffuse;
   };

   class Weather : public RegularUpdateComponent
   {
      public:
         Weather(const std::string & name) :
            RegularUpdateComponent(name),
            latLong_(Greenwich),
            cloudCoverSeries_(nullptr)
         {
            setDt(minutes(5));
         }

         virtual ~Weather() {delete cloudCoverSeries_;}

         void setLatLong(const LatLong & latLong) {latLong_ = latLong;}

         void acquireCloudCoverSeries(TimeSeries<Time, double> * newSeries)
         {
            std::swap(cloudCoverSeries_, newSeries);
         }

         double solarPower(SphericalAngles planeNormal, double planeArea) const;

         const SolarIrradiance & irradiance()
         {
            return irradiance_;
         }

      private:
         virtual void initializeState(Time t)
         {
            irradiance_ = unaveragedIrradiance(t);
         }

         virtual void updateState(Time t0, Time t1) override;

         SolarIrradiance unaveragedIrradiance(const Time & tm) const;

      private:
         LatLong latLong_;
         TimeSeries<Time, double> * cloudCoverSeries_;

         SolarIrradiance prevIrradiance_;
         SolarIrradiance irradiance_;
   };
}

#endif // WEATHER_DOT_H
