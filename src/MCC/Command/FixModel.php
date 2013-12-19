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
        echo "  Fixing missing place id for {$place->attributes()['name']}.\n";
        $place->addAttribute('id', $this->identifier_of($name));
      }
      else if (levenshtein($id, $name) > 10)
      {
        $new = $this->identifier_of($name);
        echo "  Fixing ugly place id for {$place->attributes()['id']} to ${new}.\n";
        $place->attributes()['id'] = $new;
      }
      if ($name == NULL)
      {
        echo "  Fixing missing place name for {$place->attributes()['id']}\n";
        $place->addChild('name');
        $place->name->addChild('text', "{$place->attributes()['id']}");
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
        echo "  Fixing missing transition id for {$transition->attributes()['name']}\n";
        $transition->addAttribute('id', $this->identifier_of($name));
      }
      else if (levenshtein($id, $name) > 10)
      {
        $new = $this->identifier_of($name);
        echo "  Fixing ugly transition id for {$transition->attributes()['id']} to ${new}.\n";
        $transition->attributes()['id'] = $new;
      }
      if ($name == NULL)
      {
        echo "  Fixing missing transition name for {$transition->attributes()['id']}.\n";
        $transition->addChild('name');
        $transition->name->addChild('text', "{$transition->attributes()['id']}");
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
