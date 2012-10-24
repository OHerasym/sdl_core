#ifndef HMILEVELMARSHALLER_INCLUDE
#define HMILEVELMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/HMILevel.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


//! marshalling class for HMILevel

class HMILevelMarshaller
{
public:

  static std::string toName(const HMILevel& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(HMILevel& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=HMILevel::INVALID_ENUM;
  }

  static bool checkIntegrity(HMILevel& e)		{ return e.mInternal!=HMILevel::INVALID_ENUM; } 
  static bool checkIntegrityConst(const HMILevel& e)	{ return e.mInternal!=HMILevel::INVALID_ENUM; } 

  static bool fromString(const std::string& s,HMILevel& e);
  static const std::string toString(const HMILevel& e);

  static bool fromJSON(const Json::Value& s,HMILevel& e);
  static Json::Value toJSON(const HMILevel& e);

  static const char* getName(HMILevel::HMILevelInternal e)
  {
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const HMILevel::HMILevelInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
