<?php
namespace MCC\Command;

use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Formatter\OutputFormatterStyle;
use \Symfony\Component\Console\Helper\ProgressHelper;

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
  protected $model_name;
  protected $parameter;
  protected $console_input;
  public    $console_output; // we need it in CheckFormula, sorry !
  protected $progress;

  protected function configure()
  {
    parent::configure();
    $this
      ->addArgument('root', InputArgument::REQUIRED, 'Root of MCC models')
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

  protected final function execute(InputInterface $input, OutputInterface $output)
  {
    $this->console_input  = $input;
    $this->console_output = $output;
    $this->root = $input->getArgument('root');

    $warning_style = new OutputFormatterStyle('yellow', 'red', array('bold'));
    $output->getFormatter()->setStyle('warning', $warning_style);
    $command_style = new OutputFormatterStyle('blue', 'black', array('bold'));
    $output->getFormatter()->setStyle('command', $command_style);
    $progress_style = new OutputFormatterStyle('cyan', 'black', array());
    $output->getFormatter()->setStyle('progress', $progress_style);
    $instance_style = new OutputFormatterStyle('green', 'black', array('bold'));
    $output->getFormatter()->setStyle('instance', $instance_style);
    $this->progress = $this->getHelperSet()->get('progress');
    $this->progress->setFormat(ProgressHelper::FORMAT_VERBOSE);
    $this->progress->setBarCharacter('<progress>-</progress>');
    $this->progress->setEmptyBarCharacter(' ');
    $this->progress->setProgressCharacter('>');
    $this->progress->setBarWidth(25);

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
      if (is_file($this->root . '/' . $entry))
      {
        try {
          $p = new \PharData($this->root . '/' . $entry);
          $p->extractTo($this->root);
          $output->writeln("<command>Expanded</command> file <instance>{$entry}</instance>");
          unlink($this->root . '/' . $entry);
        }
        catch (Exception $e)
        {}
      }
    }
    foreach (scandir($this->root) as $entry)
    {
      if (is_dir($this->root . '/' . $entry))
      {
        $matches = array();
        if (preg_match('/' . $regex . '/u', $entry, $matches))
        {
          $instances[] = new Instance($matches[1], $matches[3]);
        }
      }
    }
    $instances = array_unique($instances, SORT_REGULAR);
    $model_length = 0;
    $parameter_length = 0;
    $description_length = 0;
    foreach ($this->getApplication()->all() as $c)
    {
      $description_length =
        max($description_length, strlen($c->getDescription()));
    }
    // Iterate over instances to perform action:
    foreach ($instances as $instance)
    {
      $this->model_name = $instance->model;
      $this->parameter = $instance->parameter;
      $description = str_pad($this->getDescription(), $description_length);
      $output->writeln("<command>{$description}</command> on <instance>{$instance->model} - {$instance->parameter}</instance>");
      $this->sn_file = "{$this->root}/{$instance->model}-COL-{$instance->parameter}/model.pnml";
      $this->sn_model = $this->load_model($this->sn_file);
      $this->pt_file = "{$this->root}/{$instance->model}-PT-{$instance->parameter}/model.pnml";
      $this->pt_model = $this->load_model($this->pt_file);
      $this->pre_perform($input, $output);
      $this->perform();
    }
  }

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
  }

  public function load_xml($xml)
  {
    return new \SimpleXmlElement($xml, LIBXML_COMPACT | LIBXML_NOBLANKS);
  }

  protected function save_xml($xml)
  {
    $dom = new \DOMDocument('1.0');
    $dom->formatOutput = true;
    $dom->loadXML($xml->asXml());
    $dom->preserveWhiteSpace = false;
    return $dom->saveXML();
  }

  protected function save_xml_to_file($xml, $path)
  {
    // this is like the function before but saving to a file rather than to a string
    $dom = new \DOMDocument('1.0');
    $dom->formatOutput = true;
    $dom->loadXML($xml->asXml());
    $dom->preserveWhiteSpace = false;
    $dom->save($path);
  }

  // http://stackoverflow.com/questions/4778865/php-simplexml-addchild-with-another-simplexmlelement
  public function xml_adopt($root, $new)
  {
    if ($new == null)
    {
      $msg = "Error: xml_adopt: not enough operators to generate formula, internal error.";
      throw new \Exception ($msg);
    }
    $node = $root->addChild($new->getName(), (string) $new);
    foreach($new->attributes() as $attr => $value)
    {
      $node->addAttribute($attr, $value);
    }
    foreach($new->children() as $ch)
    {
      $this->xml_adopt($node, $ch);
    }
  }

  abstract protected function perform();

}
