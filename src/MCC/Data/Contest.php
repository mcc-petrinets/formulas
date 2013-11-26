<?php

namespace MCC\Data;

use \Doctrine\Common\Collections\Collection;
use \Doctrine\Common\Collections\ArrayCollection;

/**
 * @Entity
 * @Table(name="Contest")
 */
class Contest {

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
  protected $edition;

  /**
   * @ManyToMany(targetEntity="\MCC\Data\Model")
   * @var Collection
   */
  protected $models;

  /**
   * @ManyToMany(targetEntity="\MCC\Data\Instance")
   * @var Collection
   */
  protected $instances;

  /**
   * @ManyToMany(targetEntity="\MCC\Data\Tool")
   * @var Collection
   */
  protected $tools;

  /**
   * @OneToMany(targetEntity="\MCC\Data\Examination", mappedBy="contest")
   * @var Collection
   */
  protected $examinations;

  public function __construct($edition) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> edition = $edition;
    $this -> models = new ArrayCollection();
    $this -> instances = new ArrayCollection();
    $this -> tools = new ArrayCollection();
    $this -> runs = new ArrayCollection();
    $this -> examinations = new ArrayCollection();
  }

  public function __toString() {
    return "mcc {$this->edition}";
  }

  public function add($value) {
    if (get_class($value) == "MCC\Data\Examination") {
      $this -> examinations -> add($value);
      $value -> setContest($this);
    } else if (get_class($value) == "MCC\Data\Instance") {
      $this -> instances -> add($value);
    } else if (get_class($value) == "MCC\Data\Model") {
      $this -> models -> add($value);
    } else if (get_class($value) == "MCC\Data\Tool") {
      $this -> tools -> add($value);
    } else {
      throw new Exception("");
    }
    return $this;
  }

  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this -> $property;
    }
  }

}
