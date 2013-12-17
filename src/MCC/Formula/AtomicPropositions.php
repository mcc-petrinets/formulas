<?php
namespace MCC\Formula;


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

  public function is_live(array $transitions, $level)
  {
    $result = new Formula();
    $xml = "<is-live><level>{$level}</level><is-live/>";
    $ref_model = $this->cmodel ? $this->cmodel : $this->umodel;
    $ref_result = new \SimpleXMLElement($xml);
    foreach ($transitions as $transition)
    {
      $ref_result->addChild('transition', $transition->id);
    }
    if ($this->cmodel && $this->umodel)
    {
      $u_result = new \SimpleXMLElement($xml);
      foreach ($transitions as $ctransition)
      {
        foreach ($ctransition->unfolded as $utransition)
        {
          $u_result->addChild('transition', $utransition->id);
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

  public function is_fireable(array $transitions)
  {
    $result = new Formula();
    $xml = '<is-fireable><is-fireable/>';
    $ref_model = $this->cmodel ? $this->cmodel : $this->umodel;
    $ref_result = new \SimpleXMLElement($xml);
    foreach ($transitions as $transition)
    {
      $ref_result->addChild('transition', $transition->id);
    }
    if ($this->cmodel && $this->umodel)
    {
      $u_result = new \SimpleXMLElement($xml);
      foreach ($transitions as $ctransition)
      {
        foreach ($ctransition->unfolded as $utransition)
        {
          $u_result->addChild('transition', $utransition->id);
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
}
