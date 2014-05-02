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
  private $post;
  private $places;
  private $transitions;
  private $marked_in_cycle;

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
    $this->post = array();
    $this->places = array();
    $this->transitions = array();
    $this->marked_in_cycle = 1;
  }


  private function find_cycles($id, $start = null, &$stack = array())
  {
    if ($start == null)
      $start = $id;
    if (array_key_exists($start, $this->cycles) &&
      (count($stack) >= count($this->cycles[$start])))
      return;
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
            $rhs_size = count ($stack);
//            echo "Add cycle of size {$rhs_size}.\n";
            $this->cycles[$start] = $stack;
          }
          else
          {
            $lhs_size = count ($this->cycles[$start]);
            $rhs_size = count ($stack);
            if ($rhs_size < $lhs_size)
            {
//              echo "Update cycle of size {$rhs_size}.\n";
              $this->cycles[$start] = $stack;
            }
          }
          /*
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
          */
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
      if (! array_key_exists($source, $this->post))
      {
        $this->post[$target] = array();
      }
      $this->post[$target][$source] = $arc;
    }

    $quantity = count($model->net->page->place);
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    // Search cycles
    $remaining = array();
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $remaining[$id] = true;
    }
    $marked = array();
    foreach ($model->net->page->place as $place)
    {
      $initial = (string) $place->initialMarking->text;
      $id = (string) $place->attributes()['id'];
      if (($initial != NULL) && ($initial != ""))
        $marked[$id] = true;
    }
    $associated = array();
    while (true)
    {
      $m = array();
      foreach ($marked as $id => $v)
      {
        if (array_key_exists($id, $associated))
          continue;
        $this->find_cycles($id);
        if (array_key_exists($id, $this->cycles))
        {
          foreach ($this->cycles[$id] as $k => $v)
          {
            $associated[$k] = $id;
            unset ($remaining[$k]);
          }
          $this->progress->advance(count($this->cycles[$id]));
        }
        else
          $m[$id] = true;
      }
      if (count($m) == 0)
        break;
      $marked = $m;
      $this->marked_in_cycle++;
    }
    print_r ($this->cycles);
    $groups_default = array();
    foreach ($this->cycles as $start => $cycle)
    {
      $groups_default[$start] = 0;
    }
    $last_count = 0;
    $divider = 2;
    while (count($remaining) != 0)
    foreach ($remaining as $id => $v)
    {
      echo "Searching id=" . $id . "\n";
      $groups = $groups_default;
      $nb_arcs = 0;
      if (array_key_exists($id, $this->post))
      foreach ($this->post[$id] as $t => $arc)
      {
        if (array_key_exists($t, $this->post))
        foreach ($this->post[$t] as $p => $arc)
        {
//            echo $p . "  " . $id . "\n";
          if ($p == $id)
            continue;
          else if (array_key_exists($p, $associated))
            $groups[$associated[$p]] += 1;
          $nb_arcs++;
        }
        if (array_key_exists($t, $this->pre))
        foreach ($this->pre[$t] as $p => $arc)
        {
//            echo $p . "  " . $id . "\n";
          if ($p == $id)
            continue;
          else if (array_key_exists($p, $associated))
            $groups[$associated[$p]] += 1;
          $nb_arcs++;
        }
      }
      if (array_key_exists($id, $this->pre))
      foreach ($this->pre[$id] as $t => $arc)
      {
        if (array_key_exists($t, $this->pre))
        foreach ($this->pre[$t] as $p => $arc)
        {
//            echo $p . "  " . $id . "\n";
          if ($p == $id)
            continue;
          else if (array_key_exists($p, $associated))
            $groups[$associated[$p]] += 1;
          $nb_arcs++;
        }
        if (array_key_exists($t, $this->post))
        foreach ($this->post[$t] as $p => $arc)
        {
//            echo $p . "  " . $id . "\n";
          if ($p == $id)
            continue;
          else if (array_key_exists($p, $associated))
            $groups[$associated[$p]] += 1;
          $nb_arcs++;
        }
      }
      print_r ($groups);
      // Find group with highest value:
//      echo "Place " . $id . "\n";
//      print_r($groups);
      $m = max($groups);
      echo "sum=" . array_sum($groups) . "\n";
      echo "value=" . ($nb_arcs / $divider) . "\n";
      echo "divider=" . $divider . "\n";
      if (($m == 0) || array_sum($groups) < $nb_arcs / $divider)
      {}
      else
      {
        echo "here\n";
        $group = array_search($m, $groups);
        unset ($remaining[$id]);
        $associated[$id] = $group;
        $this->cycles[$group][$id] = 1;
        $this->progress->advance();
      }
      echo "last_count=" . $last_count . "\n";
      if (count($remaining) == $last_count)
        $divider *= 2;
      $last_count = count($remaining);
    }
    $this->progress->finish();

    // Fix representation of data:
    foreach ($this->cycles as $place => $cycle)
    {
      $tmp = array();
      foreach ($cycle as $id => $useless)
      {
        $tmp[] = $id;
      }
      $this->cycles[$place] = $tmp;
    }

    // Output to JSON:
    $dir = dirname($this->pt_file);
    file_put_contents("{$dir}/order.json", json_encode($this->cycles));

    // Also output to text:
    $output = array();
    foreach ($this->cycles as $place => $cycle)
    {
      $output[] = "from " . $place . ": " . implode(", ", $cycle);
    }
    $txt = implode("\n", $output) . "\n";
    file_put_contents("{$dir}/order.txt", $txt);
  }
}
