#!/bin/bash

MAX=2000
ROOT=inputs-${MAX}states

CATS="ReachabilityDeadlock
ReachabilityFireability
ReachabilityCardinality
UpperBounds
LTLFireability
LTLCardinality
CTLFireability
CTLCardinality"

function main ()
{
	echo "find(1) on the root directory:"
	find $ROOT
	echo
	echo

	for C in $CATS; do
		NR=16
		if test "$C" == "ReachabilityDeadlock"; then
			NR=1
		fi
		echo ============================
		echo "Category '$C'" depth=7 quantity=$NR
		date
		CMD="./mcc formula:generate --chain --subcategory=$C $ROOT --quantity=$NR --smc-max-states=$MAX --depth=7"
		echo $CMD
		time $CMD
		echo "End of category '$C' exit status $?"
	done
	echo "END !!!!"
	date
}

main > output-${MAX}states.txt 2>&1

