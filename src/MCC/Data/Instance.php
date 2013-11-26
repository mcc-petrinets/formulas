<?php

namespace MCC\Data;

use \MCC\Data\Formalism;
use \MCC\Data\Model;

/**
 * @Entity
 * @Table(name="Instance")
 */
class Instance {

  /**
   * @Id
   * @GeneratedValue
   * @Column(type="integer")
   * @var int
   */
  protected $id;

  /**
   * @ManyToOne(targetEntity="\MCC\Data\Model", inversedBy="instances")
   * @var Model
   */
  protected $model;

  /**
   * @ManyToOne(targetEntity="\MCC\Data\Formalism")
   * @var Formalism
   */
  protected $formalism = null;

  /**
   * @Column(type="string")
   * @var string
   */
  protected $parameter;

  public function __construct(Model $model, Formalism $formalism, $parameter) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> model = $model;
    $this -> formalism = $formalism;
    $this -> parameter = $parameter;
    $this -> model -> instances -> add($this);
  }

  public function __toString() {
    return "{$this->model->name}-{$this->formalism}-{$this->parameter}";
  }

  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this -> $property;
    }
  }

}
