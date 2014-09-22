#ifndef SINGLE_PHASE_TRANSFORMER_PARSER_PLUGIN
#define SINGLE_PHASE_TRANSFORMER_PARSER_PLUGIN

#include <SgtCore/NetworkParser.h>

namespace SmartGridToolbox
{
   class SinglePhaseTransformer;
   class Network;
   
   /// @brief ParserPlugin that parses SinglePhaseTransformer objects.
   class SinglePhaseTransformerParserPlugin : public NetworkParserPlugin
   {
      public:
         virtual const char* key()
         {
            return "single_phase_transformer";
         }

         virtual void parse(const YAML::Node& nd, Network& netw, const ParserState& state) const override;
         
         std::unique_ptr<SinglePhaseTransformer> parseSinglePhaseTransformer(const YAML::Node& nd,
               const ParserState& state) const;
   };
}

#endif // SINGLE_PHASE_TRANSFORMER_PARSER_PLUGIN