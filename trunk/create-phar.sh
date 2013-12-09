#! /bin/sh

ulimit -Sn 4096
box=$(which box.phar)
php -dphar.readonly=0 ${box} build || exit 1
mv mcc.phar mcc
echo "mcc has been created."

