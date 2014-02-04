<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class ToCami extends Base
{

  protected function configure()
  {
    $this
      ->setName('to-cami')
      ->setDescription('TODO');
    parent::configure();
  }

  protected function perform()
  {
    if ($this->pt_model == NULL) {
      return;
    }
    $model = $this->pt_model;
    $cami_id = 1;
    $equivalence = array();
    $file = fopen(preg_replace('/.pnml$/', '.cami', $this->pt_file), 'w');
    fwrite($file, "CN(3:net,${cami_id})\n");
    $cami_id++;
    foreach ($model->net->page->place as $place)
    {
      $id      = (string) $place->attributes()['id'];
      $name    = (string) $place->name->text;
      $marking = (string) $place->initialMarking->text;
      if ($marking == "") {
        $marking = "1";
      }
      $id_length      = strlen($id);
      $marking_length = strlen($marking);
      fwrite($file, "CN(5:place,${cami_id})\n");
      fwrite($file, "CT(4:name,${cami_id},${id_length}:${id})\n");
      fwrite($file, "CT(7:marking,${cami_id},${marking_length}:${marking})\n");
      $equivalence[$id] = $cami_id;
      $cami_id++;
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
    }
    fclose($file);
  }


} 
