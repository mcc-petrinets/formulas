<?php
namespace MCC\Command;

use \Symfony\Component\Console\Input\InputInterface;
use \Symfony\Component\Console\Output\OutputInterface;
use \Symfony\Component\Console\Input\InputOption;
use \MCC\Command\Base;

class CleanFormulas extends Base
{

  protected function configure()
  {
    $this
      ->setName('formula:clean')
      ->setDescription('Clean formula files')
      ->addOption('output', null,
        InputOption::VALUE_REQUIRED,
        'File name for formulas output', 'formulas')
        ;
    parent::configure();
  }

  private $output = null;
  private $extensions = array('xml', 'txt');

  protected function pre_perform(InputInterface $input, OutputInterface $output)
  {
    $this->output = $input->getOption('output');
  }

  protected function generated_files()
  {
    return array(
      'model-fix.log',
      'model-check.log',
      "{$this->output}.xml",
      "{$this->output}.txt",
    );
  }

  protected function perform()
  {
    $files = $this->generated_files();
    foreach (array( $this->sn_file, $this->pt_file ) as $f)
    {
      $quantity = count($files);
      $this->progress->setRedrawFrequency($quantity);
      $this->progress->start($this->console_output, $quantity);
      $dir = dirname($f);
      foreach ($files as $filename)
      {
        $file = "{$dir}/{$filename}";
        if (file_exists($file))
        {
          unlink($file);
        }
        $this->progress->advance();
      }
      $this->progress->finish();
    }
  }

}
