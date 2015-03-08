<?php
namespace MCC\Formula;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;

class SmtFormulaFilter
{
  private $console_output;
  private $places;

  public function __construct ($console_output, $places)
  {
    $this->console_output = $console_output;
    $this->places = $places;
  }

  public function filter_out ($formula)
  {
    // returns true if the formula should be discarded (bad quality); false otherwise

    echo "mcc: smt-filter: calling the filtering routine\n";

    $result = $this->build_smt_encoding ($formula);
    if ($result[2] != "")
    {
      $result[0] = $result[0] && $this->call_smt ($result);
    }
    return ! $result[0];
  }

  private function build_smt_encoding ($formula)
  {
  // Le résultat est un triplet :
  //   * un booléen qui indique si on doit garder la formule ou pas
  //   * un tableau de nom de variables pour la déclaration (j'utilise le nom des 
  //     variables comme clé pour être certain de l'unicité des entrées, la valeur 
  //     associée n'a aucun sens)
  //   * une chaîne de caractère représentant la formule booléenne déjà calculée
    $result = array(true, array(), "");
    switch ((string) $formula->getName())
    {
    case 'all-paths':
    case 'exists-path':
    case 'globally':
    case 'finally':
      $sub = $formula->children()[0];
      $result = $this->build_smt_encoding($sub);
      
      if ($result[2] != "")
      {
        $result[0] = $result[0] && $this->call_smt($result);
      }

      $result[1] = array();
      $result[2] = "";
      break;
    case 'next':
      $sub = $formula->children()[0];
      $result = $this->build_smt_encoding($sub);
      
      if ($result[2] != "")
      {
        $result[0] = $result[0] && $this->call_smt($result);
      }

      $result[1] = array();
      $result[2] = "";
      break;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $result_b = $this->build_smt_encoding($before);
      
      if ($result_b[2] != "")
      {
        $result_b[0] = $result_b[0] && $this->call_smt($result_b);
      }

      if ($result_b[0])
      {
        $result_r = $this->build_smt_encoding($reach);
      
        if ($result_r[2] != "")
        {
           $result_r[0] = $result_r[0] && $this->call_smt($result_r);
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
    case 'is-fireable':
      break;
    case 'negation':
      $sub = $formula->children()[0];
      $result = $this->build_smt_encoding($sub);
      if ($result[2] != '')
      {
          $result[2] = '(not ' . $result[2] . ')';
        }
      break;
    case 'conjunction':
      $result = $this->collect_formula($formula, 'and');
      break;
    case 'disjunction':
      $result = $this->collect_formula($formula, 'or');
      break;
    case 'integer-le':
      $result = $this->collect_formula($formula, '<=');
      break;
    case 'integer-constant':
      $value = (string) $formula;
      $result[2] = $value;
//    $this->console_output->writeln($value);
      break;
    case 'integer-sum':
      $result = $this->collect_formula($formula, '+');
      break;
    case 'integer-difference':
      $result = $this->collect_formula($formula, '-');
      break;
    case 'place-bound':
    case 'tokens-count':
      $res = array();
      foreach ($formula->place as $place)
      {
        $id = (string) $place;
        $name = $this->places[$id]->name;
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

  private function collect_formula($formula, $operator)
  {
//  $this->console_output->writeln('CF ' . $operator);
    $result = array(true, array(), "");
    $form = array();
    $found = false;
    foreach ($formula->children() as $sub)
    {
      $res = $this->build_smt_encoding($sub);
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

  private function call_smt($result)
  {
    $decl = '';
    foreach ($result[1] as $var => $val)
    {
      $decl = '(declare-fun ' . $var . " () Int)\n" . $decl;
    }
    $to_verify = "(set-logic QF_LIA)\n" . $decl . '(assert ' . $result[2] .  ')' .  "\n(check-sat)\n";
//    $this->console_output->writeln("Vérification 1 : \n" . $to_verify);

	 $tmp_file_path = tempnam ("/tmp/", "mcc.formulacheck.smt.");
	 echo "mcc: smt-filter: here is the program:\n";
	 echo $to_verify;
	 #echo "smt: saving to '$tmp_file_path'\n";
    file_put_contents ($tmp_file_path, $to_verify);
    $output = array();
    $command = 'z3 -smt2 ' . $tmp_file_path;
    exec($command, $output);
	 echo "mcc: smt-filter: answer: $output[0]\n";
    if ($output[0] != 'sat')
    {
//      $this->console_output->writeln('unsat 1 ! : ' . $output[0]);
      $res = false;
    }
    else
    {
      $res = true;
      $to_verify = "(set-logic QF_LIA)\n" . $decl . '(assert (not ' .  $result[2] . "))\n(check-sat)\n";
//      $this->console_output->writeln("Vérification 2 : \n" . $to_verify);
	   echo "mcc: smt-filter: same call: another program:\n";
	   echo $to_verify;
      file_put_contents($tmp_file_path, $to_verify);
      $output = array();
      $command = 'z3 -smt2 ' . $tmp_file_path;
      exec($command, $output);
	   echo "mcc: smt-filter: answer: $output[0]\n";
      if ($output[0] != 'sat')
      {
//        $this->console_output->writeln('unsat 2 ! : ' . $output[0]);
        $res = false;
      }
    }

	 unlink ($tmp_file_path);

    return $res;
  }
}
