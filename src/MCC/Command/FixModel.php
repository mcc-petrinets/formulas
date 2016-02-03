<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;

class FixModel extends Base
{

  protected function configure()
  {
    $this
      ->setName('model:fix')
      ->setDescription('Fix ids and names')
      ->addOption('dry-run', null, InputOption::VALUE_NONE, 'Only show messages, do not really perform changes');
    parent::configure();
  }

  private $dryrun = false;
  private $log_name = 'model-fix.log';
  private $log;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->dryrun = $this->dryrun || $input->getOption('dry-run');
  }

  protected function perform()
  {
    $quantity = 0;
    if ($this->sn_model)
    {
      $quantity += 2 +
        count($this->sn_model->net->page->place) +
        count($this->sn_model->net->page->transition) +
        count($this->sn_model->net->page->arc);
    }
    if ($this->pt_model)
    {
      $quantity += 2 +
        count($this->pt_model->net->page->place) +
        count($this->pt_model->net->page->transition) +
        count($this->pt_model->net->page->arc);
    }
    $this->progress->setRedrawFrequency(max(1, $quantity / 100));
    $this->progress->start($this->console_output, $quantity);
    if ($this->sn_model)
    {
      $this->perform_for($this->sn_file, $this->sn_model);
    }
    if ($this->pt_model)
    {
      $this->perform_for($this->pt_file, $this->pt_model);
    }
    $this->progress->finish();
    if ($this->sn_model)
    {
      $this->warn_for($this->sn_file, 'Colored');
    }
    if ($this->pt_model)
    {
      $this->warn_for($this->pt_file, 'P/T');
    }
  }

  private function perform_for($file, $model)
  {
    $dir = dirname($file);
    $this->log = fopen("{$dir}/{$this->log_name}", 'w');
    $this->fix($file, $model);
    fclose($this->log);
  }

  private function lines_of($file)
  {
    $result = 0;
    $handle = fopen($file, "r");
    while(! feof($handle))
    {
      $line = fgets($handle);
      $result++;
    }
    fclose($handle);
    return $result - 1;
  }

  private function warn_for($file, $special)
  {
    $dir = dirname($file);
    $count = $this->lines_of("{$dir}/{$this->log_name}");
    if ($count > 1)
    {
      if ($this->dryrun)
      {
        $this->console_output->writeln(
          "<warning>  {$special}: {$count} problems should be fixed.</warning>"
        );
      }
      else
      {
        $this->console_output->writeln(
          "<warning>  {$special}: {$count} problems have been fixed.</warning>"
        );
      }
    }
  }

  private function fix($file, $model)
  {
    $model = simplexml_load_file($file, NULL, LIBXML_COMPACT);
    $instancename = basename(dirname(realpath($file)));
    $id = (string) $model->net->attributes()['id'];
    // Fix model id and name:
    if ($instancename != $id)
    {
      $namematches = array();
      if (preg_match('/^(.*)-(COL|PT)-(.*)$/u', $instancename, $namematches))
      {
        $name = $namematches[1];
        $parameter = $namematches[3];
        $type = NULL;
        switch ($model->net->attributes()['type'])
        {
        case 'http://www.pnml.org/version-2009/grammar/ptnet':
          $type = 'PT';
          break;
        case 'http://www.pnml.org/version-2009/grammar/symmetricnet':
          $type = 'COL';
          break;
        }
        fwrite($this->log, "Fixing id from {$id} to {$name}-{$type}-{$parameter}\n");
        $model->net->attributes()['id'] = "{$name}-{$type}-{$parameter}";
      }
      else
      {
        fwrite($this->log, "{$instancename} does not respect the pattern NAME-(COL|PT)-PARAMETER.\n");
        exit(1);
      }
    }
    $this->progress->advance();
    $name = $model->net->name;
    if ($name == NULL)
    {
      fwrite($this->log, "Fixing name to {$model->net->attributes()['id']}.\n");
      $model->net->addChild('name');
      $model->net->name->addChild('text', "{$model->net->attributes()['id']}");
    }
    else if ((string) $name->text != $model->net->attributes()['id'])
    {
      fwrite($this->log, "Fixing name from {$name->text} to {$model->net->attributes()['id']}.\n");
      $model->net->name->text = "{$model->net->attributes()['id']}";
    }
    $this->progress->advance();

    // Check that all places have different id and different name
    $ids = array ();
    $names = array ();
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      //echo "id '$id' name '$name'\n";

      if (isset ($ids[$id]))
      {
        $s = "Found two places with the same id: '$id'.\n";
        fwrite($this->log, $s);
        echo $s;
        exit (0);
      }
      if (isset ($names[$name]))
      {
        $s = "Found two places with the same name: '$name'.\n";
        fwrite($this->log, $s);
        echo $s;
        exit (0);
      }
      $ids[$id] = 1;
      $names[$name] = 1;
    }

    // Same for all transitions, making sure that ids are also disjoint from
    // those of places
    $names = array ();
    foreach ($model->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      //echo "id '$id' name '$name'\n";

      if (isset ($ids[$id]))
      {
        $s = "Found two transitions/places with the same id: '$id'.\n";
        fwrite($this->log, $s);
        echo $s;
        exit (0);
      }
      if (isset ($names[$name]))
      {
        $s = "Found two transitions with the same name: '$name'.\n";
        fwrite($this->log, $s);
        echo $s;
        exit (0);
      }
      $ids[$id] = 1;
      $names[$name] = 1;
    }

    $replacements = array();
    // Now, fix place ids and names;
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;

      if (($id == NULL) && ($name == NULL))
      {
        fwrite($this->log, "Missing both place id and name.\n");
        exit(1);
      }
      if ($id == NULL)
      {
        fwrite($this->log, "Fixing missing place id for ${name}.\n");
        $id = $this->identifier_of($name);
        $place->addAttribute('id', $id);
      }
      if ($name == NULL)
      {
        fwrite($this->log, "Fixing missing place name for ${id}\n");
        $place->addChild('name');
        $name = $place->attributes()['id'];
        $place->name->addChild('text', $name);
      }
      if (levenshtein($id, $name) >= min(strlen($id), strlen($name))/2)
      {
        $new = $this->identifier_of($name);
        fwrite($this->log, "Fixing ugly place id for ${id} to ${new}.\n");
        $replacements[$id] = $new;
        $place->attributes()['id'] = $new;
      }
      // cesar
      //$this->progress->advance();
    }
    // The same for transitions:
    foreach ($model->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      if (($id == NULL) && ($name == NULL))
      {
        fwrite($this->log, "Missing both transition id and name.\n");
        exit(1);
      }
      if ($id == NULL)
      {
        fwrite($this->log, "Fixing missing transition id for ${name}\n");
        $id = $this->identifier_of($name);
        $transition->addAttribute('id', $id);
      }
      if ($name == NULL)
      {
        fwrite($this->log, "Fixing missing transition name for ${id}.\n");
        $transition->addChild('name');
        $name = $transition->attributes()['id'];
        $transition->name->addChild('text', $name);
      }
      if (levenshtein($id, $name) > min(strlen($id), strlen($name))/2)
      {
        $new = $this->identifier_of($name);
        fwrite($this->log, "Fixing ugly transition id for ${id} to ${new}.\n");
        $replacements[$id] = $new;
        $transition->attributes()['id'] = $new;
      }
      $this->progress->advance();
    }
    // Update arcs:
    if (count($replacements) != 0)
    {
      foreach ($model->net->page->arc as $arc)
      {
        $id = (string) $arc->attributes()['id'];
        $source = (string) $arc->attributes()['source'];
        if (array_key_exists($source, $replacements))
        {
          $r = $replacements[$source];
          fwrite($this->log, "Fixing arc ${id} source from ${source} to ${r}.\n");
          $arc->attributes()['source'] = $r;
        }
        $target = (string) $arc->attributes()['target'];
        if (array_key_exists($target, $replacements))
        {
          $r = $replacements[$target];
          fwrite($this->log, "Fixing arc ${id} target from ${target} to ${r}.\n");
          $arc->attributes()['target'] = $r;
        }
        $this->progress->advance();
      }
    }
    else
    {
      $this->progress->advance(count($model->net->page->arc));
    }
    if (!$this->dryrun)
    {
      $model->asXml($file);
    }
  }

  private function identifier_of($name)
  {
    return preg_replace('/[^[:alnum:]]/', '_', $name);
  }

}
