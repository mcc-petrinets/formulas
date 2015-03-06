<?php
namespace MCC\Formula;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;

class CheckFormula
{
  private $__gen_formula_cmd = null;

  public function __construct($ref)
  {
    $this->__gen_formula_cmd = $ref;
  }

  public function perform_check($formula, $places, $transitions, $smt)
  {
  // Le résultat est un triplet :
  //   * un booléen qui indique si on doit garder la formule ou pas
  //   * un tableau de nom de variables pour la déclaration (j'utilise le nom des 
  //     variables comme clé pour être certain de l'unicité des entrées, la valeur 
  //     associée n'a aucun sens)
  //   * une chaîne de caractère représentant la formule booléenne déjà calculée
    $result = array(true, array(), "");
    //$this->__gen_formula_cmd->console_output->writeln("perform_check " . $smt);
    switch ((string) $formula->getName())
    {
    /* case 'invariant': */
    /* case 'impossibility': */
    /* case 'possibility': */
    case 'all-paths':
    case 'exists-path':
    case 'globally':
    case 'finally':
      $sub = $formula->children()[0];
      $result = $this->perform_check($sub, $places, $transitions, $smt);
      
      if ($result[2] != "")
      {
        $result[0] = $result[0] && $this->call_smt($result, $smt);
      }

      $result[1] = array();
      $result[2] = "";
      break;
    case 'next':
      /* $sub = null; */
      /* foreach ($formula->children() as $child) */
      /* { */
      /*   if ($child->getName() != 'if-no-successor' && */
      /*       $child->getName() != 'steps') */
      /*   { */
      /*     $sub = $child; */
      /*   } */
      /* } */
      $sub = $formula->children()[0];
      $result = $this->perform_check($sub, $places, $transitions, $smt);
      
      if ($result[2] != "")
      {
        $result[0] = $result[0] && $this->call_smt($result, $smt);
      }

      $result[1] = array();
      $result[2] = "";
      break;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $result_b = $this->perform_check($before, $places, $transitions, $smt);
      
      if ($result_b[2] != "")
      {
        $result_b[0] = $result_b[0] && $this->call_smt($result_b, $smt);
      }

      if ($result_b[0])
      {
        $result_r = $this->perform_check($reach, $places, $transitions, $smt);
      
        if ($result_r[2] != "")
        {
           $result_r[0] = $result_r[0] && $this->call_smt($result_r, $smt);
        }

        $result[0] = $result_r[0];
      }
      else
      {
        $result[0] = false;
      }
      $result[1] = array();
      $result[2] = "";
      break;
    /* case 'true': */
    /* case 'false': */
    case 'deadlock':
    /* case 'is-live': */
    case 'is-fireable':
      break;




    case 'negation':
      $sub = $formula->children()[0];
      $result = $this->perform_check($sub, $places, $transitions, $smt);
      if ($result[2] != '')
      {
          $result[2] = '(not ' . $result[2] . ')';
        }
      break;
    case 'conjunction':
      $result = $this->collect_formula($formula, 'and', $places, $transitions, $smt);
      break;
    case 'disjunction':
      $result = $this->collect_formula($formula, 'or', $places, $transitions, $smt);
      break;
    /* case 'exclusive-disjunction': */
    /*   $result = $this->collect_formula($formula, 'xor', $places, $transitions, $smt); */
    /*   break; */
    /* case 'implication': */
    /*   $result = $this->collect_formula($formula, '=>', $places, $transitions, $smt); */
    /*   break; */
    /* case 'equivalence': */
    /*   $result = $this->collect_formula($formula, '=', $places, $transitions, $smt); */
    /*   break; */
    /* case 'integer-eq': */
    /*   $result = $this->collect_formula($formula, '=', $places, $transitions, $smt); */
    /*   break; */
    /* case 'integer-ne': */
    /*   $result = $this->collect_formula($formula, 'distinct', $places, $transitions, $smt); */
    /*   break; */
    /* case 'integer-lt': */
    /*   $result = $this->collect_formula($formula, '<', $places, $transitions, $smt); */
    /*   break; */
    case 'integer-le':
      $result = $this->collect_formula($formula, '<=', $places, $transitions, $smt);
      break;
    /* case 'integer-gt': */
    /*   $result = $this->collect_formula($formula, '>', $places, $transitions, $smt); */
    /*   break; */
    /* case 'integer-ge': */
    /*   $result = $this->collect_formula($formula, '>=', $places, $transitions, $smt); */
    /*   break; */
    case 'integer-constant':
      $value = (string) $formula;
      $result[2] = $value;
//    $this->console_output->writeln($value);
      break;
    case 'integer-sum':
      $result = $this->collect_formula($formula, '+', $places, $transitions, $smt);
      break;
    /* case 'integer-product': */
    /*   $result = $this->collect_formula($formula, '*', $places, $transitions, $smt); */
    /*   break; */
    case 'integer-difference':
      $result = $this->collect_formula($formula, '-', $places, $transitions, $smt);
      break;
    /* case 'integer-division': */
    /*   $result = $this->collect_formula($formula, 'div', $places, $transitions, $smt); */
    /*   break; */
    case 'place-bound':
    case 'tokens-count':
      $res = array();
      foreach ($formula->place as $place)
      {
        $id = (string) $place;
        $name = $places[$id]->name;
        $res[] = $name;
        $result[1][$name] = 1;
      }
      if (count($res) >= 2)
      {
        $result[2] = '(+ ' . implode(' ', $res) . ')';
      }
      else
      {
        $result[2] = $res[0];
      }
//    $this->console_output->writeln($result[2]);
      break;
    default:
      $this->__gen_formula_cmd->console_output->writeln(
        "<warning>Error: unknown node {$formula->getName()}</warning>"
      );
    }

    return $result;
  }

  protected function collect_formula($formula, $operator, $places, $transitions, $smt)
  {
//  $this->console_output->writeln('CF ' . $operator);
    $result = array(true, array(), "");
    $form = array();
    $found = false;
    foreach ($formula->children() as $sub)
    {
      $res = $this->perform_check($sub, $places, $transitions, $smt);
      if (! $res[0])
      {
        $result[0] = false;
        $result[1] = array();
        $result[2] = "";

        break;
      }
      if ($res[2] != '')
      {
        $found = true;
        $form[] = $res[2];
        foreach ($res[1] as $var => $val)
        {
          $result[1][$var] = $val;
        }
      }
    }
    if ($found)
    {
      $result[2] = '(' . $operator . ' ' . implode(' ', $form) . ')';
    }
//  $this->console_output->writeln($result[2]);
    return $result;
  }

  public function call_smt($result, $smt)
  {
    $decl = '';
    foreach ($result[1] as $var => $val)
    {
      $decl = '(declare-fun ' . $var . " () Int)\n" . $decl;
    }
    $to_verify = "(set-logic QF_LIA)\n" . $decl . '(assert ' . $result[2] . ')' .  "\n(check-sat)";
//    $this->console_output->writeln("Vérification 1 : \n" . $to_verify);
    file_put_contents($smt, $to_verify);
    $output = array();
    $command = 'z3 -smt2 ' . $smt;
    exec($command, $output);
    if ($output[0] != 'sat')
    {
//      $this->console_output->writeln('unsat 1 ! : ' . $output[0]);
      $res = false;
    }
    else
    {
      $res = true;
      $to_verify = "(set-logic QF_LIA)\n" . $decl . '(assert (not ' . $result[2] . "))\n(check-sat)";
//      $this->console_output->writeln("Vérification 2 : \n" . $to_verify);
      file_put_contents($smt, $to_verify);
      $output = array();
      $command = 'z3 -smt2 ' . $smt;
      exec($command, $output);
      if ($output[0] != 'sat')
      {
//        $this->console_output->writeln('unsat 2 ! : ' . $output[0]);
        $res = false;
      }
    }
    return $res;
  }
}
