<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;
use \MCC\Formula\AtomicPropositions;
use \MCC\Formula\EquivalentElements;

class GenerateAtomicPropositions extends Base
{

  protected function configure()
  {
    $this->setName('generate')
         ->setDescription('TODO');
    parent::configure();
  }

  protected function perform()
  {
    $f = new AtomicPropositions($this->sn_model, $this->pt_model);

    // Show deadlock formulas:
    $deadlocks = $f->deadlock();
    echo $deadlocks->sn->asXml() . "\n";
    echo $deadlocks->pt->asXml() . "\n";

    // Show bound formulas:
    $all_places = $this->sn_model ? $equivalent_elements->cplaces : $equivalent_elements->uplaces;
    shuffle($all_places);
    $selected = array_rand($all_places, rand(1, count($all_places)));
    $selection = array();
    foreach ($selected as $key)
    {
      $selection[$key] = $all_places[$key];
    }
    $bounds = $f->bound($selection);
    echo $bounds->sn->asXml() . "\n";
    echo $bounds->pt->asXml() . "\n";

    // Show cardinility formula:
    $cardinalities = $f->cardinality($selection);
    echo $cardinalities->sn->asXml() . "\n";
    echo $cardinalities->pt->asXml() . "\n";

  }

}
