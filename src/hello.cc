#include "Model.h"
#include "Component.h"
#include "Output.h"
#include "Random.h"
#include <string>
#include <iostream>

using namespace SmartGridToolbox;
using namespace std;

class TestCompA : public Component
{
   public:
      TestCompA(string name) : Component(name) {}
      virtual void AdvanceToTime(Timestamp t)
      {
         std::cout << "TCA AdvanceToTimestep " << t / gSecond << std::endl;
      }
   private:
};

class TestCompB : public Component
{
   public:
      TestCompB(string name) : Component(name) {}
      virtual void AdvanceToTime(Timestamp t)
      {
         tcA_->AdvanceToTime(t);
         std::cout << "TCB AdvanceToTimestep " << t / gSecond << std::endl;
      }
      void SetTestCompA(TestCompA & tcA)
      {
         tcA_ = &tcA;
      }
   private:
      TestCompA * tcA_;
};

int main()
{
   Model mod;
   mod.AddComponent(*(new TestCompA("tca1")));
   mod.AddComponent(*(new TestCompB("tcb1")));
   string nm1 = "tca1";
   TestCompA & tca1 = *mod.GetComponentNamed<TestCompA>(nm1);
   TestCompB & tcb1 = *mod.GetComponentNamed<TestCompB>("tcb1");
   tcb1.SetTestCompA(tca1);
   tcb1.AdvanceToTime(3 * gSecond);

   for (int i = 0; i < 10; ++i)
   {
      double d = RandNormal(10.0, 3.0);
      std::cout << "Random = " << d << std::endl;
   }
}
