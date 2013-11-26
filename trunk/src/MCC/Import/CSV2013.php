<?php

namespace MCC\Import;

use \MCC\Data\Contest;
use \MCC\Data\Examination;
use \MCC\Data\Formalism;
use \MCC\Data\Instance;
use \MCC\Data\Model;
use \MCC\Data\Tool;
use \External\ParseCSV;
use \Symfony\Component\Console\Command\Command;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Helper\ProgressHelper;

class CSV2013 {

  private $resultsdir;
  private $year;

  public function __construct($year, $dir) {
    $this -> year = $year;
    $this -> resultsdir = $dir;
  }

  function tool($name) {
    global $entityManager;
    $result = $entityManager -> getRepository('\MCC\Data\Tool') -> findBy(array('name' => $name));
    if (count($result) == 0) {
      $result = new Tool($name);
      $entityManager -> flush();
    } else if (count($result) == 1) {
      $result = $result[0];
    } else {
      throw new Exception('');
    }
    return $result;
  }

  function formalism($name) {
    global $entityManager;
    $result = $entityManager -> getRepository('\MCC\Data\Formalism') -> findBy(array('acronym' => $name));
    if (count($result) == 0) {
      $result = new Formalism($name);
      $entityManager -> flush();
    } else if (count($result) == 1) {
      $result = $result[0];
    } else {
      throw new Exception('');
    }
    return $result;
  }

  function model($name) {
    global $entityManager;
    $result = $entityManager -> getRepository('\MCC\Data\Model') -> findBy(array('name' => $name));
    if (count($result) == 0) {
      $result = new Model($name);
      $entityManager -> flush();
    } else if (count($result) == 1) {
      $result = $result[0];
    } else {
      throw new Exception('');
    }
    return $result;
  }

  function instance(Model $model, Formalism $formalism, $parameter) {
    global $entityManager;
    $result = $entityManager -> getRepository('\MCC\Data\Instance') -> findBy(array('formalism' => $formalism, 'parameter' => $parameter));
    if (count($result) == 0) {
      $result = new Instance($model, $formalism, $parameter);
      $entityManager -> flush();
    } else if (count($result) == 1) {
      $result = $result[0];
    } else {
      throw new Exception('');
    }
    return $result;
  }

  function contest($name) {
    global $entityManager;
    $result = $entityManager -> getRepository('\MCC\Data\Contest') -> findBy(array('edition' => $name));
    if (count($result) == 0) {
      $result = new Contest($name);
      $entityManager -> flush();
    } else if (count($result) == 1) {
      $result = $result[0];
    } else {
      throw new Exception('');
    }
    return $result;
  }

  function call(Command $command, OutputInterface $output) {
    global $entityManager;
    $contest = $this -> contest($this -> year);
    $resource = opendir($this -> resultsdir);
    while (false !== ($entry = readdir($resource))) {
      $file = $this -> resultsdir . $entry;
      $datafile = $this -> resultsdir . 'current.data';
      if (is_file($file) && (false !== strripos($entry, '.csv'))) {
        $lines = file($file);
        // Filter lines:
        unset($lines[0]);
        unset($lines[1]);
        unset($lines[3]);
        unset($lines[4]);
        $dresource = fopen($datafile, 'w');
        foreach ($lines as $line) {
          fwrite($dresource, $line);
        }
        fclose($dresource);
        $entryname = preg_replace('/\.csv$/', '', $entry);
        echo "Importing {$entryname}...\n";
        $csv = new ParseCSV($datafile);
        $data = $csv -> data;
        $progress = $command -> getHelperSet() -> get('progress');
        $progress -> start($output, count($data) * (floor(count($data[0]) / 4)));
        // Extract runs:
        for ($i = 0; $i < count($data); $i++) {
          // TODO
          $line = $data[$i];
          list($var, $modelName) = each($line);
          list($var, $formalismName) = each($line);
          list($var, $parameter) = each($line);
          $model = $this -> model($modelName);
          $formalism = $this -> formalism($formalismName);
          $instance = $this -> instance($model, $formalism, $parameter);
          for ($j = 0; $j < floor(count($line) / 4); $j++) {
            list($toolName, $result) = each($line);
            list($var, $time) = each($line);
            list($var, $cpu) = each($line);
            list($var, $memory) = each($line);
            $tool = $this -> tool($toolName);
            // Create examination:
            $examination = new Examination($tool, $instance);
            $examination -> setTime($time);
            $examination -> setCPU($cpu);
            $examination -> setMemory($memory);
            $examination -> setResult($result);
            if (false !== stripos($entry, 'StateSpace')) {
              $examination -> setStateSpace();
            }
            if (false !== stripos($entry, 'Reachability')) {
              $examination -> setReachability();
            }
            if (false !== stripos($entry, 'CTL')) {
              $examination -> setCTL();
            }
            if (false !== stripos($entry, 'LTL')) {
              $examination -> setLTL();
            }
            if (false !== stripos($entry, 'Deadlock')) {
              $examination -> setDeadlock();
            }
            if (false !== stripos($entry, 'Fireability')) {
              $examination -> setFireability();
            }
            if (false !== stripos($entry, 'CardinalityComparison')) {
              $examination -> setCardinalityComparison();
            }
            if (false !== stripos($entry, 'MarkingComparison')) {
              $examination -> setMarkingComparison();
            }
            if (false !== stripos($entry, 'PlaceComparison')) {
              $examination -> setPlaceComparison();
            }
            if (false !== stripos($entry, 'Mix')) {
              $examination -> setDeadlock();
              $examination -> setFireability();
              $examination -> setCardinalityComparison();
              $examination -> setMarkingComparison();
              $examination -> setPlaceComparison();
            }
            $contest -> add($examination);
            //echo "  Adding run ${examination}...\n";
            $progress -> advance();
          }
        }
        $entityManager -> flush();
        unlink($datafile);
        echo " done.\n";
      }
    }
    closedir($resource);
  }

}
