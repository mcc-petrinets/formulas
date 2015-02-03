<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class CheckUnfolding extends Base
{

  protected function configure()
  {
    $this
      ->setName('model:check-unfolding')
      ->setDescription('Check if P/T and SN are consistent');
    parent::configure();
  }

  private $log_name = 'model-check.log';
  private $log;
  private $ep;

  private function lines_of($file)
  {
    $result = 0;
    $handle = fopen($file, "r");
    while(! feof($handle))
    {
      $line = fgets($handle);
      $result++;
    }
    fclose($handle);
    return $result - 1;
  }

  protected function perform()
  {
    if ($this->sn_model && $this->pt_model)
    {
      $dir = dirname($this->pt_file);
      $this->log = fopen("{$dir}/{$this->log_name}", 'w');
      $this->ep = new EquivalentElements($this->sn_model, $this->pt_model);
      $quantity = count($this->ep->cplaces) +
        count($this->ep->cplaces) +
        count($this->ep->ctransitions);
      $this->progress->setRedrawFrequency(max(1, $quantity / 100));
      $this->progress->start($this->console_output, $quantity);
      $this->check_places();
      $this->check_parameters();
      $this->check_transitions();
      $this->progress->finish();
      fclose($this->log);
      $count = $this->lines_of("{$dir}/{$this->log_name}");
      if ($count > 0)
      {
        $this->console_output->writeln(
          "<warning>  Problems have been detected, see {$dir}/{$this->log_name} for details.</warning>"
        );
      }
    }
  }

  private function check_places()
  {
    // Check that all unfolded places belong to a colored one:
    $c = 0;
    foreach ($this->ep->cplaces as $place)
    {
      // Check that all colored places have at least one unfolding:
      if (count($place->unfolded) == 0)
      {
        fwrite($this->log, "Place {$place->id} (named {$place->name}) has no unfolding.\n");
      }
      $c += count($place->unfolded);
      $this->progress->advance();
    }
    if ($c != count($this->ep->uplaces))
    {
      fwrite($this->log, "Unfolded model may use a bigger parameter.\n");
    }
  }

  private function check_parameters()
  {
    // Check that the maximum parameter is reached at least once:
    $maximum_reached = array();
    foreach ($this->ep->cplaces as $place)
    {
      $id = $place->id;
      $size = 1;
      $where = 1;
      $max = count($place->domain->values);
      $p = '';
      foreach ($place->domain->values as $k => $v)
      {
        $first = true;
        $last = end($v);
        $p .= '_((' . $last;
        if ($where != $max)
        {
          $size *= count($v);
          $p .= '_.*)|([^_]+';
        }
        $where++;
      }
      for ($i = 0; $i < 2*count($place->domain->values); $i++)
      {
        $p .= ')';
      }
      $regex = "^{$place->name}{$p}$";
      // If you have trouble understanding what happens,
      // look at the regex!
      $maximum_reached[$id] = false;
      error_reporting(E_ERROR);
      foreach ($this->ep->uplaces as $uplace)
      {
        if ((size < 1000) && preg_match('/' . $regex . '/u', $uplace->name))
        {
          $maximum_reached[$id] = true;
          break;
        }
        else if (ereg($regex, $uplace->name))
        {
          $maximum_reached[$id] = true;
          break;
        }
      }
      error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
      $this->progress->advance();
    }
    $n = 0;
    foreach ($maximum_reached as $i => $b)
    {
      if ($b)
      {
        $n++;
      }
    }
    $m = count($this->ep->cplaces);
    if ($n < $m/2)
    {
      fwrite($this->log, "Maximum parameter is reached for only {$n}/{$m} places.\n");
    }
  }

  private function check_transitions()
  {
    $c = 0;
    // Check that every transtion has at least one unfolding:
    foreach ($this->ep->ctransitions as $transition)
    {
      if (count($transition->unfolded) == 0)
      {
        fwrite($this->log, "Transition {$transition->id} (named {$transition->name}) has no unfolding.\n");
      }
      $c += count($transition->unfolded);
      $this->progress->advance();
    }
    if ($c != count($this->ep->utransitions))
    {
      fwrite($this->log, "Unfolded model may use a bigger parameter.\n");
    }
  }

}

