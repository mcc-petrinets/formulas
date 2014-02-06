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
const BOOLEAN_OPERATOR      = 'boolean';
const CTL_OPERATOR          = 'ctl';
const LTL_OPERATOR          = 'ltl';
const REACHABILITY_OPERATOR = 'breachability';

const BOOLEAN_CONSTANT      = 'constant';
const INTEGER_CONSTANT      = 'constant';

class GenerateAtomicPropositions extends Base
{

  protected function configure()
  {
    parent::configure();
    $this->setName('generate')
      ->setDescription('Generates atomic propositions')
      ->addOption('bound'       , null, InputOption::VALUE_NONE, 'Include bound operator')
      ->addOption('cardinality' , null, InputOption::VALUE_NONE, 'Include cardinality operator')
      ->addOption('deadlock'    , null, InputOption::VALUE_NONE, 'Include dealock operator')
      ->addOption('fireability' , null, InputOption::VALUE_NONE, 'Include fireability operator')
      ->addOption('liveness'    , null, InputOption::VALUE_NONE, 'Include liveness operator')
      ->addOption('boolean'     , null, InputOption::VALUE_NONE, 'Include boolean operators between subformulas')
      ->addOption('ctl'         , null, InputOption::VALUE_NONE, 'Include CTL operators')
      ->addOption('ltl'         , null, InputOption::VALUE_NONE, 'Include LTL operators')
      ->addOption('reachability', null, InputOption::VALUE_NONE, 'Include reachability operators')
      ->addOption('integer'     , null, InputOption::VALUE_NONE, 'Include integer formulas')
      ->addOption('quantity', null, InputOption::VALUE_REQUIRED, 'Quantity of properties to generate (at most)', 1)
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
  private $quantity;
  private $id;
  private $reference_model;
  private $places;
  private $transitions;

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
    $this->id = 1;
    $this->reference_model = new EquivalentElements($this->sn_model, $this->pt_model);
    $this->places = $this->sn_model
      ? $this->reference_model->cplaces
      : $this->reference_model->uplaces;
    $this->transitions = $this->sn_model
      ? $this->reference_model->ctransitions
      : $this->reference_model->utransitions;
    $this->types[INTEGER_FORMULA] = $this->use_integer;
    $this->types[BOOLEAN_FORMULA] = $this->use_boolean
      || $this->use_ctl
      || $this->use_ltl
      || $this->use_reachability;
    $this->integer_operators[INTEGER_CONSTANT     ] = true;
    $this->integer_operators[BOUND_OPERATOR       ] = $this->use_bound;
    $this->integer_operators[CARDINALITY_OPERATOR ] = $this->use_cardinality;
    $this->boolean_operators[BOOLEAN_CONSTANT     ] = true;
    $this->boolean_operators[DEADLOCK_OPERATOR    ] = $this->use_dealock;
    $this->boolean_operators[FIREABILITY_OPERATOR ] = $this->use_fireability;
    $this->boolean_operators[LIVENESS_OPERATOR    ] = $this->use_liveness;
    $this->boolean_operators[BOOLEAN_OPERATOR     ] = $this->use_boolean;
    $this->boolean_operators[CTL_OPERATOR         ] = $this->use_ctl;
    $this->boolean_operators[LTL_OPERATOR         ] = $this->use_ltl;
    $this->boolean_operators[REACHABILITY_OPERATOR] = $this->use_reachability;
  }

  protected function perform()
  {
    $result = array();
    for ($i = 0; $i < $this->quantity; $i++)
    {
      // Choose between integer and boolean formula:
      $type = array_rand(array_filter($this->types));
      switch ($type)
      {
      case INTEGER_FORMULA:
        $oldvalue = $this->integer_operators[INTEGER_CONSTANT];
        $this->integer_operators[INTEGER_CONSTANT] = false;
        $result = array_merge($result, $this->generate_integer_formula());
        $this->integer_operators[INTEGER_CONSTANT] = $oldvalue;
        break;
      case BOOLEAN_FORMULA:
        $result = array_merge($result, $this->generate_boolean_formula());
        break;
      }
    }
    foreach ($result as $formula)
    {
      if ($formula->sn)
      {
        echo "sn:\n";
        $f = $this->load_xml($this->xml);
        $f->id = $this->sn_model->net->attributes()['id'] . "-property-" . $this->id;
        $this->xml_adopt($f, $formula->sn);
        echo $f->asXml() . "\n";
      }
      if ($formula->pt)
      {
        echo "pt:\n";
        $f = $this->load_xml($this->xml);
        $f->id = $this->pt_model->net->attributes()['id'] . "-property-" . $this->id;
        $this->xml_adopt($f, $formula->pt);
        echo $f->asXml() . "\n";
      }
      $this->id++;
    }
    return $result;
  }

  private function generate_integer_formula()
  {
    $result = array();
    $operator = array_rand(array_filter($this->integer_operators));
    switch ($operator)
    {
    case INTEGER_CONSTANT:
      $r[] = rand(0,42);
      break;
    case BOUND_OPERATOR:
      $r = $this->generate_bound();
      if ($r !== NULL) $result[] = $r;
      break;
    case CARDINALITY_OPERATOR:
        $r = $this->generate_cardinality();
      if ($r !== NULL) $result[] = $r;
      break;
    }
    return $result;
  }

  private function generate_boolean_formula()
  {
    $result = array();
    $operator = array_rand(array_filter($this->boolean_operators));
    switch ($operator)
    {
    case BOOLEAN_CONSTANT:
      break;
    case DEADLOCK_OPERATOR:
      break;
    case FIREABILITY_OPERATOR:
      break;
    case LIVENESS_OPERATOR:
      break;
    case BOOLEAN_OPERATOR:
      break;
    case CTL_OPERATOR:
      break;
    case LTL_OPERATOR:
      break;
    case REACHABILITY_OPERATOR:
      break;
    }
    return $result;
  }

  private function generate_bound()
  {
    if (! $this->integer_operators[BOUND_OPERATOR]) {
      return NULL;
    }
    $selected = array_rand($this->places, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<place-bound></place-bound>';
    $ref = $this->load_xml($xml);
    foreach ($selected as $s) {
      $place = $this->places[$s];
      $ref->addChild('place', $place->id);
      $unfolded = null;
      if (count($place->unfolded) != 0)
      {
        $unfolded = $this->load_xml($xml);
        foreach ($place->unfolded as $uplace)
        {
          $unfolded->addChild('place', $uplace->id);
        }
      }
    }
    return $this->formula($ref, $unfolded);
  }

  private function generate_cardinality()
  {
    if (! $this->integer_operators[CARDINALITY_OPERATOR]) {
      return NULL;
    }
    $selected = array_rand($this->places, 1);
    if (! is_array($selected))
    {
      $selected = array($selected);
    }
    $xml = '<tokens></tokens>';
    $ref = $this->load_xml($xml);
    foreach ($selected as $s) {
      $place = $this->places[$s];
      $ref->addChild('place', $place->id);
      $unfolded = null;
      if (count($place->unfolded) != 0)
      {
        $unfolded = $this->load_xml($xml);
        foreach ($place->unfolded as $uplace)
        {
          $unfolded->addChild('place', $uplace->id);
        }
      }
    }
    return $this->formula($ref, $unfolded);
  }

  private function formula($ref, $unfolded)
  {
    $result = new Formula();
    if ($this->sn_model && $this->pt_model)
    {
      $result->sn = $ref;
      $result->pt = $unfolded;
    }
    else if ($this->sn_model)
    {
      $result->sn = $ref;
    }
    else if ($this->pt_model)
    {
      $result->pt = $ref;
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

}
