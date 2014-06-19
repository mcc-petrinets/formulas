<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class ToCami extends Base
{

  protected function configure()
  {
    $this
      ->setName('model:to-cami')
      ->setDescription('Convert PNML (P/T) to CAMI')
      ->addOption('wrap', null, InputOption::VALUE_NONE, 'Wrap CAMI in DB() ... FB()');
    parent::configure();
  }

  private $wrap;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->wrap = $input->getOption('wrap');
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
    $cami_id = 1;
    $equivalence = array();
    $file = fopen(preg_replace('/.pnml$/', '.cami', $this->pt_file), 'w');
    if ($this->wrap)
    {
      fwrite($file, "DB()\n");
    }
    fwrite($file, "CN(3:net,${cami_id})\n");
    $cami_id++;
    foreach ($model->net->page->place as $place)
    {
      $id      = (string) $place->attributes()['id'];
      $name    = (string) $place->name->text;
      $marking = (string) $place->initialMarking->text;
      if ($marking == "") {
        $marking = "0";
      }
      $id_length      = strlen($id);
      $marking_length = strlen($marking);
      fwrite($file, "CN(5:place,${cami_id})\n");
      fwrite($file, "CT(4:name,${cami_id},${id_length}:${id})\n");
      fwrite($file, "CT(7:marking,${cami_id},${marking_length}:${marking})\n");
      $equivalence[$id] = $cami_id;
      $cami_id++;
      $this->progress->advance();
    }
    foreach ($model->net->page->transition as $transition)
    {
      $id   = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      $id_length = strlen($id);
      fwrite($file, "CN(10:transition,${cami_id})\n");
      fwrite($file, "CT(4:name,${cami_id},${id_length}:${id})\n");
      $equivalence[$id] = $cami_id;
      $cami_id++;
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
      $val_length = strlen($valuation);
      $inputid    = $equivalence[$source];
      $outputid   = $equivalence[$target];
      fwrite($file, "CA(3:arc,${cami_id},${inputid},${outputid})\n");
      fwrite($file, "CT(9:valuation,${cami_id},${val_length}:${valuation})\n");
//      $equivalence[$id] = $cami_id;
      $cami_id++;
      $this->progress->advance();
    }
    if ($this->wrap)
    {
      fwrite($file, "FB()\n");
    }
    fclose($file);
    $this->progress->finish();
  }


} 
