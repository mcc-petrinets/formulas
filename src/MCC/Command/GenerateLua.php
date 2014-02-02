<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputOption;
use \Symfony\Component\Console\Input\InputArgument;
use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \MCC\Command\Base;

class GenerateLua extends Base
{

  private $server_side = false;

  protected function configure()
  {
    $this
      ->setName('generate-lua')
      ->setDescription('TODO')
      ->addOption('server-side', null, InputOption::VALUE_NONE, 'Generate server-side code.');
    parent::configure();
  }

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->server_side = $input->getOption('server-side');
  }

  protected function perform()
  {
    if ($this->pt_model == NULL)
    {
      return;
    }
    $model = $this->pt_model;
    $file  = NULL;
    if ($this->server_side) {
      $file = dirname(realpath($this->pt_file)) . '/model.lua';
    } else {
      $file = dirname(realpath($this->pt_file)) . '/full.lua';
    }
    $fp = fopen($file, 'w');
    // Export places as integers:
    $i = 1;
    $dictionary = array();
    fwrite($fp, "local dictionary = {}\n");
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      fwrite($fp, "dictionary[\"${id}\"] = ${i}\n");
      $dictionary[$id] = $i;
      $i += 1;
    }
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
      fwrite($fp, "initial_state[${dictionary[$id]}] = ${initial}\n");
    }
    if ($this->server_side) {
      fwrite($fp, "return { initial_state = initial_state }\n");
      fclose($fp);
      $file  = dirname(realpath($this->pt_file)) . '/transition.lua';
      $fp = fopen($file, 'w');
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
          fwrite($fp, " and s[${dictionary[$source]}] >= ${value}");
        }
      }
      fwrite($fp, " then\n");
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
          fwrite($fp, "    r[${dictionary[$source]}] = r[${dictionary[$source]}] - ${value}\n");
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
          fwrite($fp, "    r[${dictionary[$target]}] = r[${dictionary[$target]}] + ${value}\n");
        }
      }
      fwrite($fp, "    result['${id}'] = r\n");
      fwrite($fp, "  end\n");
    }
    fwrite($fp, "  return result\n");
    fwrite($fp, "end\n");
    if ($this->server_side) {
      $last = <<<EOT
local todo = redis.call('zrange', 'todo', '0', '1')[1]
if todo then
  redis.call('zrem', 'todo', todo)
  local state = cjson.decode(todo)
  local successors = next_state(state)
  for transition, next in pairs(successors) do
    local s = cjson.encode(next)
    redis.call('hset', state, transition, s)
    local is_new = redis.call('setnx', s, '')
    if is_new == 1 then
      redis.call('incr', 'count')
      redis.call('zadd', 'todo', '1', s)
    end
  end
end
EOT;
      fwrite($fp, $last);
    } else {
      $last = <<<EOT
return {
  initial_state = initial_state,
  next_state    = next_state,
}
EOT;
      fwrite($fp, $last);
    }
    fclose($fp);
  }
}
