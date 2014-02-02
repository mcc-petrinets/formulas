<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class GenerateC extends Base
{

  protected function configure()
  {
    $this
      ->setName('generate-c')
      ->setDescription('TODO');
    parent::configure();
  }

  protected function perform()
  {
    if ($this->pt_model == NULL)
    {
      return;
    }
    $model = $this->pt_model;
    $file  = dirname(realpath($this->pt_file)) . '/model.hh';
    $fp = fopen($file, 'w');
    // First, output initial state:
    fwrite($fp, "#include <cstdio>\n");
    fwrite($fp, "#include <cstring>\n");
    fwrite($fp, "#include <unordered_map>\n");
    fwrite($fp, "\n");
    fwrite($fp, "struct state {\n");
    fwrite($fp, "  state() = default;\n");
    fwrite($fp, "  ~state() = default;\n");
    fwrite($fp, "  state(const state& rhs) {\n");
    fwrite($fp, "    std::memcpy(this, &rhs, sizeof(state));\n");
    fwrite($fp, "  }\n");
    fwrite($fp, "  state& operator=(const state& rhs) {\n");
    fwrite($fp, "    std::memcpy(this, &rhs, sizeof(state));\n");
    fwrite($fp, "    return *this;\n");
    fwrite($fp, "  }\n");
    fwrite($fp, "  state(state&& rhs) = default;\n");
    fwrite($fp, "  state& operator=(state&& rhs) = default;\n");
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      fwrite($fp, "  MARKING_TYPE ${id};\n");
    }
    fwrite($fp, "};\n");
    fwrite($fp, "\n");
    fwrite($fp, "state initial_state() {\n");
    fwrite($fp, "  state result;\n");
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $initial = (string) $place->initialMarking->text;
      if (($initial == NULL) || ($initial == ""))
      {
        $initial = 0;
      }
      fwrite($fp, "  result.${id} = ${initial};\n");
    }
    fwrite($fp, "  return result;\n");
    fwrite($fp, "}\n");
    fwrite($fp, "\n");
    // Build relation between IDs and arcs:
    $lookup = array();
    foreach ($model->net->page->arc as $arc)
    {
      $source = (string) $arc->attributes()['source'];
      if (! array_key_exists($source, $lookup))
      {
        $lookup[$source] = array();
      }
      $lookup[$source][] = $arc;
      $target = (string) $arc->attributes()['target'];
      if (! array_key_exists($target, $lookup))
      {
        $lookup[$target] = array();
      }
      $lookup[$target][] = $arc;
    }
    // Then, output next-state computation:
    fwrite($fp, "using result_type = std::unordered_map<std::string, state>;\n");
    fwrite($fp, "result_type next_state (const state& s) {\n");
    fwrite($fp, "  auto result = result_type();\n");
    foreach ($model->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      fwrite($fp, "  // Transition ${name} (id: ${id}):\n");
      fwrite($fp, "  if (true");
      foreach ($lookup[$id] as $arc)
      {
        $source = (string) $arc->attributes()['source'];
        $target = (string) $arc->attributes()['target'];
        $value  = (string) $arc->inscription->text;
        if (($value == NULL) || ($value == ""))
        {
          $value = 1;
        }
        if ($target == $id)
        {
          fwrite($fp, " and (s.${source} >= ${value})");
        }
      }
      fwrite($fp, ") {\n");
      fwrite($fp, "    state n = s;\n");
      foreach ($lookup[$id] as $arc)
      {
        $source = (string) $arc->attributes()['source'];
        $target = (string) $arc->attributes()['target'];
        $value  = (string) $arc->inscription->text;
        if (($value == NULL) || ($value == ""))
        {
          $value = 1;
        }
        if ($target == $id)
        {
          fwrite($fp, "    n.${source} -= ${value};\n");
        }
      }
      foreach ($lookup[$id] as $arc)
      {
        $source = (string) $arc->attributes()['source'];
        $target = (string) $arc->attributes()['target'];
        $value  = (string) $arc->inscription->text;
        if (($value == NULL) || ($value == ""))
        {
          $value = 1;
        }
        if ($source == $id)
        {
          fwrite($fp, "    n.${target} += ${value};\n");
        }
      }
      fwrite($fp, "    result.emplace(\"${id}\", n);\n");
      fwrite($fp, "  }\n");
    }
    fwrite($fp, "  return result;\n");
    fwrite($fp, "}\n");
    fclose($fp);
  }
}
