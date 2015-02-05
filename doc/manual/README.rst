
 - intro
 - grammaire BNF
 - ejemplos
 - categorias
 - relacion con RNG: xml ejemplo, antiguo manual

 w encontrar el fragmento

 - introduccion:
   - formulas generadas aleatoriamente
   - que pasos hemos utilizado para filtrar ??
   - es dificil general formulas
   - nos gustaria producirlas a mano, pero son muchos modelos, pedimos ayuda para el año que viene
   - 

P possible
N impossible
I invariant

ReachabilityDeadlock.xml
====================

with template ::
  ./mcc formula:instantiate --chain --pattern-file=TemplateReachabilityDeadlock.xml --output=ReachabilityDeadlock --prefix=ReachabilityDeadlock /tmp/MCC/BenchKit/INPUTS"

- possible (deadlock)
- impossible (deadlock)

- FIXME verify this ...

ReachabilityFireabilitySimple.xml
===========

franchissabilite d'une seule transition::
  ./mcc formula:instantiate --chain --pattern-file=TemplateReachabilityFireabilitySimple.xml --output=ReachabilityFireabilitySimple --prefix=ReachabilityFireabilitySimple /tmp/MCC/BenchKit/INPUTS"

- possible (is-fireable (t1, ..., tn))

- I don't have the TemplateReachabilityFireabilitySimple, so I cannot verify ...

ReachabilityFireability.xml
=============

franchissabilite avec combinaison (mais ici profondeur 1)::
  --reachability --fireability --boolean --type=boolean --depth=1

F :=
    possible (A)
  | impossible (A)
  | invariant (A)

A :=
    is-firable (t1, ..., tn)

- VERIFIED
 
ReachabilityCardinality.xml
=================

formules d'accessibilité avec cardinalités seulement + op booleens::
  --reachability --cardinality --integer --boolean --type=boolean

F :=
    possible (B)
  | impossible (B)
  | invariant (B)

B :=
    not A
  | A and A
  | A or A
  | A xor A
  | A -> A
  | A <-> A
  | A

A :=
    I = I
  | I <= I
  | I >= I
  | I < I
  | I > I
  | I

I :=
    tokens-count (p1, ..., pn)

- VERIFIED

ReachabilityBounds.xml
==============

bound + op boleens + additions::
  --output=ReachabilityBounds --prefix=ReachabilityBounds --quantity=$NUMBER_OF_FORMULA --bound --boolean --integer --type=boolean /tmp/MCC/BenchKit/INPUTS"

B :=
    not C
  | C and C
  | C or C
  | C xor C
  | C -> C
  | C <-> C
  | C

C :=
    not A
  | A and A
  | A or A
  | A xor A
  | A -> A
  | A <-> A
  | A

A :=
    E = E
  | E <= E
  | E >= E
  | E < E
  | E > E
  | E

E :=
    I + I
  | I - I
  | I

I :=
    place-bound (p1, ..., pn)

- VERIFIED

ReachabilityComputeBounds.xml
==========================

version simple pour les bornes::
  ./mcc formula:generate --chain --output=ReachabilityComputeBounds --prefix=ReachabilityComputeBounds --quantity=$NUMBER_OF_FORMULA --bound --type=integer /tmp/MCC/BenchKit/INPUTS

place-bound (p1, ..., pn)

- VERIFIED

LTLFireabilitySimple.xml
============

franchissabilite d'une seule transition (pas vrai !) ::
  ./mcc formula:generate --chain --output=LTLFireabilitySimple --prefix=LTLFireabilitySimple --quantity=$NUMBER_OF_FORMULA --ltl --fireability --type=boolean --depth=2 /tmp/MCC/BenchKit/INPUTS

F :=
    G A
  | F A
  | X_{steps=1,if-no-successor=false} A
  | A U_{strength=strong} A

A :=
    is-fireable (t1, ..., tn)

- FIXME do we want this is-firable () to have more than one transition ?
- VERIFIED

LTLFireability.xml
============

franchissabilite avec combinaison (mais ici profondeur 1) ::
  ./mcc formula:generate --chain --output=LTLFireability --prefix=LTLFireability --quantity=$NUMBER_OF_FORMULA --ltl --fireability --boolean --type=boolean --depth=1 /tmp/MCC/BenchKit/INPUTS

Continue here !!

F :=
    G A
  | F A
  | X_{steps=1,if-no-successor=false} A
  | A U_{strength=strong} A

A :=
    is-fireable (t1, ..., tn)

- FIXME by mistake last year this was exactly the same as LTLFireability,
  what this category should contain?
- VERIFIED

LTLCardinality.xml
==================

formules LTL avec cardinalités seulement + op booleens ::
  ./mcc formula:generate --chain --output=LTLCardinality --prefix=LTLCardinality --quantity=$NUMBER_OF_FORMULA --ltl --cardinality --integer --boolean --type=boolean /tmp/MCC/BenchKit/INPUTS

B :=
    not C
  | C and C
  | C or C
  | C xor C
  | C -> C
  | C <-> C
  | G C
  | F C
  | X_{steps=1,if-no-successor=false} C
  | C U_{strength=strong} C

C :=
    not A
  | A and A
  | A or A
  | A xor A
  | A -> A
  | A <-> A
  | G A
  | F A
  | X_{steps=1,if-no-successor=false} A
  | A U_{strength=strong} A
  | A

A :=
    I = I
  | I <= I
  | I >= I
  | I < I
  | I > I

I :=
    tokens-count (p1, ..., pn)

- You can have more than one place in `tokens-count`, checked
- VERIFIED

CTLFireabilitySimple.xml
===================

franchissabilite d'une seule transition ::
  --ctl --fireability --type=boolean --depth=2

F :=
    A T
  | E T

T :=
    G A
  | F A
  | X_{steps=1,if-no-successor=false} A
  | A U_{strength=strong} A

A :=
    is-fireable (t1, ..., tn)

- You can have more than one transition in `is-firable`, checked
- VERIFIED

CTLFireability.xml
===============

franchissabilite avec combinaison (mais ici profondeur 1) ::
  --ctl --fireability --boolean --type=boolean --depth=1

F :=
    A T
  | E T

T :=
    G A
  | F A
  | X_{steps=1,if-no-successor=false} A .. confirmar aqui FIXME
  | A U_{strength=strong} A

A :=
    is-fireable (t1, ..., tn)

- You can have more than one transition in `is-firable`, checked
- FIXME there is no difference with CTLFireabilitySimple !!!
- FIXME what should we produce here ?
- VERIFIED

CTLCardinality.xml
================

formules CTL avec cardinalités seulement + op booleens::
  --ctl --cardinality --integer --boolean --type=boolean

B :=
    not C
  | C and C
  | C or C
  | D

C :=
    not D
  | D and D
  | D or D
  | D

D :=
    A T
  | E T

T :=
    G H
  | F H
  | X_{steps=1,if-no-successor=false} H
  | H U_{strength=strong} H

H :=
    B
  | I <= I

I :=
    integer constant in [0, 1, 2]
  | tokens-count (p1, ..., pn)

- words produced by this grammar such that the derivation of the word
substitutes `at most two times the non-terminal B by any of its
productions`.

- the grammar fragment generated in this category for the 2014 edition was
different than this and difficult to characterize; for 2015 I propose this
fragment, easier to characterize. We can modify the "formula:generate"
command to produce formulas of this shape.

- in particular, in 2014, this fragment was like this: the root node of the
formula was a CTL operator or a boolean operator; the second level was CLT,
boolean operator or a integer comparison. If the first two levels were
boolean operators, the third was a CTL operator; in any other case, the
third level was an integer comparison. If the third level was a CTL
operator, the fourth was an integer comparison.
The integer expresions of an integer comparison were `token-count`
expressions. I just modified the tool so that it could now also produce an
integer constant in the left-hand size of an integer comparison.
