<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;

class FormulasAddNamespace extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:add-namespace')
      ->setDescription('Add XML namespace to formulas')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas', 'formulas')
        ;
    parent::configure();
  }

  private $output;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->output = "{$input->getOption('output')}.xml";
  }

  protected function perform()
  {
    if ($this->sn_model != null)
    {
      $file = dirname($this->sn_file) . '/' . $this->output;
      $this->convert($file);
    }
    if ($this->pt_model != null)
    {
      $file = dirname($this->pt_file) . '/' . $this->output;
      $this->convert($file);
    }
  }

  private function convert ($file)
  {
    if (! file_exists($file))
    {
      $this->console_output->writeln(
          "  <warning>Formula file '{$file}' does not exist.</warning>"
        );
      return;
    }
    $xml = $this->load_xml(file_get_contents($file));
    $quantity = 1;
    $this->progress->setRedrawFrequency(1);
    $this->progress->start($this->console_output, $quantity);
    $xml->addAttribute("xmlns", "http://mcc.lip6.fr/");
    $this->progress->advance();
    $this->progress->finish();
    file_put_contents($file, $this->save_xml($xml));
  }

}
