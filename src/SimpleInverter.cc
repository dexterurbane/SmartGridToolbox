#include "SimpleInverter.h"

#include "Model.h"
#include "Bus.h"

namespace SmartGridToolbox
{
   void SimpleInverterParser::parse(const YAML::Node & nd, Model & mod, const ParserState & state) const
   {
      SGT_DEBUG(debug() << "SimpleInverter : parse." << std::endl);

      assertFieldPresent(nd, "name");
      assertFieldPresent(nd, "bus");
      assertFieldPresent(nd, "phases");

      string name = state.expandName(nd["name"].as<std::string>());
      Phases phases = nd["phases"].as<Phases>();

      SimpleInverter & comp = mod.newComponent<SimpleInverter>(name, phases);

      if (nd["efficiency"])
      {
         comp.setEfficiency(nd["efficiency"].as<double>());
      }
      else
      {
         comp.setEfficiency(1.0);
      }

      if (nd["max_S_mag_per_phase"])
      {
         comp.setMaxSMagPerPhase(nd["max_S_mag_per_phase"].as<double>());
      }
      else
      {
         comp.setMaxSMagPerPhase(10000.0);
      }

      if (nd["min_power_factor"])
      {
         comp.setMinPowerFactor(nd["min_power_factor"].as<double>());
      }
      else
      {
         comp.setMinPowerFactor(0.0);
      }

      if (nd["requested_Q_per_phase"])
      {
         comp.setRequestedQPerPhase(nd["requested_Q_per_phase"].as<double>());
      }
      else
      {
         comp.setRequestedQPerPhase(0.0);
      }
   }

   void SimpleInverterParser::postParse(const YAML::Node & nd, Model & mod, const ParserState & state) const
   {
      SGT_DEBUG(debug() << "SimpleInverter : postParse." << std::endl);

      string name = state.expandName(nd["name"].as<std::string>());
      SimpleInverter & comp = *mod.componentNamed<SimpleInverter>(name);

      const std::string busStr = state.expandName(nd["bus"].as<std::string>());
      Bus * busComp = mod.componentNamed<Bus>(busStr);
      if (busComp != nullptr)
      {
         busComp->addZipToGround(comp);
      }
      else
      {
         error() << "For component " << name << ", bus " << busStr 
                 << " was not found in the model." << std::endl;
         abort();
      }
   }

   UblasVector<Complex> SimpleInverter::S() const
   {
      double PPerPh = PPerPhase();
      double P2PerPh = PPerPh * PPerPh; // Limited by maxSMagPerPhase_.
      double Q2PerPh = requestedQPerPhase_ * requestedQPerPhase_;
      double maxSMag2PerPh =  maxSMagPerPhase_ * maxSMagPerPhase_;
      double SMag2PerPh = std::min(P2PerPh + Q2PerPh, maxSMag2PerPh);
      double QPerPh = sqrt(SMag2PerPh - P2PerPh);
      if (requestedQPerPhase() < 0.0)
      {
         QPerPh *= -1;
      }
      Complex SPerPh{PPerPhase(), QPerPh};
      return UblasVector<Complex>(phases().size(), SPerPh);
   }

   double SimpleInverter::PPerPhase() const
   {
      double P = InverterBase::PPerPhase();
      return std::min(std::abs(P), maxSMagPerPhase_) * (P < 0 ? -1 : 1);
   }

   SimpleInverter::SimpleInverter(const std::string & name, const Phases & phases) :
      InverterBase(name, phases),
      efficiency_(1.0), 
      maxSMagPerPhase_(5000.0), 
      minPowerFactor_(0.9), 
      requestedQPerPhase_(0.0), 
      inService_(true),
      S_(phases.size(), czero)
   {
      // Empty.
   }
}