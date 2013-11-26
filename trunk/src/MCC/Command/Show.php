<?php

namespace MCC\Command;

use \MCC\Data\Contest;
use \MCC\Data\Examination;
use \MCC\Data\Formalism;
use \MCC\Data\Instance;
use \MCC\Data\Model;
use \MCC\Data\Tool;
use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Helper\FormatterHelper;

class Show extends Command {
  protected function configure() {
    $this -> setName('show')
          -> setDescription('Shows the data.')
          -> setHelp(<<<EOF
The <info>%command.name%</info> command shows the contest editions,
formalisms, models, tools, instances and runs.
EOF
             );
  }

  protected function execute(InputInterface $input, OutputInterface $output) {
    global $entityManager;
    $formatter = $this->getHelperSet()->get('formatter');

    $formalisms = $entityManager -> getRepository('\MCC\Data\Formalism') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Formalisms',
      'Count: ' . count($formalisms)
    );
    $output->writeln($formattedLine);
    foreach ($formalisms as $formalism) {
      echo "  {$formalism}\n";
    }

    $tools = $entityManager -> getRepository('\MCC\Data\Tool') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Tools',
      'Count: ' . count($tools)
    );
    $output->writeln($formattedLine);
    foreach ($tools as $tool) {
      echo "  {$tool}\n";
    }
    
    $models = $entityManager -> getRepository('\MCC\Data\Model') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Models',
      'Count: ' . count($models)
    );
    $output->writeln($formattedLine);
    foreach ($models as $model) {
      echo "  {$model}\n";
    }
    
    $instances = $entityManager -> getRepository('\MCC\Data\Instance') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Instances',
      'Count: ' . count($instances)
    );
    $output->writeln($formattedLine);
    foreach ($instances as $instance) {
      echo "  {$instance}\n";
    }
    
    $contests = $entityManager -> getRepository('\MCC\Data\Contest') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Contests',
      'Count: ' . count($contests)
    );
    $output->writeln($formattedLine);
    foreach ($contests as $contest) {
      echo "  {$contest}\n";
    }
    
    $examinations = $entityManager -> getRepository('\MCC\Data\Examination') -> findAll();
    $formattedLine = $formatter->formatSection(
      'Examinations',
      'Count: ' . count($examinations)
    );
    $output->writeln($formattedLine);
    foreach ($examinations as $examination) {
      echo "  {$examination}\n";
    }
  }

}
