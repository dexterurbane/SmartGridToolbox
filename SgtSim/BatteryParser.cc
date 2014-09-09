#include "Battery.h"
#include "BatteryParser.h"
#include "Inverter.h"
#include "Simulation.h"

namespace SmartGridToolbox
{
   void BatteryParser::parse(const YAML::Node& nd, Simulation& into) const
   {
      SGT_DEBUG(debug() << "Battery : parse." << std::endl);

      assertFieldPresent(nd, "id");
      assertFieldPresent(nd, "inverter");

      string id = nd["id"].as<std::string>();
      auto batt = into.newSimComponent<Battery>(id);

      auto nd_dt = nd["dt"];
      if (nd_dt) batt->set_dt(nd_dt.as<Time>());

      auto ndInitCharge = nd["init_charge"];
      if (ndInitCharge) batt->setInitCharge(ndInitCharge.as<double>());

      auto ndMaxCharge = nd["max_charge"];
      if (ndMaxCharge) batt->setMaxCharge(ndMaxCharge.as<double>());

      auto ndMaxChargePower = nd["max_charge_power"];
      if (ndMaxChargePower) batt->setMaxChargePower(ndMaxChargePower.as<double>());

      auto ndMaxDischargePower = nd["max_discharge_power"];
      if (ndMaxDischargePower) batt->setMaxDischargePower(ndMaxDischargePower.as<double>());

      auto ndChargeEfficiency = nd["charge_efficiency"];
      if (ndChargeEfficiency) batt->setChargeEfficiency(ndChargeEfficiency.as<double>());

      auto ndDischargeEfficiency = nd["discharge_efficiency"];
      if (ndDischargeEfficiency) batt->setDischargeEfficiency(ndDischargeEfficiency.as<double>());

      auto ndRequestedPower = nd["requested_power"];
      if (ndRequestedPower) batt->setRequestedPower(ndRequestedPower.as<double>());

      const std::string inverterStr = nd["inverter"].as<std::string>();
      auto inverter = into.simComponent<InverterAbc>(inverterStr);
      if (inverter != nullptr)
      {
         inverter->addDcPowerSource(batt);
      }
      else
      {
         error() << "For component " << id << ", inverter " << inverterStr
                 << " was not found in the model." << std::endl;
         abort();
      }
   }
}