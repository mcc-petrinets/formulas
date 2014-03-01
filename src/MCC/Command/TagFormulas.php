<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class TagFormulas extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:tag')
      ->setDescription('Tag formulas')
      ->addOption('no-warning', null,
        InputOption::VALUE_NONE,
        'Do not emit warning on missing files')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $warn = true;
  private $output;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->output = "{$input->getOption('output')}.xml";
    $this->warn = $input->getOption('no-warning');
  }

  protected function perform()
  {
    if ($this->sn_model != null)
    {
      $file = dirname($this->sn_file) . '/' . $this->output;
      $this->convert($file);
    }
    if ($this->pt_model != null)
    {
      $file = dirname($this->pt_file) . '/' . $this->output;
      $this->convert($file);
    }
  }

  private function convert ($file)
  {
    if (! file_exists($file))
    {
      if ($this->warn)
      {
        $this->console_output->writeln(
            "  <warning>Formula file '{$file}' does not exist.</warning>"
          );
      }
      return;
    }
    $xml = $this->load_xml(file_get_contents($file));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    foreach ($xml->property as $property)
    {
      $this->tag_property($property);
      $this->progress->advance();
    }
    $this->progress->finish();
    file_put_contents($file, $this->save_xml($xml));
  }

  private function boolean_of ($x)
  {
    if ($x)
      return "true";
    else
      return "false";
  }

  private function tag_property($property)
  {
    $property->tags->{'is-structural'} =
      $this->boolean_of(
        $this->is_structural($property->formula->children()[0], true)
      );
    $property->tags->{'is-reachability'} =
      $this->boolean_of(
        $this->is_reachability($property->formula->children()[0], true)
      );
    $property->tags->{'is-ctl'} =
      $this->boolean_of(
        $this->is_ctl($property->formula->children()[0], false)
      );
    $property->tags->{'is-ltl'} =
      $this->boolean_of(
        $this->is_ltl($property->formula->children()[0], true)
      );
  }

  private function is_structural($formula, $first)
  {
    switch ((string) $formula->getName())
    {
    case 'invariant':
    case 'impossibility':
    case 'possibility':
    case 'all-paths':
    case 'globally':
    case 'exists-path':
      $sub = $formula->children()[0];
      return $first &&
        $this->is_structural($sub, false);
    case 'next':
    case 'finally':
    case 'until':
      return false;
    case 'deadlock':
      return false;
    case 'is-live':
      return true;
    case 'is-fireable':
      return false;
    case 'true':
    case 'false':
      return true;
    case 'negation':
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
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_structural($sub, $first);;
      }
      return $result;
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_structural($sub, false);
      }
      return $result;
    case 'integer-constant':
      return true;
    case 'place-bound':
      return true;
    case 'tokens-count':
      return true;
    default:
      $this->console_output->writeln(
        "<error>Error: unknown node {$formula->getName()}</error>"
      );
    }
  }

  private function is_reachability($formula, $first)
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
      $sub = $formula->children()[0];
      return $first &&
        $this->is_reachability($sub, false);
    case 'next':
    case 'until':
      return false;
    case 'deadlock':
      return true;
    case 'is-live':
      return true;
    case 'is-fireable':
      return true;
    case 'true':
    case 'false':
      return true;
    case 'negation':
      $sub = $formula->children()[0];
      return $this->is_reachability($sub, $first);
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
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_reachability($sub, $first);
      }
      return $result;
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_reachability($sub, false);
      }
      return $result;
    case 'integer-constant':
      return true;
    case 'place-bound':
      return true;
    case 'tokens-count':
      return true;
    default:
      $this->console_output->writeln(
        "<error>Error: unknown node {$formula->getName()}</error>"
      );
    }
  }

  private function is_ctl($formula, $in_ctl)
  {
    switch ((string) $formula->getName())
    {
    case 'invariant':
    case 'impossibility':
    case 'possibility':
      $sub = $formula->children()[0];
      return $this->is_ltl($sub, false);
    case 'all-paths':
    case 'exists-path':
      $sub = $formula->children()[0];
      $subname = $sub->getName();
      if (($subname == 'globally') ||
        ($subname == 'finally') ||
        ($subname == 'next') ||
        ($subname == 'until'))
        return $this->is_ctl($sub, true);
    case 'globally':
    case 'finally':
    case 'next':
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          return $in_ctl && $this->is_ctl($child, false);
        }
      }
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      return $in_ctl &&
        $this->is_ctl($before, false) &&
        $this->is_ctl($reach , false);
    case 'deadlock':
      return true;
    case 'is-live':
      return false;
    case 'is-fireable':
      return true;
    case 'true':
    case 'false':
      return true;
    case 'negation':
      $sub = $formula->children()[0];
      return $this->is_ctl($sub, false);
    case 'conjunction':
    case 'disjunction':
    case 'exclusive-disjunction':
    case 'implication':
    case 'equivalence':
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
    case 'integer-eq':
    case 'integer-ne':
    case 'integer-lt':
    case 'integer-le':
    case 'integer-gt':
    case 'integer-ge':
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_ctl($sub, false);;
      }
      return $result;
    case 'integer-constant':
      return true;
    case 'place-bound':
      return false;
    case 'tokens-count':
      return true;
    default:
      $this->console_output->writeln(
        "<error>Error: unknown node {$formula->getName()}</error>"
      );
    }
  }

  private function is_ltl($formula, $first)
  {
    switch ((string) $formula->getName())
    {
    case 'invariant':
    case 'impossibility':
    case 'possibility':
      $sub = $formula->children()[0];
      return $first &&
        $this->is_ltl($sub, false);
    case 'all-paths':
    case 'exists-path':
      $sub = $formula->children()[0];
      return $first &&
        $this->is_ltl($sub, false);
    case 'globally':
    case 'finally':
    case 'next':
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          return $this->is_ltl($child, false);
        }
      }
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      return
        $this->is_ltl($before, false) &&
        $this->is_ltl($reach , false);
    case 'deadlock':
      return true;
    case 'is-live':
      return false;
    case 'is-fireable':
      return true;
    case 'true':
    case 'false':
      return true;
    case 'negation':
      $sub = $formula->children()[0];
      return $this->is_ltl($sub, $first);
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
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_ltl($sub, $first);
      }
      return $result;
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
      $result = true;
      foreach ($formula->children() as $sub)
      {
        $result = $result &&
          $this->is_ltl($sub, false);
      }
      return $result;
    case 'integer-constant':
      return true;
    case 'place-bound':
      return false;
    case 'tokens-count':
      return true;
    default:
      $this->console_output->writeln(
        "<error>Error: unknown node {$formula->getName()}</error>"
      );
    }
  }

}
