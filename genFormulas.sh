#!/bin/bash

display_usage () {
	echo "usage: $0 [-p] <val>"
	echo "   p : perform tasks in parallel"
	echo "   <val>: the number of formula of each type to be generated"
	echo "          (except for deadlock formulas)"
}

TEMPLATE_XML_DEADLOCK=$(pwd)"/TemplateReachabilityDeadlock.xml"
TEMPLATE_TXT_DEADLOCK=$(pwd)"/TemplateReachabilityDeadlock.txt"

if [ "$1" = "-p" ] ; then
	PARALLEL="YES"
	echo "parallel mode activated"
	shift
fi

if [ $# -ne 1 ] ; then
	display_usage
	exit 1
else
	NUMBER_OF_FORMULA=$1
fi

echo "==============================================="
echo "Generating Formulas"
echo
# echo "  Deadlock formulas"
# (cd /tmp/MCC/BenchKit/INPUTS
# for M in * ; do
# 	echo -n "."
# 	cd $M
# 	# ajout des formules fixes sur le deadlock
# 	sed -e "s/__MODEL__/$M/g" "$TEMPLATE_XML_DEADLOCK" > ReachabilityDeadlock.xml
# 	sed -e "s/__MODEL__/$M/g" "$TEMPLATE_TXT_DEADLOCK" > ReachabilityDeadlock.txt
# 	cd ..
# done
# echo)

# deadlocks...
CMD0="./mcc formula:instantiate --chain --pattern-file=TemplateReachabilityDeadlock.xml --output=ReachabilityDeadlock --prefix=ReachabilityDeadlock /tmp/MCC/BenchKit/INPUTS"
# version simple pour les bornes...
CMD1="./mcc formula:generate --chain --output=ReachabilityComputeBounds --prefix=ReachabilityComputeBounds --quantity=$NUMBER_OF_FORMULA --bound --type=integer /tmp/MCC/BenchKit/INPUTS"
# bound + op boleens + additions
CMD2="./mcc formula:generate --chain --output=ReachabilityBounds --prefix=ReachabilityBounds --quantity=$NUMBER_OF_FORMULA --bound --boolean --integer --type=boolean /tmp/MCC/BenchKit/INPUTS"
# formules d'accessibilité avec cardinalités seulement + op booleens
CMD3="./mcc formula:generate --chain --output=ReachabilityCardinality --prefix=ReachabilityCardinality --quantity=$NUMBER_OF_FORMULA --reachability --cardinality --integer --boolean --type=boolean /tmp/MCC/BenchKit/INPUTS"
# franchissabilite d'une seule transition
CMD4="./mcc formula:instantiate --chain --pattern-file=TemplateReachabilityFireabilitySimple.xml --output=ReachabilityFireabilitySimple --prefix=ReachabilityFireabilitySimple /tmp/MCC/BenchKit/INPUTS"
# franchissabilite avec combinaison (mais ici profondeur 1)
CMD5="./mcc formula:generate --chain --output=ReachabilityFireability --prefix=ReachabilityFireability --quantity=$NUMBER_OF_FORMULA --reachability --fireability --boolean --type=boolean --depth=1 /tmp/MCC/BenchKit/INPUTS"

if [ "$PARALLEL" ] ; then
	echo "  Reachability formulas in parallel (1/2)"
	$CMD0 > /dev/null &
	$CMD1 > /dev/null &
	$CMD2 > /dev/null &
	wait
	echo "  Reachability formulas in parallel (2/2)"
	$CMD3 > /dev/null &
	$CMD4 > /dev/null &
	$CMD5 > /dev/null &
	wait
else
	echo "  Reachability formulas in sequential"
	$CMD0
	$CMD1
	$CMD2
	$CMD3
	$CMD4
	$CMD5
fi

# formules CTL avec cardinalités seulement + op booleens
CMD1="./mcc formula:generate --chain --output=CTLCardinality --prefix=CTLCardinality --quantity=$NUMBER_OF_FORMULA --ctl --cardinality --integer --boolean --type=boolean /tmp/MCC/BenchKit/INPUTS"
# franchissabilite d'une seule transition
CMD2="./mcc formula:generate --chain --output=CTLFireabilitySimple --prefix=CTLFireabilitySimple --quantity=$NUMBER_OF_FORMULA --ctl --fireability --type=boolean --depth=2 /tmp/MCC/BenchKit/INPUTS"
# franchissabilite avec combinaison (mais ici profondeur 1)
CMD3="./mcc formula:generate --chain --output=CTLFireability --prefix=CTLFireability --quantity=$NUMBER_OF_FORMULA --ctl --fireability --boolean --type=boolean --depth=1 /tmp/MCC/BenchKit/INPUTS"

if [ "$PARALLEL" ] ; then
	echo "  CTL formulas in parallel"
	$CMD1 > /dev/null &
	$CMD2 > /dev/null &
	$CMD3 > /dev/null &
	wait
else
	echo "  CTL formulas in sequential"
	$CMD1
	$CMD2
	$CMD3
fi

# formules LTL avec cardinalités seulement + op booleens
CMD1="./mcc formula:generate --chain --output=LTLCardinality --prefix=LTLCardinality --quantity=$NUMBER_OF_FORMULA --ltl --cardinality --integer --boolean --type=boolean /tmp/MCC/BenchKit/INPUTS"
# franchissabilite d'une seule transition
CMD2="./mcc formula:generate --chain --output=LTLFireabilitySimple --prefix=LTLFireabilitySimple --quantity=$NUMBER_OF_FORMULA --ltl --fireability --type=boolean --depth=2 /tmp/MCC/BenchKit/INPUTS"
# franchissabilite avec combinaison (mais ici profondeur 1)
CMD3="./mcc formula:generate --chain --output=LTLFireability --prefix=LTLFireability --quantity=$NUMBER_OF_FORMULA --ltl --fireability --boolean --type=boolean --depth=1 /tmp/MCC/BenchKit/INPUTS"

if [ "$PARALLEL" ] ; then
	echo "  LTL formulas in parallel"
	$CMD1 > /dev/null &
	$CMD2 > /dev/null &
	$CMD3 > /dev/null &
	wait
else
	echo "  LTL formulas in sequential"
	$CMD1
	$CMD2
	$CMD3
fi
