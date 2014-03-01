<?php
ini_set("memory_limit","512M");
ini_set("pcre.backtrack_limit", "99999999");
ini_set("pcre.recursion_limit", "99999999");

use \Doctrine\ORM\Tools\Setup;
use \Doctrine\ORM\EntityManager;
use \Symfony\Component\Console\Application;
use \MCC\Command\Import;
use \MCC\Command\Show;
use \MCC\Command\Statistics;
use \MCC\Command\CheckUnfolding;
use \MCC\Command\FixModel;
use \MCC\Command\GenerateFormulas;
use \MCC\Command\TagFormulas;
use \MCC\Command\UnfoldFormulas;
use \MCC\Command\InstantiateFormulas;
use \MCC\Command\FormulasToText;
use \MCC\Command\GenerateC;
use \MCC\Command\GenerateLua;
use \MCC\Command\ToCami;

require_once "vendor/autoload.php";

$isDevMode = false;
$config = Setup::createAnnotationMetadataConfiguration(array(__DIR__."/src/MCC/Data/"), $isDevMode);
$conn = array(
    'driver' => 'pdo_sqlite',
    'path' => __DIR__ . '/db.sqlite',
);
$entityManager = EntityManager::create($conn, $config);
$helperSet = new \Symfony\Component\Console\Helper\HelperSet(
  array(
    'em' => new \Doctrine\ORM\Tools\Console\Helper\EntityManagerHelper($entityManager)
  )
);

$application = new Application('Model Checking Contest Utilities', '0.1');
$application->add(new Import);
$application->add(new Show);
$application->add(new Statistics);
$application->add(new CheckUnfolding());
$application->add(new FixModel());
$application->add(new GenerateFormulas());
$application->add(new TagFormulas());
$application->add(new UnfoldFormulas());
$application->add(new InstantiateFormulas());
$application->add(new FormulasToText());
$application->add(new GenerateC());
$application->add(new GenerateLua());
$application->add(new ToCami());
$application->run();
