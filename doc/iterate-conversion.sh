#! /bin/sh

EXEC=./txt-to-xml

LIST_FILE="txtprop-list"
LOCK_FILE="txtprop-lock"

rm -f $LOCK_FILE

(for x in TMP_PROP/*/*.txt ; do echo $x ; done) > $LIST_FILE


ENDED=0

# Launch N processes:
N=24
function generate()
{
  local PID=$1
  echo "Starting $PID"
  rm -f log.$PID
  while [ 1 -eq 1 ]
  do
    # Get line:
    lockfile $LOCK_FILE
    if [ ! -f $LIST_FILE ]
    then
      echo "End of process $PID"
      ENDED=`expr $ENDED + 1`
      rm -f $LOCK_FILE
      exit 0
    fi
    FILE=`head -n 1 $LIST_FILE`
    SIZE=`cat $LIST_FILE | wc -l | sed -e 's/^[ \t]*//'`
    if [ $SIZE -eq 1 ]
    then
      rm -f $LIST_FILE
    else
      tail -n `expr $SIZE - 1` $LIST_FILE > .tmp
      mv .tmp $LIST_FILE
    fi
    rm -f $LOCK_FILE
    # Perform conversion:
    echo "$FILE" >> log.$PID
    $EXEC $FILE $FILE.xml  >> log.$PID
  done
}

for i in `seq 1 $N`
do
  generate $i &
done
exit 0

for i in `seq 1 $N`
do
  wait
done
