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
      ->setName('check-unfolding')
      ->setDescription('Checks if P/T unfoldings correspond to the original Colored nets');
    parent::configure();
  }

  private $ep;

  protected function perform()
  {
    $this->ep = new EquivalentElements($this->sn_model, $this->pt_model);
    if ($this->sn_model && $this->pt_model)
    {
      $this->check_places();
      $this->check_parameters();
      $this->check_transitions();
    }
  }

  private function check_places()
  {
    // Check that all unfolded places belong to a colored one:
    $c = 0;
    foreach ($this->ep->cplaces as $place)
    {
      $c += count($place->unfolded);
    }
    if ($c != count($this->ep->uplaces))
    {
      echo "  Inconsistency detected: unfolded model may use a bigger parameter!\n";
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
      echo "  Inconsistency detected: maximum parameter is reached for only {$n}/{$m} places.\n";
    }
  }

  private function check_transitions()
  {
    // Check that every transtion has at least one unfolding:
    foreach ($this->ep->ctransitions as $transition)
    {
      if (count($transition->unfolded) == 0)
      {
        $name = $transition->name;
        echo "  Inconsistency detected: transition {$name} has no unfolding.\n";
      }
    }
  }

}

