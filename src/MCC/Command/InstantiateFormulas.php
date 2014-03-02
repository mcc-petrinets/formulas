<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\ArrayInput;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;
use \MCC\Formula\EquivalentElements;

class InstantiateFormulas extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:instantiate')
      ->setDescription('Instantiate formulas')
      ->addOption('pattern-file', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas pattern', './pattern.xml')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
      ->addOption('prefix', null,
        InputOption::VALUE_REQUIRED,
        'Prefix for formula identifiers', 'formula')
      ->addOption('chain', null,
        InputOption::VALUE_NONE,
        'Chain with unfolding and conversion to text')
        ;
    parent::configure();
  }

  private $input = null;
  private $output = null;
  private $output_name;
  private $chain;
  private $reference_model;
  private $places;
  private $transitions;
  private $prefix;
  private $model;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->input = $input->getOption('pattern-file');
    $this->output_name = $input->getOption('output');
    $output_path = null;
    if ($this->sn_model)
    {
      $output_path = dirname($this->sn_file);
    }
    else
    {
      $output_path = dirname($this->pt_file);
    }
    $this->output = "${output_path}/{$input->getOption('output')}.xml";
    $this->prefix = $input->getOption('prefix');
    $this->chain = $input->getOption('chain');
    $this->reference_model = new EquivalentElements($this->sn_model, $this->pt_model);
    $this->places = $this->sn_model
      ? $this->reference_model->cplaces
      : $this->reference_model->uplaces;
    $this->transitions = $this->sn_model
      ? $this->reference_model->ctransitions
      : $this->reference_model->utransitions;
    if ($this->sn_model)
      $this->model = $this->sn_model;
    else
      $this->model = $this->pt_model;
  }

  protected function perform()
  {
    if (file_exists($this->output))
      unlink($this->output);
    $xml = $this->load_xml(file_get_contents($this->input));
    $quantity = count($xml->children());
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    $id = 1;
    foreach ($xml->property as $property)
    {
      $property->id = $this->model->net->attributes()['id'] .
        "-{$this->prefix}-" . $id;
      $this->instantiate_formula($property->formula->children()[0]);
      $this->progress->advance();
      $id += 1;
    }
    $this->progress->finish();
    file_put_contents($this->output, $this->save_xml($xml));
    if ($this->chain)
    {
      foreach (array(
        'formula:tag',
        'formula:unfold',
        'formula:to-text'
      ) as $c)
      {
        $command = $this->getApplication()->find($c);
        $arguments = array(
            'command'   => $c,
            'root'      => $this->root,
            'model'     => $this->model_name,
            'parameter' => $this->parameter,
            '--output'  => $this->output_name,
          );
        if ($c == 'formula:tag')
        {
          $arguments[] = '--no-warning';
        }
        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $this->console_output);
      }
    }
  }

  private function instantiate_formula($formula)
  {
    switch ((string) $formula->getName())
    {
    case 'invariant':
    case 'impossibility':
    case 'possibility':
    case 'all-paths':
    case 'exists-path':
    case 'globally':
    case 'finally':
    case 'negation':
      $sub = $formula->children()[0];
      $this->instantiate_formula($sub);
      return;
    case 'next':
      foreach ($formula->children() as $child)
      {
        if ($child->getName() != 'if-no-successor' &&
            $child->getName() != 'steps')
        {
          $this->instantiate_formula($child);
        }
      }
      return;
    case 'until':
      $before = $formula->before->children()[0];
      $reach  = $formula->reach->children()[0];
      $this->instantiate_formula($before);
      $this->instantiate_formula($reach);
      return;
    case 'deadlock':
    case 'true':
    case 'false':
      return;
    case 'integer-constant':
      if ((string) $formula == "")
      {
        $formula = rand(0, 2);
      }
      return;
    case 'is-live':
    case 'is-fireable':
      $selected = array_rand($this->transitions, 1);
      if (! is_array($selected))
      {
        $selected = array($selected);
      }
      foreach ($selected as $s) {
        $transition = $this->transitions[$s];
        $formula->addChild('transition', $transition->id);
      }
      return;
    case 'conjunction':
    case 'disjunction':
    case 'exclusive-disjunction':
    case 'implication':
    case 'equivalence':
    case 'integer-eq':
    case 'integer-ne':
    case 'integer-lt':
    case 'integer-le':
    case 'integer-gt':
    case 'integer-ge':
    case 'integer-sum':
    case 'integer-product':
    case 'integer-difference':
    case 'integer-division':
      foreach ($formula->children() as $sub)
      {
        $this->instantiate_formula($sub);
      }
      return;
    case 'place-bound':
    case 'tokens-count':
      $selected = array_rand($this->places, 1);
      if (! is_array($selected))
      {
        $selected = array($selected);
      }
      foreach ($selected as $s) {
        $place = $this->places[$s];
        $formula->addChild('place', $place->id);
      }
      return;
    default:
      $this->console_output->writeln(
        "  <warning>Error: unknown node {$formula->getName()}</warning>"
      );
    }
  }

}
