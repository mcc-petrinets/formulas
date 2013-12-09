<?php
namespace MCC\Formula;

class Formula {

  public $sn;
  public $pt;

}

class AtomicPropositions {

  public $cmodel;
  public $umodel;

  public function __construct(\SimpleXMLElement $cmodel = NULL,
                              \SimpleXMLElement $umodel = NULL)
  {
    $this->cmodel = $cmodel;
    $this->umodel = $umodel;
  }

  public function deadlock()
  {
    $result = new Formula();
    $xml = '<deadlock />';
    if ($this->cmodel)
    {
      $result->sn = new \SimpleXMLElement($xml);
    }
    if ($this->umodel)
    {
      $result->pt = new \SimpleXMLElement($xml);
    }
    return $result;
  }

  public function is_live()
  {
    $result = array();
    return $result;
  }

  public function is_fireable()
  {
    $result = array();
    return $result;
  }

  public function bound(array $places)
  {
    $result = new Formula();
    $xml = '<place-bound></place-bound>';
    $ref_model = $this->cmodel ? $this->cmodel : $this->umodel;
    $ref_result = new \SimpleXMLElement($xml);
    foreach ($places as $place)
    {
      $ref_result->addChild('place', $place->id);
    }
    if ($this->cmodel && $this->umodel)
    {
      $u_result = new \SimpleXMLElement($xml);
      foreach ($places as $cplace)
      {
        foreach ($cplace->unfolded as $uplace)
        {
          $u_result->addChild('place', $uplace->id);
        }
      }
      $result->sn = $ref_result;
      $result->pt = $u_result;
    }
    else if ($this->cmodel)
    {
      $result->sn = $ref_result;
    }
    else if ($this->umodel)
    {
      $result->pt = $ref_result;
    }
    return $result;
  }

  public function cardinality(array $places)
  {
    $result = new Formula();
    $xml = '<cardinality><tokens></tokens></cardinality>';
    $ref_model = $this->cmodel ? $this->cmodel : $this->umodel;
    $ref_result = new \SimpleXMLElement($xml);
    foreach ($places as $place)
    {
      $ref_result->tokens->addChild('place', $place->id);
    }
    if ($this->cmodel && $this->umodel)
    {
      $u_result = new \SimpleXMLElement($xml);
      foreach ($places as $cplace)
      {
        foreach ($cplace->unfolded as $uplace)
        {
          $u_result->tokens->addChild('place', $uplace->id);
        }
      }
      $result->sn = $ref_result;
      $result->pt = $u_result;
    }
    else if ($this->cmodel)
    {
      $result->sn = $ref_result;
    }
    else if ($this->umodel)
    {
      $result->pt = $ref_result;
    }
    return $result;
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
      xml_adopt($node, $ch);
    }
  }

}
