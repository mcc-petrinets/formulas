
P possible
N impossible
I invariant


CTLCardinality.xml
CTLFireability.xml
CTLFireabilitySimple.xml
LTLCardinality.xml

ReachabilityDeadlock.xml
====================

- possible (deadlock)
- impossible (deadlock)


ReachabilityFireabilitySimple.xml
===========

- possible (is-fireable (t1, ..., tn))

ReachabilityFireability.xml
=============

F :=
    possible (A)
  | impossible (A)
  | invariant (A)

A :=
    is-firable (t1, ..., tn)
 
ReachabilityCardinality.xml
=================

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
  | A ????????????

A :=
    I = I
  | I <= I
  | I >= I
  | I < I
  | I > I
  | I

I :=
    tokens-count (p1, ..., pn)

ReachabilityBounds.xml
==============

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

ReachabilityComputeBounds.xml
==========================

place-bound (p1, ..., pn)


LTLFireabilitySimple.xml
============

F :=
    G A
  | F A
  | X_{steps=1,if-no-successor=false} A
  | A U_{strength=strong} A

A :=
    is-fireable (t1, ..., tn)

LTLFireability.xml
============

Continue here !!
