#ifndef DELETECOMMAND_RESPONSE_INCLUDE
#define DELETECOMMAND_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class DeleteCommand_response : public ALRPCResponse
{
public:

  DeleteCommand_response(const DeleteCommand_response& c);
  DeleteCommand_response(void);
  
  virtual ~DeleteCommand_response(void);

  DeleteCommand_response& operator =(const DeleteCommand_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);

private:

  friend class DeleteCommand_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
};

#endif
