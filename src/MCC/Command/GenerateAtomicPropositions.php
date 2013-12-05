<?php
namespace MCC\Command;

use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Formula\AtomicPropositions;

class GenerateAtomicPropositions extends Command
{

  private $root;

  protected function configure()
  {
    parent::configure();
    $this
      ->setName('generate-atomic-propositions')
      ->setDescription('')
      ->addArgument('root', InputArgument::REQUIRED, 'Root of MCC INPUTS.')
      ->addArgument('model', InputArgument::REQUIRED, 'Model name')
      ->addArgument('parameter', InputArgument::REQUIRED, 'Model parameter');
  }

  private function load_model($name)
  {
    $model = NULL;
    $file = "{$this->root}/{$name}/model.pnml";
    if (file_exists($file) and is_file ($file))
    {
      $model = simplexml_load_file($file, NULL, LIBXML_COMPACT);
    }
    else
    {
      echo "File {$file} does not exist.\n";
    }
    return $model;
  }

  protected function execute(InputInterface $input, OutputInterface $output)
  {
    $this->root = $input->getArgument('root');
    if (!file_exists($this->root) or !is_dir($this->root))
    {
      echo "{$this->root} directory does not exist.\n";
      exit(1);
    }
    $name = $input->getArgument('model');
    $parameter = $input->getArgument('parameter');
    $sn_model = $this->load_model("{$name}-COL-{$parameter}");
    $pt_model = $this->load_model("{$name}-PT-{$parameter}");
    $f = new AtomicPropositions($sn_model, $pt_model);
    $deadlocks = $f->deadlock();
    echo $deadlocks['sn']->asXml() . "\n";
    echo $deadlocks['pt']->asXml() . "\n";
  }

}
