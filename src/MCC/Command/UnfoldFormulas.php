<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;
use \MCC\Formula\FormulaUnfolder;

class UnfoldFormulas extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:unfold')
      ->setDescription('Unfold formulas')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $input = null;
  private $output = null;
  private $ep;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    if (($this->sn_model == null) || ($this->pt_model == null))
    {
      return;
    }
    $input_path = dirname($this->sn_file);
    $output_path = dirname($this->pt_file);
    $this->input = "${input_path}/{$input->getOption('output')}.xml";
    $this->output = "${output_path}/{$input->getOption('output')}.xml";
  }

  protected function perform()
  {
    if (($this->sn_model == null) || ($this->pt_model == null))
    {
      return;
    }

    $unfolder = new FormulaUnfolder ($this->sn_model, $this->pt_model);

    if (file_exists($this->output))
      unlink($this->output);
    $xml = $this->load_xml(file_get_contents($this->input));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    foreach ($xml->property as $property)
    {
      $unfolder->unfold ($property->formula->children()[0]);
      $this->progress->advance();
    }
    $this->progress->finish();
    file_put_contents($this->output, $this->save_xml($xml));
  }

}
