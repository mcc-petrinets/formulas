#! /bin/sh

export PATH=$PATH:.

# Download composer if it does not exist:
composer=$(which composer.phar)
if [ -z ${composer} ]
then
  curl -sS https://getcomposer.org/installer | php
fi

# Download box if it does not exist:
box=$(which box.phar)
if [ -z ${box} ]
then
  curl -s http://box-project.org/installer.php | php
fi

# Increase maximum opened files, because box requires it:
ulimit -Sn 4096

# Build phar:
rm -f mcc
composer.phar install                         || exit 1
php -dphar.readonly=0 $(which box.phar) build || exit 1
mv mcc.phar mcc                               || exit 1
echo "mcc has been created."
