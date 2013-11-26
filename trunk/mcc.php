<?php
ini_set("memory_limit","512M");

//require_once "phar://mcc.phar/" . "autoloader.php";
require_once "Bootstrap.php";

use \Symfony\Component\Console\Application;
use MCC\Command\Import;
use MCC\Command\Show;
use MCC\Command\Statistics;
use \MCC\Command\Places;

$application = new Application('Model Checking Contest Utilities', '0.1');
$application->add(new Import);
$application->add(new Show);
$application->add(new Statistics);
$application->add(new Places());
$application->run();
