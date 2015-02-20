#! /bin/bash

FILE=mccproperty
PYTHON=/opt/local/bin/python2.7
GENERATEDS=/usr/local/bin/generateDS.py
#CC=clang++-mp-3.1
#CC=g++-mp-4.5
CC="g++"

echo "Generating C++ classes..."
xsdcxx cxx-tree --generate-serialization --generate-doxygen --generate-ostream \
      --generate-comparison --generate-detach --generate-default-ctor \
      --generate-polymorphic --polymorphic-type-all \
      --namespace-map http://mcc.lip6.fr/=mcc \
      --root-element property-set \
      $FILE.xsd
# --root-element mcc.property.set

echo "Compiling example..."
$CC -O3 -I/opt/local/include/ -L/opt/local/lib/ -Ilibxsd -lxerces-c mccproperty.cxx txt-to-xml.cc -o txt-to-xml
$CC -O3 -I/opt/local/include/ -L/opt/local/lib/ -Ilibxsd -lxerces-c mccproperty.cxx read-xml.cc -o read-xml

# The two bindings below fail:
#$PYTHON $GENERATEDS -o $FILE.py $FILE.xsd
#xjc -p $FILE $FILE.xsd
