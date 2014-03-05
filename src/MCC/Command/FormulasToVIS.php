<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class FormulasToVIS extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:to-vis')
      ->setDescription('Convert formulas to VIS format')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $pt_input = null;
  private $pt_output = null;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $pt_path = dirname($this->pt_file);
    $this->pt_input  = "${pt_path}/{$input->getOption('output')}.xml";
    $this->pt_output = "${pt_path}/{$input->getOption('output')}.txt";
  }

  protected function perform()
  {
    if ($this->pt_model != null)
    {
      $this->convert(
        $this->pt_input,
        $this->pt_output
      );
    }
  }

  private function convert ($input, $output)
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
    $errors = array();
    foreach ($xml->property as $property)
    {
      try {
        $result[] = $this->translate_property($property);
      } catch (\Exception $e) {
        $errors[] = (string) $property->id . ": " . $e->getMessage();
      }
      $this->progress->advance();
    }
    $this->progress->finish();
    if (count($errors) != 0) {
      $message = "<warning>Some formulas could not be translated:";
      $message = $message . implode("; ", $errors);
      $message = $message . "</warning>";
      $this->console_output->writeln($message);
    }
    $result = implode("\n", $result);
    file_put_contents($output, $result . "\n");
  }

  private function translate_property($property)
  {
    $id = (string) $property->id;
    $description = (string) $property->description;
    $result = "# {$id}\n";
    $result = $result . "# {$description}\n";
    $formula = $this->translate_formula(
      $property->formula->children()[0],
      true
    );
    return $result;
  }

  private function translate_formula($formula, $first = false)
  {
    $result = null;
    switch ((string) $formula->getName())
    {
    case 'invariant':
      $sub = $formula->children()[0];
      $result = 'AG (' .
        $this->translate_formula($sub) .
        ')';
      break;
    case 'impossibility':
      $sub = $formula->children()[0];
      $result = '! EF (' .
        $this->translate_formula($sub) .
        ')';
      break;
    case 'possibility':
      $sub = $formula->children()[0];
      $result = 'EF (' .
        $this->translate_formula($sub) .
        ')';
      break;
    case 'all-paths':
      $sub = $formula->children()[0];
      $result = 'A' .
        $this->translate_formula($sub);
      break;
    case 'exists-path':
      $sub = $formula->children()[0];
      $result = 'E' .
        $this->translate_formula($sub);
      break;
    case 'next':
      $succ_ = (string) $formula->{'if-no-successor'};
      if ($succ == 'true') {
        throw new \Exception("X~");
      }
      $steps_ = (string) $formula->steps;
      $sub = null;
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          $sub = $child;
        }
      }
      $result = "X:{$steps}(" .
        $this->translate_formula($sub) .
        ')';
      if ($first) {
        $result = "A{$result}";
      }
      break;
    case 'globally':
      $sub = $formula->children()[0];
      $result = 'G (' .
        $this->translate_formula($sub) .
        ')';
      if ($first) {
        $result = "A{$result}";
      }
      break;
    case 'finally':
      $sub = $formula->children()[0];
      $result = 'F (' .
        $this->translate_formula($sub) .
        ')';
      if ($first) {
        $result = "A{$result}";
      }
      break;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $strength = (string) $formula->strength;
      $operator = null;
      if ($strength == 'weak')
      {
        throw new \Exception("W");
      } else if ($strength == 'strong')
      {
        $operator = 'U';
      }
      $result = "((" .
        $this->translate_formula($befores) .
        ") {$operator} (" .
        $this->translate_formula($reachs) .
        '))';
      if ($first) {
        $result = "A{$result}";
      }
      break;
    case 'deadlock':
      $result = '! AX(TRUE)';
      break;
    case 'is-live':
      throw new \Exception("t?lx");
      break;
    case 'is-fireable':
      throw new \Exception("t?");
      break;
    case 'true':
      $result = 'TRUE';
      break;
    case 'false':
      $result = 'FALSE';
      break;
    case 'negation':
      $sub = $formula->children()[0];
      $result = '! (' .
        $this->translate_formula($sub) .
        ')';
      break;
    case 'conjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' * ', $res) . ')';
      break;
    case 'disjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' + ', $res) . ')';
      break;
    case 'exclusive-disjunction':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' ^ ', $res) . ')';
      break;
    case 'implication':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' -> ', $res) . ')';
      break;
    case 'equivalence':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' <-> ', $res) . ')';
      break;
    case 'integer-eq':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '(' . implode(' == ', $res) . ')';
      $result = $this->wrap_ag($formula->children(), $result);
      break;
    case 'integer-ne':
      $res = array();
      foreach ($formula->children() as $sub)
      {
        $res[] = $this->translate_formula($sub);
      }
      $result = '! (' . implode(' == ', $res) . ')';
      $result = $this->wrap_ag($formula->children(), $result);
     break;
    case 'integer-lt':
      $lhs = $formula->children()[0];
      $rhs = $formula->children()[1];
      if ($lhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<integer-ge/>');
        $this->xml_adopt($x, $rhs);
        $this->xml_adopt($x, $lhs);
        $result = $this->translate_formula($x);
      } elseif ($rhs->getName() == 'integer-constant') {
        $n = intval((string) $rhs->children()[0]);
        $result = 'TRUE';
        for ($i=0; $i < $n; $i++) {
          $result = $result . ' + (' .
            $this->translate_formula($lhs) .
            " == {$i} )";
        }
      } else {
        throw new \Exception("<");
      }
      $result = $this->wrap_ag($formula->children(), $result);
      break;
    case 'integer-le':
      $lhs = $formula->children()[0];
      $rhs = $formula->children()[1];
      if ($lhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<integer-gt/>');
        $this->xml_adopt($x, $rhs);
        $this->xml_adopt($x, $lhs);
        $result = $this->translate_formula($x);
      } elseif ($rhs->getName() == 'integer-constant') {
        $n = intval((string) $rhs->children()[0]);
        $result = 'TRUE';
        for ($i=0; $i <= $n; $i++) {
          $result = $result . ' + (' .
            $this->translate_formula($lhs) .
            " == {$i} )";
        }
      } else {
        throw new \Exception("<=");
      }
      $result = $this->wrap_ag($formula->children(), $result);
      break;
    case 'integer-gt':
      $lhs = $formula->children()[0];
      $rhs = $formula->children()[1];
      if ($lhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<integer-le/>');
        $this->xml_adopt($x, $rhs);
        $this->xml_adopt($x, $lhs);
        $result = $this->translate_formula($x);
      } elseif ($rhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<negation/>');
        $y = $this->load_xml('<integer-le/>');
        $this->xml_adopt($y, $lhs);
        $this->xml_adopt($y, $rhs);
        $this->xml_adopt($x, $y);
        $result = $this->translate_formula($x);
      } else {
        throw new \Exception(">");
      }
      $result = $this->wrap_ag($formula->children(), $result);
      break;
    case 'integer-ge':
      $lhs = $formula->children()[0];
      $rhs = $formula->children()[1];
      if ($lhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<integer-lt/>');
        $this->xml_adopt($x, $rhs);
        $this->xml_adopt($x, $lhs);
        $result = $this->translate_formula($x);
      } elseif ($rhs->getName() == 'integer-constant') {
        $x = $this->load_xml('<negation/>');
        $y = $this->load_xml('<integer-lt/>');
        $this->xml_adopt($y, $lhs);
        $this->xml_adopt($y, $rhs);
        $this->xml_adopt($x, $y);
        $result = $this->translate_formula($x);
      } else {
        throw new \Exception(">=");
      }
      $result = $this->wrap_ag($formula->children(), $result);
      break;
    case 'integer-constant':
      $result = (string) $formula->children()[0];
      break;
    case 'integer-sum':
      throw new \Exception("+");
      break;
    case 'integer-product':
      throw new \Exception("*");
      break;
    case 'integer-difference':
      throw new \Exception("-");
      break;
    case 'integer-division':
      throw new \Exception("/");
      break;
    case 'place-bound':
      $places = $formula->place;
      if (count($places) > 1) {
        throw new \Exception("bound p1 .. pn");
      } else {
        $result = (string) $place->children()[0];
      }
      break;
    case 'tokens-count':
      $places = $formula->place;
      if (count($places) > 1) {
        throw new \Exception("#tokens p1 .. pn");
      } else {
        $result = (string) $place->children()[0];
      }
      break;
    default:
      $this->console_output->writeln(
        "<warning>Error: unknown node {$formula->getName()}</warning>"
      );
    }
    return $result;
  }

  private function wrap_ag ($subs, $result)
  {
    $is_bound = false;
    $is_tokens = false;
    foreach ($subs as $sub) {
      if ($sub->getName() == 'place-bound') {
        $is_bound = true;
      } elseif ($sub->getName() == 'tokens-count') {
        $is_tokens = true;
      } else {
        throw new \Exception($sub->getName());
      }
    }
    if ($is_bound && $is_tokens) {
      throw new \Exception("#tokens && bound");
    } elseif ($is_bound) {
      return "AG ({$result})";
    } elseif ($is_token) {
      return $result;
    } else {
      throw new \Exception("!#tokens && !bound");
    }
  }

}
