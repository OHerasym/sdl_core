#ifndef VEHICLETYPE_INCLUDE
#define VEHICLETYPE_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



class VehicleType
{
public:

  VehicleType(const VehicleType& c);
  VehicleType(void);

  bool checkIntegrity(void);

  ~VehicleType(void);
  VehicleType& operator =(const VehicleType&);

// getters

  const std::string* get_make(void) const;
  const std::string* get_model(void) const;
  const std::string* get_modelYear(void) const;
  const std::string* get_trim(void) const;

// setters

  void reset_make(void);
  bool set_make(const std::string& make_);
  void reset_model(void);
  bool set_model(const std::string& model_);
  void reset_modelYear(void);
  bool set_modelYear(const std::string& modelYear_);
  void reset_trim(void);
  bool set_trim(const std::string& trim_);

private:

  friend class VehicleTypeMarshaller;

  std::string* make;	//!< (500)
  std::string* model;	//!< (500)
  std::string* modelYear;	//!< (500)
  std::string* trim;	//!< (500)
};

#endif
