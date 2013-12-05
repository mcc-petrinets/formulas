#! /bin/sh

#php -dphar.readonly=0 create-phar.php || exit 1
ulimit -Sn 4096
php -dphar.readonly=0 box.phar build || exit 1
mv mcc.phar mcc
echo "mcc has been created."

