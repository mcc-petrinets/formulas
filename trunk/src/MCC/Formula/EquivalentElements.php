<?php
namespace MCC\Formula;

use \MCC\Formula\Domain;
use \MCC\Formula\Place;
use \MCC\Formula\Transition;

class EquivalentElements
{
  public $cmodel;
  public $umodel;
  public $domains = array();
  public $cplaces = array();
  public $uplaces = array();
  public $ctransitions = array();
  public $utransitions = array();

  public function __construct(\SimpleXMLElement $cmodel = NULL,
                              \SimpleXMLElement $umodel = NULL)
  {
    $this->cmodel = $cmodel;
    $this->umodel = $umodel;
    if ($this->cmodel)
    {
      $this->sn_places();
      $this->sn_transitions();
    }
    else
    {
      $this->pt_places();
      $this->pt_transitions();
    }
  }

  private function sn_places()
  {
    // Load colored domains:
    foreach ($this->cmodel->net->declaration->structure->declarations->namedsort as $sort)
    {
      $id = (string) $sort->attributes()['id'];
      $name = (string) $sort->attributes()['name'];
      $domain = new Domain();
      $domain->id = $id;
      $domain->name = $name;
      if ($sort->cyclicenumeration)
      {
        $domain->values[$id] = array();
        foreach ($sort->cyclicenumeration->feconstant as $value)
        {
          $vid = (string) $value->attributes()['id'];
          $vname = (string) $value->attributes()['name'];
          $domain->values[$id][$vid] = $vname;
        }
      }
      else if ($sort->finiteenumeration)
      {
        $domain->values[$id] = array();
        foreach ($sort->finiteenumeration->feconstant as $value)
        {
          $vid = (string) $value->attributes()['id'];
          $vname = (string) $value->attributes()['name'];
          $domain->values[$id][$vid] = $vname;
        }
      } else if ($sort->productsort)
      {
        $i = 0;
        foreach ($sort->productsort->usersort as $sub)
        {
          $subname = (string) $sub->attributes()['declaration'];
          $domain->values[$i] = $subname;
          $i += 1;
        }
      }
      $this->domains[$id] = $domain;
    }
    // When all domains are declared, fill the products:
    foreach ($this->domains as $domain)
    {
      foreach ($domain->values as $k => $v)
      {
        if (gettype($v) == 'string')
        {
          $domain->values[$k] = $this->domains[$v]->values[$v];
        }
      }
    }
    // Load colored domains and places:
    foreach ($this->cmodel->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      $domain = (string) $place->type->structure->usersort->attributes()['declaration'];
      $cplace = new Place();
      $cplace->id = $id;
      $cplace->name = $name;
      $cplace->domain = $this->domains[$domain];
      $this->cplaces[$id] = $cplace;
    }
    if ($this->umodel)
    {
      // Load unfolded places:
      foreach ($this->umodel->net->page->place as $place)
      {
        $id = (string) $place->attributes()['id'];
        $name = (string) $place->name->text;
        $uplace = new Place();
        $uplace->id = $id;
        $uplace->name = $name;
        $this->uplaces[$id] = $uplace;
      }
      // For each colored place, build regex that recognize its unfoldings:
      foreach ($this->cplaces as $place)
      {
        $size = 1;
        $p = '';
        foreach ($place->domain->values as $k => $v)
        {
          $size *= count($v);
          $first = true;
          $p .= '_(';
          // Warning: here we do an prroximation to save a lot of time:
          if ((count($v) > 100) && (is_numeric(reset($v))))
          {
            $p .= '[0-9]+';
          }
          else
          {
            foreach ($v as $vid => $vname)
            {
              if ($first)
              {
                $first = false;
              } else
              {
                $p .= '|';
              }
                $p .= $vname;
            }
          }
          $p .= ')';
        }
        $regex = "^{$place->name}{$p}$";
        error_reporting(E_ERROR);
        foreach ($this->uplaces as $uplace)
        {
          if ((size < 100) && preg_match('/' . $regex . '/u', $uplace->name))
          {
            $place->unfolded[$uplace->id] = $uplace;
          }
          else if (ereg($regex, $uplace->name))
          {
            $place->unfolded[$uplace->id] = $uplace;
          }
        }
        error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
      }
    } 
  }

  private function sn_transitions()
  {
    // Load colored transitions:
    foreach ($this->cmodel->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      $ctransition = new Transition();
      $ctransition->id = $id;
      $ctransition->name = $name;
      $this->ctransitions[$id] = $ctransition;
    }
    if ($this->umodel)
    {
      // Load unfolded transitions:
      foreach ($this->umodel->net->page->transition as $transition)
      {
        $id = (string) $transition->attributes()['id'];
        $name = (string) $transition->name->text;
        $utransition = new Transition();
        $utransition->id = $id;
        $utransition->name = $name;
        $this->utransitions[$id] = $utransition;
      }
      // For each colored transition, build regex that recognize its unfoldings:
      if ($this->utransitions)
      {
        foreach ($this->ctransitions as $transition)
        {
          $avoids = array ();
          foreach ($this->ctransitions as $other)
          {
            if (($transition != $other) && (strpos($other->name, $transition->name) !== false))
            { // transition name is a prefix of other name
              $avoids[$other->id] = $other->name;
            }
          }
          $regex = "^{$transition->name}(_[^_]+)*$";
          foreach ($this->utransitions as $utransition)
          {
            $check = true;
            foreach ($avoids as $avoid)
            {
              $check = $check && (strpos($utransition->name, $avoid) === false);
            }
            if ($check)
            {
              error_reporting(E_ERROR);
              if ((size < 1000) && preg_match('/' . $regex . '/u', $utransition->name))
              {
                $transition->unfolded[$utransition->id] = $utransition;
              }
              else if (ereg($regex, $utransition->name))
              {
                $transition->unfolded[$utransition->id] = $utransition;
              }
              error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
            }
          }
        }
      }
    }
  }

  private function pt_places()
  {
    // Load colored transitions:
    foreach ($this->umodel->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      $uplace = new Place();
      $uplace->id = $id;
      $uplace->name = $name;
      $this->uplaces[$id] = $uplace;
    }
  }

  private function pt_transitions()
  {
    // Load colored transitions:
    foreach ($this->umodel->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      $utransition = new Transition();
      $utransition->id = $id;
      $utransition->name = $name;
      $this->utransitions[$id] = $utransition;
    }
  }


}
