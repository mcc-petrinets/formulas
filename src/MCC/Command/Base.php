<?php
namespace MCC\Command;

use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Output\OutputInterface;

class Instance
{
  public $model;
  public $paramater;

  public function __construct($model, $parameter)
  {
    $this->model = $model;
    $this->parameter = $parameter;
  }

}

abstract class Base extends Command
{

  protected $sn_model;
  protected $pt_model;
  protected $sn_file;
  protected $pt_file;

  protected function configure()
  {
    parent::configure();
    $this
      ->addArgument('root', InputArgument::REQUIRED, 'Root of MCC INPUTS.')
      ->addArgument('model', InputArgument::OPTIONAL, 'Model name')
      ->addArgument('parameter', InputArgument::OPTIONAL, 'Model parameter');
  }

  private function load_model($file)
  {
    $model = NULL;
    if (file_exists($file) and is_file ($file))
    {
      $model = simplexml_load_file($file, NULL, LIBXML_COMPACT);
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
    $model = $input->getArgument('model');
    $parameter = $input->getArgument('parameter');
    if (!$model && $parameter)
    {
      echo "Parameter can only be specified if model is given.\n";
      exit(1);
    }
    // Compute instances:
    $instances = array();
    $regex = '^' .
      ($model ? '(' . $model . ')' : '(.*)') .
      '-(COL|PT)-' .
      ($parameter ? '(' . $parameter . ')' : '(.*)') .
      '$';
    foreach (scandir($this->root) as $entry)
    {
      $matches = array();
      if (preg_match('/' . $regex . '/u', $entry, $matches))
      {
        $instances[] = new Instance($matches[1], $matches[3]);
      }
    }
    $instances = array_unique($instances, SORT_REGULAR);
    // Iterate over instances to perform action:
    foreach ($instances as $instance)
    {
      echo "Apply on {$instance->model} - {$instance->parameter}\n";
      $this->sn_file = "{$this->root}/{$instance->model}-COL-{$instance->parameter}/model.pnml";
      $this->sn_model = $this->load_model($this->sn_file);
      $this->pt_file = "{$this->root}/{$instance->model}-PT-{$instance->parameter}/model.pnml";
      $this->pt_model = $this->load_model($this->pt_file);
      $this->perform();
    }
  }

  abstract protected function perform();

}
