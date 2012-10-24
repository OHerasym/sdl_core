#ifndef ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE
#define ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct EncodedSyncPData_requestMarshaller
{
  static bool checkIntegrity(EncodedSyncPData_request& e);
  static bool checkIntegrityConst(const EncodedSyncPData_request& e);

  static bool fromString(const std::string& s,EncodedSyncPData_request& e);
  static const std::string toString(const EncodedSyncPData_request& e);

  static bool fromJSON(const Json::Value& s,EncodedSyncPData_request& e);
  static Json::Value toJSON(const EncodedSyncPData_request& e);
};
#endif
