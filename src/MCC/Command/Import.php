<?php
namespace MCC\Command;

use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Import\CSV2013;
use \Doctrine\ORM\Tools\SchemaTool;

class Import extends Command {
  protected function configure() {
    $this -> setName('import')
          -> setDescription('Import data.')
          -> addArgument('year', InputArgument::REQUIRED, 'Edition of the MCC?')
          -> addArgument('directory', InputArgument::REQUIRED, 'Directory where csv files are stored?')
          -> setHelp(<<<EOF
The <info>%command.name%</info> command cleans the database
and imports the data of a particular MCC edition.

Edition of the MCC is given as a year. Currently, only 2013 is handled.

Data is given as a directory containing <info>comma separated values</info> files.
EOF
             );
  }

  protected function execute(InputInterface $input, OutputInterface $output) {
    $this -> generateDatabase($input, $output);
    $year = $input -> getArgument('year');
    $resultsdir = $input -> getArgument('directory');
    if (file_exists($resultsdir)) {
      $importer = new CSV2013($year, $resultsdir);
      $importer -> call($this, $output);
    } else {
      $output -> writeln("Directory {$resultsdir} does not exist.");
    }
  }

  protected function generateDatabase(InputInterface $input, OutputInterface $output) {
    global $entityManager;
    $classes = array(
      $entityManager -> getClassMetadata('\MCC\Data\Contest'),
      $entityManager -> getClassMetadata('\MCC\Data\Examination'),
      $entityManager -> getClassMetadata('\MCC\Data\Formalism'),
      $entityManager -> getClassMetadata('\MCC\Data\Instance'),
      $entityManager -> getClassMetadata('\MCC\Data\Model'),
      $entityManager -> getClassMetadata('\MCC\Data\Tool')
    );
    $tool = new SchemaTool($entityManager);
    $tool -> dropSchema($classes);
    $tool -> createSchema($classes);
  }

}
