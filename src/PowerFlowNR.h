#ifndef POWER_FLOW_NR_DOT_H
#define POWER_FLOW_NR_DOT_H

#include "Common.h"
#include "PowerFlow.h"
#include <vector>
#include <map>

// Terminology:
// "Bus" and "Branch" refer to n-phase objects i.e. they can contain several phases.
// "Node" and "Link" refer to individual bus conductors and single phase lines.
// A three phase network involving busses and branches can always be decomposed into a single phase network
// involving nodes and links. Thus use of busses and branches is simply a convenience that lumps together nodes and
// links.

namespace SmartGridToolbox
{
   class BusNR
   {
      public:
         BusNR(const std::string & id, BusType type, const std::vector<Phase> & phases, const Vector<Complex> & V,
               const Vector<Complex> & Y, const Vector<Complex> & I, const Vector<Complex> & S);

         const std::string & getId() const {return id_;}
         BusType getType() const {return type_;}
         const std::vector<NodeNR> & getNodes() const {return nodes_;}
         std::vector<NodeNR> & getNodes() {return nodes_;}

      private:
         std::string id_;           ///< Arbitrary bus ID, for external use.
         BusType type_;             ///< Bus type.
         std::vector<NodeNR> nodes_ ///< Bus nodes.
   };

   class NodeNR
   {
      public:
         NodeNR(Bus & bus, Phase phase, const Complex & V, const Complex & Y, const Complex & I, const Complex & S);

         const Bus & getBus() const {return *bus;}
         Bus & getBus() {return *bus;}

         Phase getPhase() const {return phase_;}

         const Complex & getV() const {return V_;}
         const Complex & getY() const {return V_;}
         const Complex & getI() const {return V_;}
         const Complex & getS() const {return V_;}

         int getIdx() {return idx_;}
         void setIdx(int idx) {idx_ = idx;}

      private:
         Bus * bus_;

         Phase phase_;

         Complex V_; ///< Voltage.
         Complex Y_; ///< Constant admittance.
         Complex I_; ///< Constant current.
         Complex S_; ///< Constant power.

         int idx_;   ///< Node index.
   };

   class BranchNR
   {
      public:
         BranchNR(const std::string & id, const std::string & id0, const std::string & id1, 
               const std::vector<Phase> & phasesBus0, const std::vector<Phase> & phasesBus1,
               const Matrix<Complex> & Y);

      private:
         std::string id_;                    ///< id of branch.
         std::array<std::string, 2> busIds_; ///< id of bus 0/1.
         std::vector<Link> links_;           ///< Branch links.
   };

   class LinkNR
   {
      public:
         LinkNR(const Branch & branch, int busIdx0, int busIdx0, int phaseIdx1, int phaseIdx1, 
               const Array2D<Complex, 2, 2> & Y);

      private:
         const BranchNR * branch_;              ///< My branch.
         std::array<int, 2> busIdxs_;           ///< bus indices of terminals in parent branch.
         std::array<int, 2> phaseIdxs_;         ///< Phases indices on terminals in parent branch.
         Array2D<Complex, 2, 2> Y_;             ///< Complex value of elements in bus admittance matrix in NR solver.
         std::array<const NodeNR *, 2> nodes_;  ///< My nodes.
   };

   class PowerFlowNR
   {
      public:
         typedef std::map<std::string, BusNR *> BusMap;
         typedef std::vector<BranchNR *> BranchVec;
         typedef std::vector<NodeNR *> NodeVec;
         typedef std::vector<LinkNR *> LinkVec;

      public:
         void addBus(const std::string & id, BusType type, const Vector<Phase> & phases,
               const Vector<Complex> & V, const Vector<Complex> & Y, Vector<Complex> & I, Vector<Complex> & S);

         const BusMap & getBusses()
         {
            return bussesById_;
         }

         void addBranch(const std::string & id, const std::string & idBus0, const std::string & idBus1,
               const Vector<Phase> & phasesBus0, const Vector<Phase> & phasesBus1, const Matrix<Complex> & Y);

         const BranchVec & getBranches()
         {
            return branches_;
         }

         void reset();
         void validate();
         bool solve();
         void outputNetwork();

      private:
         void buildBusAdmit();
         void initx();
         void updateBusV();
         void updateF();
         void updateJ();
         void outputCurrentState();

      private:
         /// @name Vectors of busses and branches.
         /// @{
         BusMap bussesById_;
         NodeVec SLNodes;
         NodeVec PQNodes;

         BranchVec branches_;
         /// @}

         /// @name Array bounds.
         /// @{
         int nSL_;                     ///< Number of slack nodes.
         int nPQ_;                     ///< Number of PQ nodes.
         int nNode;                    ///< Total number of nodes.
         int nVar_;                    ///< Total number of variables.
         /// @}

         /// @name ublas ranges into vectors/matrices.
         /// @{
         UblasRange rPQ_;              ///< Range of PQ nodes in list of all nodes.
         UblasRange rAll_;             ///< Range of all nodes in list of all nodes.
                                       /**< Needed for matrix_range. */
         int iSL_;                     ///< Index of slack node in list of all nodes.
         UblasRange rx0_;              ///< Range of real voltage components in x_. 
         UblasRange rx1_;              ///< Range of imag voltage components in x_.
         /// @}

         Complex V0_;                  ///< Slack voltages.

         UblasVector<double> PPQ_;     ///< Constant power injection of PQ nodes.
         UblasVector<double> QPQ_;

         UblasVector<double> IrPQ_;    ///< Constant current injection of PQ nodes.
         UblasVector<double> IiPQ_;

         UblasVector<double> Vr_;
         UblasVector<double> Vi_;
         UblasCMatrix<Complex> Y_;
         UblasCMatrix<double> G_;
         UblasCMatrix<double> B_;

         UblasVector<double> x_;
         UblasVector<double> f_;
         UblasCMatrix<double> J_;
         UblasCMatrix<double> JConst_; ///< The part of J that doesn't update at each iteration.
   };
}

#endif // POWER_FLOW_NR_DOT_H
