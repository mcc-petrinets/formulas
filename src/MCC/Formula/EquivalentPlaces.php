<?php
namespace MCC\Formula;

use \MCC\Formula\Domain;
use \MCC\Formula\Place;

class EquivalentPlaces
{
  public $domains = array();
  public $cplaces = array();
  public $uplaces = array();

  public function __construct(\SimpleXMLElement $cmodel, \SimpleXMLElement $umodel)
  {
    // Load colored domains:
    foreach ($cmodel->net->declaration->structure->declarations->namedsort as $sort)
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
    foreach ($cmodel->net->page->place as $place)
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
    // Load unfolded places:
    foreach ($umodel->net->page->place as $place)
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
        $p .= ')';
      }
      $regex = "^{$place->name}{$p}$";
      error_reporting(E_ERROR);
      foreach ($this->uplaces as $uplace)
      {
        if ((size < 1000) && preg_match('/' . $regex . '/', $uplace->name))
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
