<?php
namespace MCC\Formula;

error_reporting(-1);

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Formula\EquivalentElements;

class FormulaUnfolder
{
  protected $color_model;
  protected $pt_model;
  private $ep;

  public function __construct ($color_model, $pt_model)
  {
    #echo "mcc: formula-unfolder: constructing formula unfolder\n";
    $this->color_model = $color_model;
    $this->pt_model = $pt_model;
    $this->ep = new EquivalentElements($color_model, $pt_model);
  }

  public function unfold ($formula)
  {
    #echo "mcc: formula-unfolder: unfold: tag " . $formula->getName () . "\n";
    switch ((string) $formula->getName())
    {
    case 'all-paths':
    case 'exists-path':
    case 'globally':
    case 'finally':
    case 'next':
    case 'negation':
    case 'conjunction':
    case 'disjunction':
    case 'integer-le':
    case 'integer-sum':
    case 'integer-difference':
      foreach ($formula->children() as $sub)
      {
        $this->unfold($sub);
      }
      return;

    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $this->unfold($before);
      $this->unfold($reach);
      return;

    case 'deadlock':
    case 'integer-constant':
    case 'true':
    case 'false':
      return;

    case 'is-fireable':
		assert (count ($formula->children ()) == 1);
      $id = (string) $formula->transition;
      unset($formula->transition[0]);
      foreach ($this->ep->ctransitions[$id]->unfolded as $t)
      {
        $formula->addChild('transition', $t->id);
      }
      return;
    case 'place-bound':
    case 'tokens-count':
      $ids = array();
      foreach ($formula->place as $place)
      {
        $ids[] = (string) $place;
      }
      unset($formula->place);
      foreach ($ids as $id)
      {
        foreach ($this->ep->cplaces[$id]->unfolded as $p)
        {
          $formula->addChild('place', $p->id);
        }
      }
      return;

    default:
      $msg = "Unknown XML tag '{$formula->getName()}', internal error";
      throw new \Exception ($msg);
    }
  }
}

