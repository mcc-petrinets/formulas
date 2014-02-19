<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class Formula {

  public $description = "Automatically generated.\n";
  public $is_structural = false;
  public $is_ctl = false;
  public $is_ltl = false;
  public $is_reachability = false;
  public $sn;
  public $pt;

}

const INTEGER_FORMULA       = 'integer';
const BOOLEAN_FORMULA       = 'boolean';

const BOUND_OPERATOR        = 'bound';
const CARDINALITY_OPERATOR  = 'cardinality';
const DEADLOCK_OPERATOR     = 'deadlock';
const FIREABILITY_OPERATOR  = 'fireability';
const LIVENESS_OPERATOR     = 'liveness';
const INTEGER_OPERATOR      = 'integer';
const BOOLEAN_OPERATOR      = 'boolean';
const CTL_OPERATOR          = 'ctl';
const LTL_OPERATOR          = 'ltl';
const REACHABILITY_OPERATOR = 'breachability';
const INTEGER_COMPARISON    = 'comparison';

const BOOLEAN_CONSTANT      = 'constant';
const INTEGER_CONSTANT      = 'constant';

class GenerateAtomicPropositions extends Base
{

  protected function configure()
  {
    parent::configure();
    $this->setName('generate')
      ->setDescription('Generates atomic propositions')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
      ->addOption('type', null,
        InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY,
        'Type of results (boolean | integer)', array('boolean', 'integer'))
      ->addOption('prefix', null,
        InputOption::VALUE_REQUIRED,
        'Prefix for formula identifiers', 'formula')
      ->addOption('quantity', null,
        InputOption::VALUE_REQUIRED,
        'Quantity of properties to generate (at most)', 5)
      ->addOption('depth', null,
        InputOption::VALUE_REQUIRED,
        'Depth of properties to generate (at most)', 3)
      ->addOption('bound'       , null,
        InputOption::VALUE_NONE, 'Include bound operator')
      ->addOption('cardinality' , null,
        InputOption::VALUE_NONE, 'Include cardinality operator')
      ->addOption('deadlock'    , null,
        InputOption::VALUE_NONE, 'Include dealock operator')
      ->addOption('fireability' , null,
        InputOption::VALUE_NONE, 'Include fireability operator')
      ->addOption('liveness'    , null,
        InputOption::VALUE_NONE, 'Include liveness operator')
      ->addOption('boolean'     , null,
        InputOption::VALUE_NONE, 'Include boolean operators between subformulas')
      ->addOption('integer'     , null,
        InputOption::VALUE_NONE, 'Include integer operators between subformulas')
      ->addOption('ctl'         , null,
        InputOption::VALUE_NONE, 'Include CTL operators')
      ->addOption('ltl'         , null,
        InputOption::VALUE_NONE, 'Include LTL operators')
      ->addOption('reachability', null,
        InputOption::VALUE_NONE, 'Include reachability operators')
      ;
  }

  private $use_bound        = false;
  private $use_cardinality  = false;
  private $use_deadlock     = false;
  private $use_fireability  = false;
  private $use_liveness     = false;
  private $use_boolean      = false;
  private $use_ctl          = false;
  private $use_ltl          = false;
  private $use_reachability = false;
  private $use_integer      = false;
  private $prefix;
  private $quantity;
  private $depth;
  private $id;
  private $reference_model;
  private $model;
  private $places;
  private $transitions;
  private $current_depth = 0;
  private $output;

  private $xml = <<<EOT
  <property>
    <id></id>
    <description>Automatically generated formula.</description>
    <tags>
      <is-structural>false</is-structural>
      <is-reachability>false</is-reachability>
      <is-ctl>false</is-ctl>
      <is-ltl>false</is-ltl>
    </tags>
  </property>
EOT;

  private $types             = array();
  private $boolean_operators = array();
  private $integer_operators = array();

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $path = NULL;
    if ($this->sn_model)
      $path = dirname($this->sn_file);
    else
      $path = dirname($this->pt_file);
    $this->output = "${path}/{$input->getOption('output')}.xml";
    $this->types[INTEGER_FORMULA] = false;
    $this->types[BOOLEAN_FORMULA] = false;
    foreach ($input->getOption('type') as $type)
    {
      if ($type == 'integer')
        $this->types[INTEGER_FORMULA] = true;
      else if ($type == 'boolean')
        $this->types[BOOLEAN_FORMULA] = true;
      else
        echo "Type ${type} is not recognized (should be 'integer' or 'boolean').\n";
    }
    $this->prefix           = $input->getOption('prefix'       );
    $this->use_bound        = $input->getOption('bound'        );
    $this->use_cardinality  = $input->getOption('cardinality'  );
    $this->use_dealock      = $input->getOption('deadlock'     );
    $this->use_fireability  = $input->getOption('fireability'  );
    $this->use_liveness     = $input->getOption('liveness'     );
    $this->use_boolean      = $input->getOption('boolean'      );
    $this->use_ctl          = $input->getOption('ctl'          );
    $this->use_ltl          = $input->getOption('ltl'          );
    $this->use_reachability = $input->getOption('reachability' );
    $this->use_integer      = $input->getOption('integer'      );
    $this->quantity = intval($input->getOption('quantity'));
    $this->depth    = intval($input->getOption('depth'));
    $this->id = 1;
    $this->reference_model = new EquivalentElements($this->sn_model, $this->pt_model);
    $this->places = $this->sn_model
      ? $this->reference_model->cplaces
      : $this->reference_model->uplaces;
    $this->transitions = $this->sn_model
      ? $this->reference_model->ctransitions
      : $this->reference_model->utransitions;
    $this->integer_operators[INTEGER_CONSTANT     ] = true;
    $this->integer_operators[BOUND_OPERATOR       ] = $this->use_bound;
    $this->integer_operators[CARDINALITY_OPERATOR ] = $this->use_cardinality;
    $this->integer_operators[INTEGER_OPERATOR     ] = $this->use_integer;
    $this->boolean_operators[INTEGER_COMPARISON   ] = $this->use_integer;
    $this->boolean_operators[BOOLEAN_CONSTANT     ] = true;
    $this->boolean_operators[DEADLOCK_OPERATOR    ] = $this->use_dealock;
    $this->boolean_operators[FIREABILITY_OPERATOR ] = $this->use_fireability;
    $this->boolean_operators[LIVENESS_OPERATOR    ] = $this->use_liveness;
    $this->boolean_operators[BOOLEAN_OPERATOR     ] = $this->use_boolean;
    $this->boolean_operators[CTL_OPERATOR         ] = $this->use_ctl;
    $this->boolean_operators[LTL_OPERATOR         ] = $this->use_ltl;
    $this->boolean_operators[REACHABILITY_OPERATOR] = $this->use_reachability;
    if ($this->sn_model)
      $this->model = $this->sn_model;
    else
      $this->model = $this->pt_model;
  }

  protected function perform()
  {
    if (file_exists($this->output))
      unlink($this->output);
    $result = array();
    for ($i = 0; $i < $this->quantity; $i++)
    {
      // Choose between integer and boolean formula:
      $type = array_rand(array_filter($this->types));
      switch ($type)
      {
      case INTEGER_FORMULA:
        $result[] = $this->generate_integer_formula();
        break;
      case BOOLEAN_FORMULA:
        $result[] = $this->generate_boolean_formula();
        break;
      }
    }
    $xml = $this->load_xml("<property-set/>");
    foreach ($result as $formula)
    {
      $f = $this->load_xml($this->xml);
      $f->id = $this->model->net->attributes()['id'] .
               "-{$this->prefix}-" . $this->id;
      $this->xml_adopt($f, $formula);
      $this->xml_adopt($xml, $f);
      $this->id++;
    }
    file_put_contents($this->output, $xml->asXml());
  }

  private function generate_integer_formula()
  {
    $result = null;
    $this->current_depth++;
    $back = $this->copy($this->integer_operators);
    if ($this->current_depth >= $this->depth)
    {
      $this->integer_operators[INTEGER_OPERATOR] = false;
    }
    $operator = array_rand(array_filter($this->integer_operators));
    $this->integer_operators = $back;
    switch ($operator)
    {
    case INTEGER_CONSTANT:
      $result = $this->generate_integer_constant();
      break;
    case INTEGER_OPERATOR:
      $result = $this->generate_integer_operator();
      break;
    case BOUND_OPERATOR:
      $result = $this->generate_bound();
      break;
    case CARDINALITY_OPERATOR:
      $result = $this->generate_cardinality();
      break;
    }
    $this->current_depth--;
    return $result;
  }

  private function generate_boolean_formula($with = array(), $without = array())
  {
    $result = null;
    $this->current_depth++;
    $back = $this->copy($this->boolean_operators);
    if ($this->current_depth >= $this->depth)
    {
      $this->boolean_operators[CTL_OPERATOR] = false;
      $this->boolean_operators[LTL_OPERATOR] = false;
      $this->boolean_operators[REACHABILITY_OPERATOR] = false;
    }
    foreach ($with as $v)
    {
      $this->boolean_operators[$v] = true;
    }
    foreach ($without as $v)
    {
      $this->boolean_operators[$v] = false;
    }
    $operator = array_rand(array_filter($this->boolean_operators));
    $this->boolean_operators = $back;
    switch ($operator)
    {
    case BOOLEAN_CONSTANT:
      $result = $this->generate_boolean_constant();
      break;
    case DEADLOCK_OPERATOR:
      $result = $this->generate_deadlock();
      break;
    case FIREABILITY_OPERATOR:
      $result = $this->generate_fireability();
      break;
    case INTEGER_COMPARISON:
      $result = $this->generate_comparison();
      break;
    case LIVENESS_OPERATOR:
      $result = $this->generate_liveness();
      break;
    case BOOLEAN_OPERATOR:
      $result = $this->generate_boolean_operator();
      break;
    case CTL_OPERATOR:
      $result = $this->generate_ctl();
      break;
    case LTL_OPERATOR:
      $result = $this->generate_ltl();
      break;
    case REACHABILITY_OPERATOR:
      $result = $this->generate_reachability();
      break;
    }
    $this->current_depth--;
    return $result;
  }

  private function generate_integer_constant($min = 0, $max = 42)
  {
    $r = rand($min, $max);
    $xml = "<integer-constant>${r}</integer-constant>";
    $result = $this->load_xml($xml);
    return $result;
  }

  private function generate_integer_operator()
  {
    $constants = array(
      "<sum></sum>",
      "<difference></difference>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    for ($i = 0; $i != 2; $i++) {
      $sub = $this->generate_integer_formula();
      $this->xml_adopt($result, $sub);
    }
    return $result;
  }

  private function generate_bound()
  {
    $selected = array_rand($this->places, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<place-bound></place-bound>';
    $result = $this->load_xml($xml);
    foreach ($selected as $s) {
      $place = $this->places[$s];
      $result->addChild('place', $place->id);
    }
    return $result;
  }

  private function generate_cardinality()
  {
    $selected = array_rand($this->places, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<tokens-count></tokens-count>';
    $result = $this->load_xml($xml);
    foreach ($selected as $s) {
      $place = $this->places[$s];
      $result->addChild('place', $place->id);
    }
    return $result;
  }

  private function generate_boolean_constant()
  {
    $constants = array(
      "<true/>",
      "<false/>"
    );
    $r = array_rand($constants, 1);
    $xml = $constants[$r];
    $result = $this->load_xml($xml);
    return $result;
  }

  private function generate_boolean_operator()
  {
    $constants = array(
      "<negation></negation>",
      "<conjunction></conjunction>",
      "<disjunction></disjunction>",
      "<exclusive-disjunction></exclusive-disjunction>",
      "<implication></implication>",
      "<equivalence></equivalence>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    $max = 2;
    if ($r == 0)
      $max = 1;
    for ($i = 0; $i != $max; $i++) {
      $sub = $this->generate_boolean_formula();
      $this->xml_adopt($result, $sub);
    }
    return $result;
  }

  private function generate_deadlock()
  {
    $xml = "<deadlock/>";
    $result = $this->load_xml($xml);
    return $result;
  }

  private function generate_reachability()
  {
    $constants = array(
      "<invariant></invariant>",
      "<possibility></possibility>",
      "<impossibility></impossibility>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    $back = $this->copy($this->boolean_operators);
    $old_reach = $this->boolean_operators[REACHABILITY_OPERATOR];
    $this->boolean_operators[CTL_OPERATOR         ] = false;
    $this->boolean_operators[LTL_OPERATOR         ] = false;
    $this->boolean_operators[REACHABILITY_OPERATOR] = false;
    $sub = $this->generate_boolean_formula();
    $this->boolean_operators = $back;
    $this->xml_adopt($result, $sub);
    return $result;
  }

  private function generate_ctl()
  {
    $constants = array(
      "<all-paths/>",
      "<exists-path/>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    $sub = $this->generate_boolean_formula(array(LTL_OPERATOR), array(CTL_OPERATOR));
    $this->xml_adopt($result, $sub);
    return $result;
  }

  private function generate_ltl()
  {
    $constants = array(
      "<next><if-no-successor>false</if-no-successor><steps>1</steps></next>",
      "<globally/>",
      "<finally/>",
      "<until><before/><reach/><strength>strong</strength></until>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    switch ($r)
    {
    case 0:
    case 1:
    case 2:
      $sub = $this->generate_boolean_formula();
      $this->xml_adopt($result, $sub);
      break;
    case 0:
      $before = $this->generate_boolean_formula();
      $reach  = $this->generate_boolean_formula();
      $this->xml_adopt($result->before, $before);
      $this->xml_adopt($result->reach , $reach );

      break;
    }
    return $result;
  }

  private function generate_comparison()
  {
    $constants = array(
      "<integer-eq/>",
      "<integer-ne/>",
      "<integer-lt/>",
      "<integer-le/>",
      "<integer-gt/>",
      "<integer-ge/>"
    );
    $r = array_rand($constants, 1);
    $result = $this->load_xml($constants[$r]);
    for ($i = 0; $i != 2; $i++) {
      $sub = $this->generate_integer_formula();
      $this->xml_adopt($result, $sub);
    }
    return $result;
  }

  private function generate_fireability()
  {
    $selected = array_rand($this->transitions, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<is-fireable></is-fireable>';
    $result = $this->load_xml($xml);
    foreach ($selected as $s) {
      $transition = $this->transitions[$s];
      $result->addChild('transition', $transition->id);
    }
    return $result;
  }

  private function generate_liveness()
  {
    $selected = array_rand($this->transitions, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<is-live><level>l4</level></is-live>';
    $result = $this->load_xml($xml);
    foreach ($selected as $s) {
      $transition = $this->transitions[$s];
      $result->addChild('transition', $transition->id);
    }
    return $result;
  }

  private function load_xml($xml)
  {
    return new \SimpleXmlElement($xml, LIBXML_COMPACT | LIBXML_NOBLANKS);
  }

  // http://stackoverflow.com/questions/4778865/php-simplexml-addchild-with-another-simplexmlelement
  private function xml_adopt($root, $new)
  {
    $node = $root->addChild($new->getName(), (string) $new);
    foreach($new->attributes() as $attr => $value)
    {
      $node->addAttribute($attr, $value);
    }
    foreach($new->children() as $ch)
    {
      $this->xml_adopt($node, $ch);
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

}
