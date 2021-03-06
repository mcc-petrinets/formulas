#include <iostream>
#include <sstream>

#include "parser.hxx"
#include "mccproperty.hxx"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

std::ifstream* input_file;
std::ofstream* output_file;

void
xml_output(const property_set& s)
{
  xml_schema::namespace_infomap map;
  map[""].name   = "http://mcc.lip6.fr/";
  map[""].schema = "mccproperty.xsd";
  mcc::property_set_(*output_file, *s, map);
}

int
main (int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Syntax error: " << argv[0] << " input-file output-file" << std::endl;
    return 1;
  }
  input_file = new std::ifstream(argv[1]);
  output_file = new std::ofstream(argv[2]);
  try
  {
    unsigned int line_number = 0;
    string line;
    while (getline(*input_file, line))
    {
      ++line_number;
      if (line == "")
      {
        cout << "-";
        continue;
      }
      string::const_iterator begin = line.begin();
      string::const_iterator end   = line.end();
      bool ok = qi::phrase_parse(begin, end,
                                 mccgrammar<string::const_iterator>()[&xml_output], standard_wide::space);
      if (ok) {
        cout << ".";
      } else {
        cout << "x";
        cerr << "Could not parse line " << line_number << "." << endl;
      }
      cout.flush();
    }
  }
  catch (const xml_schema::exception& e)
  {
    cout << e << endl;
    return 1;
  }
  cout << endl;
}


