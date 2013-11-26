<?php

namespace MCC\Data;

/**
 * @Entity
 * @Table(name="Tool")
 */
class Tool {

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
  protected $name;

  /**
   * @Column(type="string")
   * @var string
   */
  protected $color = 'black';

  public function __construct($name) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> name = $name;
  }

  public function __toString() {
    return "{$this->name}";
  }

  public function setColor($color) {
    $this -> color = $color;
  }

  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this -> $property;
    }
  }

}
