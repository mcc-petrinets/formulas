<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class UnfoldFormulas extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:unfold')
      ->setDescription('Unfold formulas')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $input = null;
  private $output = null;
  private $ep;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    if (($this->sn_model == null) || ($this->pt_model == null))
    {
      return;
    }
    $input_path = dirname($this->sn_file);
    $output_path = dirname($this->pt_file);
    $this->input = "${input_path}/{$input->getOption('output')}.xml";
    $this->output = "${output_path}/{$input->getOption('output')}.xml";
  }

  protected function perform()
  {
    if (($this->sn_model == null) || ($this->pt_model == null))
    {
      return;
    }
    $this->ep = new EquivalentElements($this->sn_model, $this->pt_model);
    if (file_exists($this->output))
      unlink($this->output);
    $xml = $this->load_xml(file_get_contents($this->input));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    foreach ($xml->property as $property)
    {
      $this->translate_formula($property->formula->children()[0]);
      $this->progress->advance();
    }
    $this->progress->finish();
    file_put_contents($this->output, $this->save_xml($xml));
  }

  private function translate_formula($formula)
  {
    switch ((string) $formula->getName())
    {
    case 'invariant':
    case 'impossibility':
    case 'possibility':
    case 'all-paths':
    case 'exists-path':
    case 'globally':
    case 'finally':
    case 'negation':
      $sub = $formula->children()[0];
      $this->translate_formula($sub);
      return;
    case 'next':
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          $this->translate_formula($child);
        }
      }
      return;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $this->translate_formula($before);
      $this->translate_formula($reach);
      return;
    case 'deadlock':
    case 'true':
    case 'false':
    case 'integer-constant':
      return;
    case 'is-live':
    case 'is-fireable':
      $id = (string) $formula->transition;
      unset($formula->transition[0]);
      foreach ($this->ep->ctransitions[$id]->unfolded as $t)
      {
        $formula->addChild('transition', $t->id);
      }
      return;
    case 'conjunction':
    case 'disjunction':
    case 'exclusive-disjunction':
    case 'implication':
    case 'equivalence':
    case 'integer-eq':
    case 'integer-ne':
    case 'integer-lt':
    case 'integer-le':
    case 'integer-gt':
    case 'integer-ge':
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
      foreach ($formula->children() as $sub)
      {
        $this->translate_formula($sub);
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
      echo "Error: unknown node {$formula->getName()}\n";
    }
  }

}
