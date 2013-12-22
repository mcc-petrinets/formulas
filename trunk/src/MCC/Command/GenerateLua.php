<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class GenerateLua extends Base
{

  protected function configure()
  {
    $this
      ->setName('generate-lua')
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
    $file  = dirname(realpath($this->pt_file)) . '/model.lua';
    $fp = fopen($file, 'w');
    // First, output initial state:
    fwrite($fp, "local initial_state = {}\n");
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $initial = (string) $place->initialMarking->text;
      if (($initial == NULL) || ($initial == ""))
      {
        $initial = 0;
      }
      fwrite($fp, "initial_state[\"${id}\"] = ${initial}\n");
    }
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
    fwrite($fp, "local function next_state (s)\n");
    fwrite($fp, "  local result = {}\n");
    foreach ($model->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      fwrite($fp, "  -- Transition ${name} (id: ${id}):\n");
      fwrite($fp, "  if true");
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
          fwrite($fp, " and s[\"${source}\"] >= ${value}");
        }
      }
      fwrite($fp, "  then\n");
      fwrite($fp, "    local r = {}\n");
      fwrite($fp, "    for k, v in pairs(s) do\n");
      fwrite($fp, "      r[k] = v\n");
      fwrite($fp, "    end\n");
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
          fwrite($fp, "    r[\"${source}\"] = r[\"${source}\"] - ${value}\n");
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
          fwrite($fp, "    r[\"${target}\"] = r[\"${target}\"] + ${value}\n");
        }
      }
      fwrite($fp, "    result[\"${id}\"] = r\n");
      fwrite($fp, "  end\n");
    }
    fwrite($fp, "  return result\n");
    fwrite($fp, "end\n");
    fwrite($fp, "return { initial_state = initial_state, next_state = next_state }\n");
    fclose($fp);
  }
}
