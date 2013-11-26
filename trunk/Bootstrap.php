<?php

require_once "vendor/autoload.php";

use \Doctrine\ORM\Tools\Setup;
use \Doctrine\ORM\EntityManager;

$isDevMode = false;
$config = Setup::createAnnotationMetadataConfiguration(array(__DIR__."/src/MCC/Data/"), $isDevMode);
$conn = array(
    'driver' => 'pdo_sqlite',
    'path' => __DIR__ . '/db.sqlite',
);
$entityManager = EntityManager::create($conn, $config);
