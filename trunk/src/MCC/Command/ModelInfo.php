<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class ModelInfo extends Base
{

  protected function configure()
  {
    $this
      ->setName('model:info')
      ->setDescription('Give some information about the model');
    parent::configure();
  }

  protected function perform()
  {
    if ($this->sn_model && $this->pt_model)
    {
      $this->console_output->writeln("Colored with P/T equivalent");
    }
    else if ($this->sn_model && ! $this->pt_model)
    {
      $this->console_output->writeln("Colored only");
    }
    else if (! $this->sn_model && $this->pt_model)
    {
      $this->console_output->writeln("P/T only");
    }
    if ($this->sn_model)
    {
      $this->console_output->writeln("# Colored places: " .
        count($this->sn_model->net->page->place));
      $this->console_output->writeln("# Colored transitions: " .
        count($this->sn_model->net->page->transition));
      $this->console_output->writeln("# Colored arcs: " .
        count($this->sn_model->net->page->arc));
    }
    if ($this->pt_model)
    {
      $this->console_output->writeln("# P/T places: " .
        count($this->pt_model->net->page->place));
      $this->console_output->writeln("# P/T transitions: " .
        count($this->pt_model->net->page->transition));
      $this->console_output->writeln("# P/T arcs: " .
        count($this->pt_model->net->page->arc));
    }
  }

}
