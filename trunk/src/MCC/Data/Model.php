<?php

namespace MCC\Data;

use \Doctrine\Common\Collections\Collection;
use \Doctrine\Common\Collections\ArrayCollection;

/**
 * @Entity
 * @Table(name="Model")
 */
class Model {

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

  /**
   * @OneToMany(targetEntity="\MCC\Data\Instance", mappedBy="model")
   * @var Collection
   */
  protected $instances;

  public function __construct($name) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> name = $name;
    $this -> instances = new ArrayCollection();
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

  public function instance($parameter, Formalism $formalism) {
    foreach ($instances as $instance) {
      if (($instance -> parameter == $parameter) && ($instance -> formalism == $formalism)) {
        return $instance;
      }
    }
    throw new Exception("Cannot find instance {$this->name}-{$formalism}-{$parameter}.");
  }

}
