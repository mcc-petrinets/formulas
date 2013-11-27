<?php
namespace MCC\Command;

use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Formula\EquivalentPlaces;

class CheckUnfolding extends Command
{

  protected function configure()
  {
    parent::configure();
    $this
      ->setName('check-unfolding')
      ->setDescription('Checks that all places in unfolded model are unfoldings of places in colored model')
      ->addArgument('colored-model', InputArgument::REQUIRED, 'Colored model file (in PNML)')
      ->addArgument('unfolded-model', InputArgument::REQUIRED, 'Unfolded model file (in PNML)');
  }

  protected function execute(InputInterface $input, OutputInterface $output)
  {
    $cmodelfile = $input->getArgument('colored-model');
    $umodelfile = $input->getArgument('unfolded-model');
    $cmodel = simplexml_load_file($cmodelfile, NULL, LIBXML_COMPACT);
    $umodel = simplexml_load_file($umodelfile, NULL, LIBXML_COMPACT);
    $ep = new EquivalentPlaces($cmodel, $umodel);

    // Check that all unfolded places belong to a colored one:
    $c = 0;
    foreach ($ep->cplaces as $place)
    {
      $c += count($place->unfolded);
    }
    if ($c != count($ep->uplaces))
    {
      echo "     Inconsistency detected!\n";
    }

    // Check that the maximum parameter is reached at least once:
    $maximum_reached = array();
    foreach ($ep->cplaces as $place)
    {
      $id = $place->id;
      $size = 1;
      $where = 1;
      $max = count($place->domain->values);
      $p = '';
      foreach ($place->domain->values as $k => $v)
      {
        $first = true;
        $last = end(array_values($v));
        $p .= '_((' . $last;
        if ($where != $max)
        {
          $size *= count($v);
          $p .= '_.*)|((';
          foreach ($v as $vid => $vname)
          {
            if ($first)
            {
              $first = false;
            } else
            {
              $p .= '|';
            }
            $p .= $vname;
          }
          $p .= ')';
        }
        $where++;
      }
      for ($i = 0; $i < 2*count($place->domain->values); $i++)
      {
        $p .= ')';
      }
      $regex = "^{$place->name}{$p}$";
      $maximum_reached[$id] = false;
      error_reporting(E_ERROR);
      foreach ($ep->uplaces as $uplace)
      {
        if ((size < 1000) && preg_match('/' . $regex . '/', $uplace->name))
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
    $m = count($ep->cplaces);
    if ($n < $m/2)
    {
      echo "     Maximum parameter is reached for only {$n}/{$m} places.\n";
    }
  }

}
