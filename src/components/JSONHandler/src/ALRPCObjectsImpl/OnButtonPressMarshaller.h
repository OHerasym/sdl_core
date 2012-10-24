#ifndef ONBUTTONPRESSMARSHALLER_INCLUDE
#define ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnButtonPressMarshaller
{
  static bool checkIntegrity(OnButtonPress& e);
  static bool checkIntegrityConst(const OnButtonPress& e);

  static bool fromString(const std::string& s,OnButtonPress& e);
  static const std::string toString(const OnButtonPress& e);

  static bool fromJSON(const Json::Value& s,OnButtonPress& e);
  static Json::Value toJSON(const OnButtonPress& e);
};
#endif
