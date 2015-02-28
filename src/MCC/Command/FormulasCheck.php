<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;
use \MCC\Formula\CheckFormula;

class FormulasCheck extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:check')
      ->setDescription('Eliminate trivially satisfied or unsatified formulas')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $sn_input = null;
  private $pt_input = null;
  private $sn_output = null;
  private $pt_output = null;
  private $sn_smt = null;
  private $pt_smt = null;
  private $ep;
  private $checker;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $sn_path = dirname($this->sn_file);
    $pt_path = dirname($this->pt_file);
    $this->sn_input  = "${sn_path}/{$input->getOption('output')}.xml";
    $this->sn_output = "${sn_path}/{$input->getOption('output')}.xml";
    $this->pt_input  = "${pt_path}/{$input->getOption('output')}.xml";
    $this->pt_output = "${pt_path}/{$input->getOption('output')}.xml";
    $this->sn_smt = "${sn_path}/test.smt";
    $this->pt_smt = "${pt_path}/test.smt";
    $this->checker = new CheckFormula();
  }
  
  protected function perform()
  {
//  $this->console_output->writeln("perform " . $this->sn_smt . " " . $this->pt_smt);
    $this->ep = new EquivalentElements($this->sn_model, $this->pt_model);
    if ($this->sn_model != null)
    {
      $this->check
        (
        $this->sn_input,
        $this->sn_output,
        $this->sn_smt,
        $this->ep->cplaces,
        $this->ep->ctransitions
      );
    }
    if ($this->pt_model != null)
    {
      $this->check(
        $this->pt_input,
        $this->pt_output,
        $this->pt_smt,
        $this->ep->uplaces,
        $this->ep->utransitions
      );
    }
  }

  protected function check ($input, $output, $smt, $places, $transitions)
  {
//  $this->console_output->writeln("check " . $smt);
    if (! file_exists($input))
    {
      $this->console_output->writeln(
        "<error>Formula file {$input} not found.</error>"
      );
      return;
    }
    $xml = $this->load_xml(file_get_contents($input));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    $n = 0;
    $to_suppress = array();
    foreach ($xml->property as $property)
    {
      $result = array(true, array(), "");
      $result = $this->checker->perform_check($property->formula->children()[0], $places, 
                                     $transitions, $smt);
      
      if ($result[2] != "")
      {
        $result[0] = $result[0] && $this->checker->call_smt($result, $smt);
      }
      
      if (!$result[0])
      {
        $to_suppress[] = $n;
//        $this->console_output->writeln("On supprime : " . $n);
//        unset($xml->children()[$n]);
      }
      $n++;
      $this->progress->advance();
    }
    $n = 0;
    foreach ($to_suppress as $i)
    {
      unset($xml->children()[$i - $n]);
      $n++;
    }
    $this->progress->finish();
    if (file_exists($output))
      unlink($output);
    file_put_contents($output, $this->save_xml($xml));
  }

  
}
