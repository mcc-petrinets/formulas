<?php
namespace MCC\Formula;

use \MCC\Formula\EquivalentPlaces;

class Formula {
  
  public $sn;
  public $pt;

}

class AtomicPropositions {

  public $cmodel;
  public $umodel;
  public $equivalent_places = NULL;

  public function __construct(\SimpleXMLElement $cmodel = NULL, \SimpleXMLElement $umodel = NULL)
  {
    $this->cmodel = $cmodel;
    $this->umodel = $umodel;
    if (($cmodel != NULL) and ($umodel != NULL))
    {
      $this->equivalent_places = new EquivalentPlaces($cmodel, $umodel);
    }
  }

  public function deadlock()
  {
    $xml = '<deadlock />';
    $result = array();
    if ($this->cmodel)
    {
      $result['sn'] = new \SimpleXMLElement($xml);
    }
    if ($this->umodel)
    {
      $result['pt'] = new \SimpleXMLElement($xml);
    }
    return $result;
  }

}
