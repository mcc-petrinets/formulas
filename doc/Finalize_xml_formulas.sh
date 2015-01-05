#! /bin/bash
for FILE in $(find . -name "*.xml")
do

  echo $i

  mv $FILE $FILE.back

  echo '<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<property-set xmlns="http://mcc.lip6.fr/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://mcc.lip6.fr/ mccproperty.xsd">' > $FILE

  grep -v "<property-set" $FILE.back | grep -v "</property-set" | grep -v "<?xml" >> $FILE

  echo '</property-set>' >> $FILE

  rm $FILE.back

done
