
all :
	echo "nothing to do :)"

generate_no_filtering :
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityDeadlock --quantity 1 --no-ansi
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityFireabilitySimple
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityFireability
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityCardinality
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityBounds
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory ReachabilityComputeBounds
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory LTLFireabilitySimple
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory LTLFireability
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory LTLCardinality
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory CTLFireabilitySimple
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory CTLFireability
	./mcc formula:generate inputs-smallest --quantity 9 --chain --no-filtering --subcategory CTLCardinality
