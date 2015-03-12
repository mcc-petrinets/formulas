#! /bin/sh

# Increase maximum opened files (required by box.phar):
ulimit -Sn 2048

export PATH="${PATH}:${PWD}"

# Download composer if it does not exist:
command -v composer.phar || {
  #curl -S https://getcomposer.org/installer | php -d detect_unicode=0
  wget -O - https://getcomposer.org/installer | php -d detect_unicode=0
}

# Download box if it does not exist:
command -v box.phar || {
  #curl -S http://box-project.org/installer.php | php -d detect_unicode=0
  wget -O - http://box-project.org/installer.php | php -d detect_unicode=0
}

box="$(command -v box.phar)"

# Build mcc phar:
composer.phar install || exit 1
php -dphar.readonly=0 ${box:=box.phar} build || exit 1
mv mcc.phar mcc || exit 1
echo "mcc has been created."
