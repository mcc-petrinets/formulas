#!/bin/bash

ROOT=../inputs-known-2015/

CATS="ReachabilityDeadlock
ReachabilityFireabilitySimple
ReachabilityFireability
ReachabilityCardinality
ReachabilityBounds
ReachabilityComputeBounds
LTLFireabilitySimple
LTLFireability
LTLCardinality
CTLFireabilitySimple
CTLFireability
CTLCardinality"

function main ()
{
	for C in $CATS; do
		echo ============================
		echo "Category '$C'" depth=7
		date
		CMD="./mcc formula:generate --subcategory=$C $ROOT --quantity=16 --smc-max-states=10 --depth=7"
		echo $CMD
		time $CMD
		echo Exit status: $?
	done

	echo
	echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	echo "First run (10 states) done"
	date
	echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	echo

	for C in $CATS; do
		echo ============================
		echo "Category '$C'" depth=7
		date
		CMD="./mcc formula:generate --subcategory=$C $ROOT --quantity=16 --smc-max-states=1000 --depth=7"
		echo $CMD
		time $CMD
		echo Exit status: $?
	done

	echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	echo "Second run, with 1000 states, done"
	date
	echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
}

main > output.txt 2>&1

