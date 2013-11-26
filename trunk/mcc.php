<?php
ini_set("memory_limit","512M");
ini_set("pcre.backtrack_limit", "99999999");
ini_set("pcre.recursion_limit", "99999999");

//require_once "phar://mcc.phar/" . "autoloader.php";
require_once "Bootstrap.php";

use \Symfony\Component\Console\Application;
use \MCC\Command\Import;
use \MCC\Command\Show;
use \MCC\Command\Statistics;
use \MCC\Command\Places;
use \MCC\Command\FixModel;

$application = new Application('Model Checking Contest Utilities', '0.1');
$application->add(new Import);
$application->add(new Show);
$application->add(new Statistics);
$application->add(new Places());
$application->add(new FixModel());
$application->run();
