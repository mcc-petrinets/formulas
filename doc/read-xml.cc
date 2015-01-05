#include <iostream>
#include <fstream>
#include <memory>
#include "mccproperty.hxx"

using std::cout;
using std::endl;
using std::auto_ptr;

typedef auto_ptr<mcc::property_set>
        property_set;

int
main (int argc, char* argv[])
{
  try
  {
    property_set ps = mcc::property_set_("properties.xml");
    cout << *ps << endl;
  }
  catch (const xml_schema::exception& e)
  {
    cout << e << endl;
    return 1;
  }
}
