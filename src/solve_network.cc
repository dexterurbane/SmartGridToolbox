#include <iostream>
#include <cstdlib>

#include <SgtCore.h>

int main(int argc, char** argv)
{
   using namespace SmartGridToolbox;

   assert(argc == 2);
   std::string fname = argv[1];
   Network nw("network", 100.0);
   SmartGridToolbox::Parser<Network> p;
   p.parse(fname, nw);
   auto print = [&](){
      for (auto nd : nw.nodes())
      {
         auto bus = nd->bus();
         auto V = nw.V2Pu(bus->V()[0], bus->VBase());
         auto S = nw.S2Pu(nd->SGen()[0]);
         printf("%12s : %9.4f %9.4f %9.4f %9.4f\n", bus->id().c_str(), std::abs(V), std::arg(V)*180.0/pi,
               S.real(), S.imag());
      }
   };
   print();
   nw.solvePowerFlow();
   print();
}