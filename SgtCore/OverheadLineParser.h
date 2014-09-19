#ifndef OVERHEAD_LINE_PARSER
#define OVERHEAD_LINE_PARSER

#include <SgtCore/NetworkParser.h>

namespace SmartGridToolbox
{
   class Network;
   class OverheadLine;
   
   /// @brief ParserPlugin that parses OverheadLine objects.
   class OverheadLineParser : public NetworkParserPlugin
   {
      public:
         virtual const char* key()
         {
            return "overhead_line";
         }

         virtual void parse(const YAML::Node& nd, Network& netw, const ParserState& state) const override;

         std::unique_ptr<OverheadLine> parseOverheadLine(const YAML::Node& nd, const ParserState& state) const;
   };
}

#endif // OVERHEAD_LINE_PARSER
