<?php
namespace MCC\Command;

use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\HttpKernel\Log\LoggerInterface;

class Domain
{
  public $id;
  public $name;
  public $values = array();
}

class Place
{
  public $id;
  public $name;
  public $domain = NULL;
  public $unfolded = array();
}

class Places extends Command
{

  protected function configure()
  {
    parent::configure();
    $this
      ->setName('places')
      ->setDescription('Compute unfolded places')
      ->addArgument('colored-model', InputArgument::REQUIRED, 'Colored model file (in PNML)')
      ->addArgument('unfolded-model', InputArgument::REQUIRED, 'Unfolded model file (in PNML)');
  }

  protected function execute(InputInterface $input, OutputInterface $output)
  {
    $cmodelfile = $input->getArgument('colored-model');
    $umodelfile = $input->getArgument('unfolded-model');
    $domains = array();
    // TODO check that files exist.
    $cmodel = simplexml_load_file($cmodelfile, NULL, LIBXML_COMPACT);
    $umodel = simplexml_load_file($umodelfile, NULL, LIBXML_COMPACT);
    // Load colored domains:
    foreach ($cmodel->net->declaration->structure->declarations->namedsort as $sort)
    {
      $id = (string) $sort->attributes()['id'];
      $name = (string) $sort->attributes()['name'];
      $domain = new Domain();
      $domain->id = $id;
      $domain->name = $name;
      foreach ($sort->cyclicenumeration->feconstant as $value)
      {
        $vid = $value->attributes()['id'];
        $vname = $value->attributes()['name'];
        $domain->values[(string) $vid] = (string) $vname;
      }
      $domains[$id] = $domain;
    }
    // Load colored domains and places:
    $cplaces = array();
    foreach ($cmodel->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      $domain = (string) $place->type->structure->usersort->attributes()['declaration'];
      $cplace = new Place();
      $cplace->id = $id;
      $cplace->name = $name;
      $cplace->domain = $domains[$domain];
      $cplaces[$id] = $cplace;
    }
    // Load unfolded places:
    $uplaces = array();
    foreach ($umodel->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      $uplace = new Place();
      $uplace->id = $id;
      $uplace->name = $name;
      $uplaces[$id] = $uplace;
    }
    print_r($domains);
    print_r($cplaces);
    print_r($uplaces);
    // For each colored place, build regex that recognize its unfoldings:
    foreach ($cplaces as $place)
    {
      $regex = '^' . $place->name;
      $p = '(';
      $first = true;
      foreach ($place->domain->values as $vid => $vname)
      {
        if ($first)
        {
          $first = false;
        } else
        {
          $p .= '|';
        }
        $p .= '_' . $vname;
      }
      $p .= ')';
      $regex = $regex . $p . '$';
      echo $regex . "\n";
      foreach ($uplaces as $uplace)
      {
        if (ereg($regex, $uplace->name))
        {
          $place->unfolded[$uplace->id] = $uplace;
        }
      }
    }
    print_r($cplaces);
  }

}
