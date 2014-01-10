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
      ->setName('fix-model')
      ->setDescription('TODO')
      ->addOption('dry-run', null, InputOption::VALUE_NONE, 'If set, the model is not really fixed.');
    parent::configure();
  }

  private $dryrun = false;

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->dryrun = $this->dryrun || $input->getOption('dry-run');
  }

  protected function perform()
  {
    if ($this->sn_model)
    {
      $this->fix($this->sn_file, $this->sn_model);
    }
    if ($this->pt_model)
    {
      $this->fix($this->pt_file, $this->pt_model);
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
        echo "  Fixing id from {$id} to {$name}-{$type}-{$parameter}\n";
        $model->net->attributes()['id'] = "{$name}-{$type}-{$parameter}";
      }
      else
      {
        echo "  {$instancename} does not respect the pattern NAME-(COL|PT)-PARAMETER.\n";
        exit(1);
      }
    }
    $name = $model->net->name;
    if ($name == NULL)
    {
      echo "  Fixing name to {$model->net->attributes()['id']}.\n";
      $model->net->addChild('name');
      $model->net->name->addChild('text', "{$model->net->attributes()['id']}");
    }
    else if ((string) $name->text != $model->net->attributes()['id'])
    {
      echo "  Fixing name from {$name->text} to {$model->net->attributes()['id']}.\n";
      $name->text = "{$model->net->attributes()['id']}";
    }
    //
    $replacements = array();
    // Now, fix place ids and names;
    foreach ($model->net->page->place as $place)
    {
      $id = (string) $place->attributes()['id'];
      $name = (string) $place->name->text;
      if (($id == NULL) && ($name == NULL))
      {
        echo "  Missing both place id and name.\n";
        exit(1);
      }
      if ($id == NULL)
      {
        echo "  Fixing missing place id for ${name}.\n";
        $id = $this->identifier_of($name);
        $place->addAttribute('id', $id);
      }
      if ($name == NULL)
      {
        echo "  Fixing missing place name for ${id}\n";
        $place->addChild('name');
        $name = $place->attributes()['id'];
        $place->name->addChild('text', $name);
      }
      if (levenshtein($id, $name) >= min(strlen($id), strlen($name))/2)
      {
        $new = $this->identifier_of($name);
        echo "  Fixing ugly place id for ${id} to ${new}.\n";
        $replacements[$id] = $new;
        $place->attributes()['id'] = $new;
      }
    }
    // The same for transitions:
    foreach ($model->net->page->transition as $transition)
    {
      $id = (string) $transition->attributes()['id'];
      $name = (string) $transition->name->text;
      if (($id == NULL) && ($name == NULL))
      {
        echo "  Missing both transition id and name.\n";
        exit(1);
      }
      if ($id == NULL)
      {
        echo "  Fixing missing transition id for ${name}\n";
        $id = $this->identifier_of($name);
        $transition->addAttribute('id', $id);
      }
      if ($name == NULL)
      {
        echo "  Fixing missing transition name for ${id}.\n";
        $transition->addChild('name');
        $name = $transition->attributes()['id'];
        $transition->name->addChild('text', $name);
      }
      if (levenshtein($id, $name) > min(strlen($id), strlen($name))/2)
      {
        $new = $this->identifier_of($name);
        echo "  Fixing ugly transition id for ${id}} to ${new}.\n";
        $replacements[$id] = $new;
        $transition->attributes()['id'] = $new;
      }
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
          echo "  Fixing arc ${id} source from ${source} to ${r}.\n";
          $arc->attributes()['source'] = $r;
        }
        $target = (string) $arc->attributes()['target'];
        if (array_key_exists($target, $replacements))
        {
          $r = $replacements[$target];
          echo "  Fixing arc ${id} target from ${target} to ${r}.\n";
          $arc->attributes()['target'] = $r;
        }
      }
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
