<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class FormulasToText extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:to-text')
      ->setDescription('Convert formulas to text')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $sn_input = null;
  private $pt_input = null;
  private $sn_output = null;
  private $pt_output = null;
  private $ep;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $sn_path = dirname($this->sn_file);
    $pt_path = dirname($this->pt_file);
    $this->sn_input  = "${sn_path}/{$input->getOption('output')}.xml";
    $this->sn_output = "${sn_path}/{$input->getOption('output')}.txt";
    $this->pt_input  = "${pt_path}/{$input->getOption('output')}.xml";
    $this->pt_output = "${pt_path}/{$input->getOption('output')}.txt";
  }

  protected function perform()
  {
    $this->ep = new EquivalentElements($this->sn_model, $this->pt_model);
    if ($this->sn_model != null)
    {
      $this->convert(
        $this->sn_input,
        $this->sn_output,
        $this->ep->cplaces,
        $this->ep->ctransitions
      );
    }
    if ($this->pt_model != null)
    {
      $this->convert(
        $this->pt_input,
        $this->pt_output,
        $this->ep->uplaces,
        $this->ep->utransitions
      );
    }
  }

  private function convert ($input, $output, $places, $transitions)
  {
    if (file_exists($output))
      unlink($output);
    if (! file_exists($input))
    {
      $this->console_output->writeln(
        "<error>Formula file {$input} not found.</error>"
      );
      return;
    }
    $xml = $this->load_xml(file_get_contents($input));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    $result = array();
    foreach ($xml->property as $property)
    {
      $result[] = $this->translate_property($property, $places, $transitions);
      $this->progress->advance();
    }
    $this->progress->finish();
    $result = implode("\n", $result);
    file_put_contents($output, $result . "\n");
  }

  private function translate_property($property, $places, $transitions)
  {
    $result = null;
    $id = (string) $property->id;
    $description = (string) $property->description;
    $tags = array();
    if ((string) $property->tags->is_structural)
      $tags[] = 'structural';
    if ((string) $property->tags->is_reachability)
      $tags[] = 'reachability';
    if ((string) $property->tags->is_ctl)
      $tags[] = 'ctl';
    if ((string) $property->tags->is_ltl)
      $tags[] = 'ltl';
    $tags = implode(', ', $tags);
    $formula = $this->translate_formula(
      $property->formula->children()[0],
      $places,
      $transitions
    );
    if ($property->{'expected-result'})
    {
      $expected = (string) $property->{'expected-result'}->value;
      $explanation = (string) $property->{'expected-result'}->explanation;
      $result = <<<EOS
Property {$id}
  "{$description}"
  [{$tags}]
  expects {$expected} because "{$explanation}" is:
    {$formula}
  end.
EOS;
    }
    else {
      $result = <<<EOS
Property {$id}
  "{$description}"
  [{$tags}] is:
    {$formula}
  end.
EOS;
    }
    return $result;
  }

  private function translate_formula($formula, $places, $transitions, $in_nary = false)
  {
    $result = null;
    switch ((string) $formula->getName())
    {
    case 'invariant':
      $sub = $formula->children()[0];
      $result = 'I ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'impossibility':
      $sub = $formula->children()[0];
      $result = 'N ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'possibility':
      $sub = $formula->children()[0];
      $result = 'P ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'all-paths':
      $sub = $formula->children()[0];
      $result = 'A ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'exists-path':
      $sub = $formula->children()[0];
      $result = 'E ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'next':
      $succ_ = (string) $formula->{'if-no-successor'};
      $succ = $succ_ == 'true' ? '' : '~';
      $steps_ = (string) $formula->steps;
      $steps = $steps_ == '1' ? '' : $steps_;
      $sub = null;
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          $sub = $child;
        }
      }
      $result = "X{$steps}{$succ} " .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'globally':
      $sub = $formula->children()[0];
      $result = 'G ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'finally':
      $sub = $formula->children()[0];
      $result = 'F ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $strength = (string) $formula->strength;
      $operator = null;
      if ($strength == 'weak')
      {
        $operator = 'W';
      } else if ($strength == 'strong')
      {
        $operator = 'U';
      }
      $result = "(" .
        $this->translate_formula($before, $places, $transitions, true) .
        " {$operator} " .
        $this->translate_formula($reach, $places, $transitions, true) .
        ')';
      break;
    case 'deadlock':
      $result = 'deadlock';
      break;
    case 'is-live':
      $level = (string) $formula->level;
      $names = array();
      foreach ($formula->transition as $transition)
      {
        $names[] = $transitions[(string) $transition]->name;
      }
      $name = null;
      if (count($names) == 1)
      {
        $name = $names[0];
      }
      else
      {
        $name = '(' . implode(', ', $names) . ')';
      }
      $result = "\"{$name}\"?{$level}";
      break;
    case 'is-fireable':
      $transition = (string) $formula->transition;
      foreach ($formula->transition as $transition)
      {
        $names[] = $transitions[(string) $transition]->name;
      }
      $name = null;
      if (count($names) == 1)
      {
        $name = $names[0];
      }
      else
      {
        $name = '(' . implode(', ', $names) . ')';
      }
      $result = "\"{$name}\"?";
      break;
    case 'true':
      $result = 'true';
      break;
    case 'false':
      $result = 'false';
      break;
    case 'negation':
      $sub = $formula->children()[0];
      $result = '! ' .
        $this->translate_formula($sub, $places, $transitions);
      break;
    case 'conjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' & ', $res) . ')';
      break;
    case 'disjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' | ', $res) . ')';
      break;
    case 'exclusive-disjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' xor ', $res) . ')';
      break;
    case 'implication':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' => ', $res) . ')';
      break;
    case 'equivalence':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' <=> ', $res) . ')';
      break;
    case 'integer-eq':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' = ', $res) . ')';
      break;
    case 'integer-ne':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' != ', $res) . ')';
      break;
    case 'integer-lt':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' < ', $res) . ')';
      break;
    case 'integer-le':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' <= ', $res) . ')';
      break;
    case 'integer-gt':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' > ', $res) . ')';
      break;
    case 'integer-ge':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' >= ', $res) . ')';
      break;
    case 'integer-constant':
      $value = (string) $formula;
      $result = $value;
      break;
    case 'integer-sum':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' + ', $res) . ')';
      break;
    case 'integer-product':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' * ', $res) .')';
      break;
    case 'integer-difference':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' - ', $res) . ')';
      break;
    case 'integer-division':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub, $places, $transitions, true);
      }
      $result = '(' . implode(' / ', $res) . ')';
      break;
    case 'place-bound':
      $res = array();
      foreach ($formula->place as $place)
      {
        $id = (string) $place;
        $name = $places[$id]->name;
        $res[] = '"' . $name . '"';
      }
      $result = 'bound(' . implode(', ', $res) . ')';
      break;
    case 'tokens-count':
      $res = array();
      foreach ($formula->place as $place)
      {
        $id = (string) $place;
        $name = $places[$id]->name;
        $res[] = '"' . $name . '"';
      }
      $result = '#tokens(' . implode(', ', $res) . ')';
      break;
    default:
      $this->console_output->writeln(
        "<warning>Error: unknown node {$formula->getName()}</warning>"
      );
    }
    if ($in_nary)
    {
      $result = "({$result})";
    }
    return $result;
  }

}
