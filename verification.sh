CATS="ReachabilityFireability
ReachabilityCardinality
UpperBounds
LTLFireability
LTLCardinality
CTLFireability
CTLCardinality"

if [ $# != 1 ]
then
  echo Usage $0 \<BenchKit\>
  exit 4
fi

cd $1/INPUTS

verifFile()
{
  if ! [ -f $1 ]
  then
    echo File missing: $1
    exit 1
  fi
}

verifCategory()
{
  verifFile $1/$2.xml
  verifFile $1/$2.txt

  if ! [ $(grep Property $1/$2.txt | wc -l) == 16 ]
  then
    echo File incomplete: $1/$2.txt
    exit 2
  fi

  if ! [ $(grep formula $1/$2.xml | wc -l) == 32 ]
  then
    echo File incomplete: $1/$2.xml
    exit 3
  fi
}



for name in *
do
  echo $name
  
  for cat in $CATS
  do
    verifCategory $name $cat
  done
done