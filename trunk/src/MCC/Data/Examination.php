<?php

namespace MCC\Data;

use \MCC\Data\Instance;
use \MCC\Data\Tool;
use \Doctrine\Common\Collections\Collection;
use \Doctrine\Common\Collections\ArrayCollection;

/**
 * @Entity
 * @Table(name="Examination")
 */
class Examination {

  /**
   * @Id
   * @GeneratedValue
   * @Column(type="integer")
   * @var int
   */
  protected $id;

  /**
   * @ManyToOne(targetEntity="\MCC\Data\Tool")
   * @var Tool
   */
  protected $tool;

  /**
   * @ManyToOne(targetEntity="\MCC\Data\Instance")
   * @var Instance
   */
  protected $instance;

  /**
   * @Column(type="string")
   * @var string
   */
  protected $result;
  /**
   * @Column(type="float")
   * @var float
   */
  protected $time = null;
  /**
   * @Column(type="float")
   * @var float
   */
  protected $cpu = null;
  /**
   * @Column(type="float")
   * @var float
   */
  protected $memory = null;

  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $isStateSpace = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $isReachability = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $isCTL = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $isLTL = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $hasDeadlock = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $hasFireability = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $hasCardinalityComparison = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $hasMarkingComparison = false;
  /**
   * @Column(type="boolean")
   * @var bool
   */
  protected $hasPlaceComparison = false;

  /**
   * @ManyToOne(targetEntity="\MCC\Data\Contest", inversedBy="examinations")
   * @var Contest
   */
  protected $contest = null;

  public function __construct(Tool $tool, Instance $instance) {
    global $entityManager;
    $entityManager -> persist($this);
    $this -> tool = $tool;
    $this -> instance = $instance;
    $this -> result = new ArrayCollection();
  }

  public function __toString() {
    $tags = '';
    if ($this -> isStateSpace) {
      $tags = $tags . ' state-space';
    }
    if ($this -> isReachability) {
      $tags = $tags . ' reachability';
    }
    if ($this -> isCTL) {
      $tags = $tags . ' ctl';
    }
    if ($this -> isLTL) {
      $tags = $tags . ' ltl';
    }
    if ($this -> hasDeadlock) {
      $tags = $tags . ' deadlock';
    }
    if ($this -> hasFireability) {
      $tags = $tags . ' fireability';
    }
    if ($this -> hasCardinalityComparison) {
      $tags = $tags . ' cardiality';
    }
    if ($this -> hasMarkingComparison) {
      $tags = $tags . ' marking';
    }
    if ($this -> hasPlaceComparison) {
      $tags = $tags . ' place';
    }
    return "{$this->tool} on {$this->instance} (tags:{$tags}): {$this->result} [{$this->time}s/{$this->cpu}s/{$this->memory}%]";
  }

  public function setStateSpace() {
    $this -> isStateSpace = true;
    $this -> isReachability = false;
    $this -> isCTL = false;
    $this -> isLTL = false;
    return $this;
  }

  public function setReachability() {
    $this -> isStateSpace = false;
    $this -> isReachability = true;
    $this -> isCTL = false;
    $this -> isLTL = false;
    return $this;
  }

  public function setCTL() {
    $this -> isStateSpace = false;
    $this -> isReachability = false;
    $this -> isCTL = true;
    $this -> isLTL = false;
    return $this;
  }

  public function setLTL() {
    $this -> isStateSpace = false;
    $this -> isReachability = false;
    $this -> isCTL = false;
    $this -> isLTL = true;
    return $this;
  }

  public function setDeadlock() {
    $this -> hasDeadlock = true;
    return $this;
  }

  public function setFireability() {
    $this -> hasFireability = true;
    return $this;
  }

  public function setCardinalityComparison() {
    $this -> hasCardinalityComparison = true;
    return $this;
  }

  public function setMarkingComparison() {
    $this -> hasMarkingComparison = true;
    return $this;
  }

  public function setPlaceComparison() {
    $this -> hasPlaceComparison = true;
    return $this;
  }

  public function setResult($value) {
    $this -> result = $value;
    return $this;
  }

  public function setContest($contest) {
    $this -> contest = $contest;
    return $this;
  }

  public function result() {
    if ($this -> isStateSpace) {
      return floatval($this -> result);
    } else if ($this -> isReachability || $this -> isCTL || $this -> isLTL) {
      $res = array();
      for ($i = 0; $i < strlen($this -> result); $i++) {
        if ($this -> result[$i] == 'T') {
          $res[] = true;
        } else if ($this -> result[$i] == 'F') {
          $res[] = false;
        } else if ($this -> result[$i] == '-') {
          $res[] = null;
        } else {
          throw new Exception("Character representing result {$i} of examination {$this} is not 'T', 'F' or '-'.");
        }
      }
    }
  }

  public function setTime($value) {
    $this -> time = floatval($value);
    return $this;
  }

  public function setCPU($value) {
    $this -> cpu = floatval($value);
    return $this;
  }

  public function setMemory($value) {
    $value = str_replace('%', '', $value);
    $this -> memory = floatval($value) * 100;
    return $this;
  }

  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this -> $property;
    }
  }

}
