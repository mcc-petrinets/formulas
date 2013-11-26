<?php

namespace MCC\Data;

/**
 * @Entity
 * @Table(name="Formalism")
 */
class Formalism {

  /**
   * @Id
   * @GeneratedValue
   * @Column(type="integer")
   * @var int
   */
  protected $id;

  /**
   * @Column(type="string")
   * @var string
   */
  protected $acronym;

  public function __construct($acronym) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> acronym = $acronym;
  }

  public function __toString() {
    return "{$this->acronym}";
  }

  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this -> $property;
    }
  }

}
