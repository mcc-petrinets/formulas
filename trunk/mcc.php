<?php
ini_set("memory_limit","2G");
ini_set("pcre.backtrack_limit", "99999999");
ini_set("pcre.recursion_limit", "99999999");

use \Doctrine\ORM\Tools\Setup;
use \Doctrine\ORM\EntityManager;
use \Symfony\Component\Console\Application;

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
$application->add(new \MCC\Command\Import);
$application->add(new \MCC\Command\Show);
$application->add(new \MCC\Command\Statistics);
$application->add(new \MCC\Command\CheckUnfolding());
$application->add(new \MCC\Command\FixModel());
$application->add(new \MCC\Command\ModelInfo());
$application->add(new \MCC\Command\CleanFormulas());
$application->add(new \MCC\Command\GenerateFormulas());
$application->add(new \MCC\Command\TagFormulas());
$application->add(new \MCC\Command\FormulasAddNamespace());
$application->add(new \MCC\Command\UnfoldFormulas());
$application->add(new \MCC\Command\InstantiateFormulas());
$application->add(new \MCC\Command\FormulasToText());
$application->add(new \MCC\Command\FormulasToVIS());
$application->add(new \MCC\Command\GenerateC());
$application->add(new \MCC\Command\GenerateLua());
$application->add(new \MCC\Command\ToCami());
$application->add(new \MCC\Command\ToCosy());
$application->add(new \MCC\Command\Order());
$application->run();
