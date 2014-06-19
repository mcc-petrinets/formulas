<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class ToCosy extends Base
{

  protected function configure()
  {
    $this
      ->setName('model:to-cosy')
      ->setDescription('Convert PNML (P/T) to Cosy')
      ;
    parent::configure();
  }

  protected function perform()
  {
    if ($this->pt_model == NULL) {
      return;
    }
    $model = $this->pt_model;
    $quantity = count($model->net->page->place) +
      count($model->net->page->transition) +
      count($model->net->page->arc);
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    $file = fopen(preg_replace('/.pnml$/', '.cosy.lua', $this->pt_file), 'w');
    fwrite($file, "local type       = {}\n");
    fwrite($file, "local place      = {}\n");
    fwrite($file, "local transition = {}\n");
    fwrite($file, "local arc        = {}\n");
    fwrite($file, "local model = {}\n");
    foreach ($model->net->page->place as $place)
    {
      $id      = (string) $place->attributes()['id'];
      $name    = (string) $place->name->text;
      $marking = (string) $place->initialMarking->text;
      if ($marking == "") {
        $marking = "0";
      }
      fwrite($file, "do\n");
      fwrite($file, "  local p = {}\n");
      fwrite($file, "  p[type] = place\n");
      fwrite($file, "  p.name = '{$name}'\n");
      fwrite($file, "  p.marking = {$marking}\n");
      fwrite($file, "  model['{$id}'] = p\n");
      fwrite($file, "end\n");
      $this->progress->advance();
    }
    foreach ($model->net->page->transition as $transition)
    {
      $id   = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      fwrite($file, "do\n");
      fwrite($file, "  local t = {}\n");
      fwrite($file, "  t[type] = transition\n");
      fwrite($file, "  t.name = '{$name}'\n");
      fwrite($file, "  model['{$id}'] = t\n");
      fwrite($file, "end\n");
      $this->progress->advance();
    }
    foreach ($model->net->page->arc as $arc)
    {
      $id        = (string) $arc->attributes()['id'];
      $source    = (string) $arc->attributes()['source'];
      $target    = (string) $arc->attributes()['target'];
      $valuation = (string) $arc->inscription->text;
      if ($valuation == "") {
        $valuation = "1";
      }
      fwrite($file, "do\n");
      fwrite($file, "  local a = {}\n");
      fwrite($file, "  a[type] = arc\n");
      fwrite($file, "  a.source = model['{$source}']\n");
      fwrite($file, "  a.target = model['{$target}']\n");
      fwrite($file, "  a.valuation = {$valuation}\n");
      fwrite($file, "  model[#model + 1] = a\n");
      fwrite($file, "end\n");
      $this->progress->advance();
    }
    fclose($file);
    $this->progress->finish();
  }


} 
