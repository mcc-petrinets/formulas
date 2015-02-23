<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\ArrayInput;
use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

const INT_MAX                                 = 9999999;

const SUBCAT_REACHABILITY_DEADLOCK            = "ReachabilityDeadlock";
const SUBCAT_REACHABILITY_FIREABILITY_SIMPLE  = "ReachabilityFireabilitySimple";
const SUBCAT_REACHABILITY_FIREABILITY         = "ReachabilityFireability";
const SUBCAT_REACHABILITY_CARDINALITY         = "ReachabilityCardinality";
const SUBCAT_REACHABILITY_BOUNDS              = "ReachabilityBounds";
const SUBCAT_REACHABILITY_COMPUTE_BOUNDS      = "ReachabilityComputeBounds";

const SUBCAT_LTL_FIREABILITY_SIMPLE           = "LTLFireabilitySimple";
const SUBCAT_LTL_FIREABILITY                  = "LTLFireability";
const SUBCAT_LTL_CARDINALITY                  = "LTLCardinality";

const SUBCAT_CTL_FIREABILITY_SIMPLE           = "CTLFireabilitySimple";
const SUBCAT_CTL_FIREABILITY                  = "CTLFireability";
const SUBCAT_CTL_CARDINALITY                  = "CTLCardinality";

class GenerateFormulas extends Base
{

  protected function configure()
  {
    parent::configure();
    $this->setName('formula:generate')
      ->setDescription('Generate formulas')
      ->addOption('chain', null,
        InputOption::VALUE_NONE,
        'Chain with unfolding and conversion to text')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
      ->addOption('quantity', null,
        InputOption::VALUE_REQUIRED,
        'Quantity of properties to generate (at most)', 5)
      ->addOption('subcategory', null,
        InputOption::VALUE_REQUIRED,
        'The competition subcategory for which we will generate formulas', 'ReachabilityDeadlock')
      ->addOption('depth', null,
        InputOption::VALUE_REQUIRED,
        'Unfold the grammar of the specified subcategory up to this depth and generate a random formula from this unfolded grammar (at most)', 3)
      ;
  }

  private $model;
  private $reference_model;
  public  $places;
  public  $transitions;
  private $id;
  private $output_name;
  private $output;
  private $subcategory;
  private $quantity;
  private $depth;
  private $property_xml_template = <<<EOT
  <property>
    <id></id>
    <description>Automatically generated formula.</description>
    <formula></formula>
  </property>
EOT;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $path = NULL;
    if ($this->sn_model)
      $path = dirname($this->sn_file);
    else
      $path = dirname($this->pt_file);
    $this->output_name = $input->getOption('output');
    $this->output      = "${path}/{$this->output_name}.xml";
    $this->chain       = $input->getOption('chain');
    $this->subcategory = $input->getOption('subcategory');
    $this->quantity    = intval($input->getOption('quantity'));
    $this->depth       = intval($input->getOption('depth'));
    $this->id = 1;
    $this->reference_model = new EquivalentElements($this->sn_model, $this->pt_model);
    $this->places = $this->sn_model
      ? $this->reference_model->cplaces
      : $this->reference_model->uplaces;
    $this->transitions = $this->sn_model
      ? $this->reference_model->ctransitions
      : $this->reference_model->utransitions;
    if ($this->sn_model)
      $this->model = $this->sn_model;
    else
      $this->model = $this->pt_model;
  }

  protected function perform()
  {
    $this->test1 ();
    return;

    if (file_exists($this->output))
    {
      unlink($this->output);
    }
    $this->progress->setRedrawFrequency(max(1, $this->quantity / 100));
    $this->progress->start($this->console_output, $this->quantity);
    $result = array();

    // produce $this->quantity formulas
    for ($i = 0; $i < $this->quantity; $i++)
    {
      $g = $this->build_grammar (SUBCAT_REACHABILITY_DEADLOCK);
      return;
      $g->static_analysis ();
      var_dump ($g);
      echo "$g\n";

      $result[] = null;
      $this->progress->advance();
    }

    // save all formulas into one xml file
    $xml_tree = $this->load_xml('<property-set xmlns="http://mcc.lip6.fr/"/>');
    foreach ($result as $formula)
    {
      echo "\nsave formula\n";
      $property = $this->load_xml($this->property_xml_template);
      var_dump ($property);
      $property->id = $this->model->net->attributes()['id'] .
        "-{$this->subcategory}-" . $this->id;
      $this->xml_adopt($property->formula, $formula);
      $this->xml_adopt($xml_tree, $property);
      $this->id++;
    }
    file_put_contents($this->output, $this->save_xml($xml_tree));
    $this->progress->finish();

    if ($this->chain)
    {
      foreach (array(
        'formula:tag',
        'formula:unfold',
        'formula:to-text'
      ) as $c)
      {
        $command = $this->getApplication()->find($c);
        $arguments = array(
            'command'   => $c,
            'root'      => $this->root,
            'model'     => $this->model_name,
            'parameter' => $this->parameter,
            '--output'  => $this->output_name,
          );
        if ($c == 'formula:tag')
        {
          $arguments[] = '--no-warning';
        }
        $input = new ArrayInput($arguments);
        //$returnCode = $command->run($input, $this->console_output);
      }
    }
  }

  private function copy($a)
  {
    $result = array();
    foreach($a as $k => $v)
    {
      $result[$k] = $v;
    }
    return $result;
  }

  private function build_grammar ($subcategory)
  {
    // terminal symbols used in any grammar
    $exists           = new Symbol ("E",                "<exists-path/>",  $this, true);
    $all              = new Symbol ("A",                "<all-paths/>",    $this, true);
    $globally         = new Symbol ("G",                "<globally/>",     $this, true);
    $finally          = new Symbol ("F",                "<finally/>",      $this, true);
    $next             = new Symbol ("X",                "<next/>",         $this, true);
    $until            = new Symbol ("U",                "<until/>",        $this, true);
    $until_before     = new Symbol ("U-before",         "<before/>",       $this, true);
    $until_reach      = new Symbol ("U-reach",          "<reach/>",        $this, true);
    $not              = new Symbol ("not",              "<negation/>",     $this, true);
    $and              = new Symbol ("and",              "<conjunction/>",  $this, true);
    $or               = new Symbol ("or",               "<disjunction/>",  $this, true);
    $deadlock         = new Symbol ("deadlock",         "<deadlock/>",     $this, true);
    $is_fireable      = new Symbol ("is-fireable",      null,              $this, true);
    $leq              = new Symbol ("<=",               "<integer-le/>",   $this, true);
    $integer_constant = new Symbol ("integer-constant", null,              $this, true);
    $place_bound      = new Symbol ("place-bound",      "<place-bound/>",  $this, true);
    $tokens_count     = new Symbol ("tokens-count",     "<tokens-count/>", $this, true);

    // non-terminal symbols used in any grammar
    $boolean_formula    = new Symbol ("boolean-formula");
    $integer_formula    = new Symbol ("integer-formula");
    $state_formula      = new Symbol ("state-formula");
    $atom               = new Symbol ("atom");
    $integer_expression = new Symbol ("integer-expression");
    $tmp1               = new Symbol ("tmp-formula1");
    $tmp2               = new Symbol ("tmp-formula2");

    switch ($subcategory)
    {
    case SUBCAT_REACHABILITY_DEADLOCK :
      $g = new Grammar ($boolean_formula);
      $g->add_rule (new Rule ($boolean_formula, array ($exists,  $tmp1)));
      $g->add_rule (new Rule ($tmp1,            array ($finally, $deadlock)));
      break;

    case SUBCAT_REACHABILITY_FIREABILITY_SIMPLE :
      $g = new Grammar ($boolean_formula);
      $g->add_rule (new Rule ($boolean_formula, array ($exists,   $tmp1)));
      $g->add_rule (new Rule ($boolean_formula, array ($all,      $tmp2)));
      $g->add_rule (new Rule ($tmp1,            array ($finally,  $is_fireable)));
      $g->add_rule (new Rule ($tmp2,            array ($globally, $is_fireable)));
      break;

    case SUBCAT_REACHABILITY_FIREABILITY :
      $g = new Grammar ($boolean_formula);
      $g->add_rule (new Rule ($boolean_formula, array ($exists,   $tmp1)));
      $g->add_rule (new Rule ($boolean_formula, array ($all,      $tmp2)));
      $g->add_rule (new Rule ($tmp1,            array ($finally,  $state_formula)));
      $g->add_rule (new Rule ($tmp2,            array ($globally, $state_formula)));

      $g->add_rule (new Rule ($state_formula,   array ($not,      $state_formula)));
      $g->add_rule (new Rule ($state_formula,   array ($and,      $state_formula, $state_formula)));
      $g->add_rule (new Rule ($state_formula,   array ($or,       $state_formula, $state_formula)));
      $g->add_rule (new Rule ($state_formula,   array ($atom)));
      $g->add_rule (new Rule ($atom,            array ($is_fireable)));
      break;

    default :
      $this->console_output->writeln(
          "<error>Error: internal error, unknown subcategory '$subcategory'</error>");
      return null;
    }

    $g->static_analysis ();
    return $g;
  }

  private function test1 ()
  {
    foreach (array (
        SUBCAT_REACHABILITY_DEADLOCK            ,
        SUBCAT_REACHABILITY_FIREABILITY_SIMPLE  ,
        SUBCAT_REACHABILITY_FIREABILITY         ,
        SUBCAT_REACHABILITY_CARDINALITY         ,
        SUBCAT_REACHABILITY_BOUNDS              ,
        SUBCAT_REACHABILITY_COMPUTE_BOUNDS      ,
        
        SUBCAT_LTL_FIREABILITY_SIMPLE           ,
        SUBCAT_LTL_FIREABILITY                  ,
        SUBCAT_LTL_CARDINALITY                  ,
        
        SUBCAT_CTL_FIREABILITY_SIMPLE           ,
        SUBCAT_CTL_FIREABILITY                  ,
        SUBCAT_CTL_CARDINALITY                  
        )
        as $cat)
    {
      echo "Category '$cat'\n";
      echo "==============================\n";
      $g = $this->build_grammar ($cat);
      echo "$g\n\n";
    }
  }

  private function test2 ()
  {
  }
}

class Symbol
{
  public $name;
  public $value1;
  public $value2;
  public $is_terminal;
  public $min_derivation_len;
  public $mark = 0;

  public function __construct ($name, $value1 = null, $value2 = null, $is_terminal = false)
  {
    // 'name' is to identify the symbol
    // 'value1' will be data needed to build the associated xml node
    // 'value2' will be a reference to the GenerateFormulas object

    $this->name = $name;
    $this->value1 = $value1;
    $this->value2 = $value2;
    $this->is_terminal = $is_terminal;
    $this->min_derivation_len = $is_terminal ? 0 : INT_MAX;
  }

  public function generate ()
  {
    switch ($this->name)
    {
    case "integer-constant" :
      $i = rand (1, 3);
      return $this->value2->load_xml("<integer-constant>$i</integer-constant>");

    case "is-fireable" :
      $selected = array_rand($this->value2->transitions, 1);
      if (! is_array($selected))
      {
        $selected = array($selected);
      }
      $xml = '<is-fireable/>';
      $xml_tree = $this->value2->load_xml($xml);
      foreach ($selected as $i) {
        $transition = $this->value2->transitions[$i];
        $xml_tree->addChild('transition', $transition->id);
      }
      return $xml_tree;

    case "place-bound" :
    case "tokens-count" :
      $selected = array_rand($this->value2->places, 1);
      if (! is_array($selected))
      {
        $selected = array($selected);
      }
      $xml = "<$this->name/>";
      $xml_tree = $this->value2->load_xml($xml);
      foreach ($selected as $i) {
        $place = $this->value2->places[$i];
        $xml_tree->addChild('place', $place->id);
      }
      return $xml_tree;

    default :
      return $this->value2->load_xml($this->value1);
    }
  }

  public function __toString ()
  {
    $s = "";
    if ($this->is_terminal)
      return "\"$this->name\"";
    else
      return "<$this->name>";
  }
}

class Rule
{
  public $head;
  public $body;
  public $min_derivation_len;
  public $mark = 0;

  public function __construct ($head, $body = array ())
  {
    $this->head = $head;
    $this->body = $body;
    $this->min_derivation_len = INT_MAX;
  }

  public function __toString ()
  {
    $s = "$this->head ::= \n";
    for ($i = 0; $i < count ($this->body); $i++)
    {
      $s .= "\t" . $this->body[$i] . "\n";
    }
    $s .= "\t# minimum derivation length: $this->min_derivation_len\n";
    return $s;
  }
}

class Grammar
{
  public $start_symbol;
  public $rules;
  public $mark = 1;
  public $static_analysis_done = false;

  public function __construct ($ss)
  {
    $this->start_symbol = $ss;
    $this->rules = array ();
  }

  public function add_rule ($rule)
  {
    $this->rules[] = $rule;
  }

  public function generate ($max_depth, $symbol = null)
  {
    if ($symbol == null) $symbol = $this->start_symbol;
    if ($symbol->min_derivation_len > $max_depth)
    {
      echo "error - minimum derivation from this symbol is deeper than authorized\n";
      return;
    }
    __generate ($symbol, $max_depth);
  }

  private function __generate ($symbol, $max_depth)
  {
    assert ($symbol->min_derivation_len <= $max_depth);
    if ($symbol->is_terminal) return $symbol->generate ();

    $choices_array = $this->find_matching_rules ($symbol, $max_depth);
    assert (count ($choices_array) >= 1);
    $index = array_rand ($choices_array);
    $rule = $choices_array[$index];

    assert (count($rule->body) >= 1);
    if (count($rule->body) == 1) return __generate ($rule->body[0]);
    assert ($rule->body[0]->is_terminal);
    $xml_tree = $rule->body[0]->generate ();

    for ($i = 1; $i < count ($rule->body); $i++)
    {
      $xml_subtree = __generate ($rule->body[$i], $max_depth - 1);
      $this->xml_adopt ($xml_tree, $xml_subtree);
    }
  }

  private function find_matching_rules ($symbol, $max_deriv_len = INT_MAX)
  {
    $result = array ();
    foreach ($this->rules as $rule)
    {
      if ($rule->head == $symbol && $rule->min_derivation_len <= $max_deriv_len)
      {
        $result[] = $rule;
      }
    }
    return $result;
  }

  public function static_analysis ()
  {
    if ($this->static_analysis_done) return;
    $this->static_analysis_done = true;
    $this->sa_compute_min_derivation_lengths ();
  }

  private function sa_compute_min_derivation_lengths ()
  {
    /* this function computes the minimum (concurrent, tree-wise) length of
    a derivation to a string of terminals for every symbol and rule in the
    grammar (default values are safe for symbols unreachable from the start
    symbol) */

    $len = 0;
    $changed = true;
    $visited = $this->new_mark ();

    while ($changed)
    {
      $changed = false;
      foreach ($this->rules as $rule)
      {
        if ($rule->mark == $visited) continue;
        $want_it = true;
        foreach ($rule->body as $symbol)
        {
          if ($symbol->mark == $visited) continue;
          if ($symbol->is_terminal)
          {
            $symbol->mark = $visited;
            $symbol->min_derivation_len = 0;
            continue;
          }
          $want_it = false;
          break;
        }
        if (! $want_it) continue;

        $rule->mark = $visited;
        $maxi = -1;
        foreach ($rule->body as $symbol)
        {
          if ($symbol->min_derivation_len > $maxi)
            $maxi = $symbol->min_derivation_len;
        }
        $rule->min_derivation_len = $maxi;
        assert ($maxi == $len);
        if ($rule->head->mark != $visited)
        {
          $rule->head->mark = $visited;
          $rule->head->min_derivation_len = $len + 1;
          $changed = true;
        }
      }
      $len++;
    }
  }

  private function health_check ()
  {
    static_analysis ();

    // the grammar generates at least one string of terminals
    if ($this->start_symbol->min_derivation_len == INT_MAX)
    {
      echo "warning - this grammar do not generate any string!\n";
    }

    // every rule can be used in at least one terminal derivation
    $visited = new_mark ();
    $this->sa_mark_reachable_symbols ($this->start_symbol, $visited);
    $dead_rules = sa_find_dead_rules ($visited);
    if (count ($dead_rules))
    {
      echo "warning - this grammar contains dead rules !\n";
    }

    // the first symbol in every rule's body needs to be a terminal
    // if there is more than one symbol in the body
    foreach ($this->rules as $rule)
    {
      if (count ($rule->body) == 0)
      {
        echo "error - there is rules with an empty body\n";
        continue;
      }
      if (count($rule->body) > 1 && ! $rule->body[0]->is_terminal)
      {
        echo "error - the first symbol in the body of a rule needs to be a terminal\n";
      }
    }
  }

  private function sa_find_dead_rules ($reachable_non_term)
  {
    // a rule is dead iff either the head is not reachable from the start
    // symbol or if the body contains at least one symbol with infinite
    // minimum derivation length

    $dead_rules = array ();
    foreach ($this->rules as $rule)
    {
      if ($rule->head->marked != $reachable_non_term
          || $rule->min_derivation_len == INT_MAX)
      {
        $dead_rules[] = $rule;
      }
    }
    return $dead_rules;
  }

  private function sa_mark_reachable_symbols ($symbol, $visited)
  {
    if ($symbol->mark == $visited) return;
    $symbol->mark = $visited;

    foreach (find_matching_rules ($symbol) as $rule)
    {
      assert ($symbol == $rule->head);
      foreach ($rule->body as $symbol2)
      {
        $this->sa_mark_reachable_symbols ($symbol2, $visited);
      }
    }
  }

  private function new_mark ()
  {
    return ++$this->mark;
  }

  public function __toString ()
  {
    $s = "Start symbol:\n\t$this->start_symbol\n";
    $s .= "Rules (" . count ($this->rules) . "):\n";
    for ($i = 0; $i < count ($this->rules); $i++)
    {
      $s .= $this->rules[$i] . "\n";
    }

    $s .= "Minimum derivation lenghts for non-terminals:\n";
    $mark = $this->new_mark ();
    for ($i = 0; $i < count ($this->rules); $i++)
    {
      if ($this->rules[$i]->head->mark == $mark) continue;
      $this->rules[$i]->head->mark = $mark;
      $s .= sprintf ("\t%-8d %s\n",
          $this->rules[$i]->head->min_derivation_len,
          $this->rules[$i]->head);
    }
    return $s;
  }
}

// vim:et ts=2:

