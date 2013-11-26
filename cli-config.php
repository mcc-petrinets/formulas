<?php

use Doctrine\ORM\Tools\Console\ConsoleRunner;

require_once 'Bootstrap.php';

$helperSet = new \Symfony\Component\Console\Helper\HelperSet(array(
    'em' => new \Doctrine\ORM\Tools\Console\Helper\EntityManagerHelper($entityManager)
));
