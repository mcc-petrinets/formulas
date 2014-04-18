<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class Order extends Base
{

  private $cycles;
  private $pre;
  private $places;
  private $transitions;

  protected function configure()
  {
    $this
      ->setName('model:generate-order')
      ->setDescription('Generate an order in JSON');
    parent::configure();
  }

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->cycles = array();
    $this->pre = array();
    $this->places = array();
    $this->transitions = array();
  }


  private function find_cycles($id, $start = null, &$stack = array())
  {
    if ($start == null)
      $start = $id;
    if (array_key_exists($id, $this->places))
      $stack[$id] = true;
    if (array_key_exists($id, $this->pre))
    {
      foreach ($this->pre[$id] as $succ => $arc)
      {
//        echo "start = {$start}, id = {$id}, succ = {$succ}\n";
        if (array_key_exists($succ, $stack))
        {
          if (count($stack) <= 1)
          {
            continue;
          }
          if (! array_key_exists($start, $this->cycles))
          {
            $this->cycles[$start] = array();
          }
          ksort($stack);
          foreach ($this->cycles[$start] as $k => $s)
          {
            $ldiff = array_diff_assoc($s, $stack);
            $rdiff = array_diff_assoc($stack, $s);
            if (count($ldiff) != 0 && count($rdiff) == 0)
            {
              // Nothing to do
            } else if (count($ldiff) == 0 && count($rdiff) != 0)
            {
              $this->cycles[$start][$k] = $stack;
            } else if (count($ldiff) == 0 && count($rdiff) == 0)
            {
              continue 2;
            }
          }
          $size = count($stack);
          echo "Add stack of size {$size} for {$start}.\n";
          print_r($stack);
          $this->cycles[$start][] = $stack;
          $c = count($this->cycles[$start]);
          echo "# {$c}\n";
        }
        else
        {
          if (array_key_exists($succ, $this->places))
          {
            $initial = (string) $this->places[$succ]->initialMarking->text;
            if (($initial != NULL) && ($initial != ""))
            {
              continue;
            }
          }
          $this->find_cycles($succ, $start, $stack);
        }
      }
    }
    if (array_key_exists($id, $this->places))
      unset($stack[$id]);
  }

  protected function perform()
  {
    if ($this->pt_model == NULL)
    {
      return;
    }
    $model = $this->pt_model;
    // Algorithm:
    // 1. find all initially marked places
    // 2. for each one, find cycles (directed graph)
    //
    // Build relation between IDs and arcs:
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $this->places[$id] = $place;
    }
    foreach ($model->net->page->transitions as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $this->transitions[$id] = $transition;
    }
    foreach ($model->net->page->arc as $arc)
    {
      $source = (string) $arc->attributes()['source'];
      $target = (string) $arc->attributes()['target'];
      if (! array_key_exists($source, $this->pre))
      {
        $this->pre[$source] = array();
      }
      $this->pre[$source][$target] = $arc;
    }

    // Search cycles:
    foreach ($model->net->page->place as $place)
    {
      $initial = (string) $place->initialMarking->text;
      if (($initial != NULL) && ($initial != ""))
      {
        $id = (string) $place->attributes()['id'];
        echo "Searching for place {$id}\n";
        $this->find_cycles($id);
      }
    }

    $c = count($this->cycles);
    print ("Found {$c} cycles.\n");
    print_r($this->cycles);

    /*
    $keys = array_keys($this->cycles);
    $merges = array();
    for ($i = 0; $i < count($keys); $i++)
    {
      $with = array();
      for ($j = $i+1; $j < count($keys); $j++)
      {
        $lhs = $this->cycles[$keys[$i]];
        $rhs = $this->cycles[$keys[$j]];
        $inter = array_intersect_assoc($lhs, $rhs);
        if (count($inter) != 0)
        {
          $with[] = $j;
        }
      }
      if (count($with) == 1)
      {
        $l = $keys[$i];
        $r = $keys[$with[0]];
        $merges[] = array($r, $l);
      }
    }
    foreach ($merges as $merge)
    {
      $l = $merge[0];
      $r = $merge[1];
      $this->cycles[$l] = array_merge($this->cycles[$l], $this->cycles[$r]);
      unset($this->cycles[$r]);
    }

    $c = count($this->cycles);
    print ("Found {$c} cycles.\n");
    print_r($this->cycles);

    // Add remaining places:
    $all_places = array();
    foreach ($this->cycles as $cycle)
    {
      foreach ($cycle as $k => $v)
      {
        if (array_key_exists($k, $this->places))
        {
          $all_places[$k] = true;
        }
      }
    }
    */

    // Sort and add remaining places:

  }
}
