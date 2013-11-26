#! /usr/bin/env php
<?php

$srcRoot = ".";
$buildRoot = ".";
$phar = new Phar($buildRoot . "/mcc.phar",
  FilesystemIterator::CURRENT_AS_FILEINFO |
  FilesystemIterator::KEY_AS_FILENAME,
  "mcc.phar");

$phar->startBuffering();
$phar->buildFromDirectory(".");
$defaultStub = $phar->createDefaultStub('mcc.php');
$stub = "#!/usr/bin/env php\n" . $defaultStub;
$phar->setStub($stub);
$phar->compressFiles(Phar::GZ);
$phar->stopBuffering();
