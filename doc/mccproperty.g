grammar mccproperty { backtrack=true; };

IDENTIFIER:
  ('a'..'z'|'A'..'Z'|'0'..'9'|'_')+;

INTEGER:
  '0'..'9'+;

EOL:
  ('\r' | '\n')+;

property_set:
  property+
  ;

property:
  ( 'reach' | 'struct' | 'ctl' | 'ltl' )? IDENTIFIER ':' complex_formula EOL
  ;

simple_formula:
    'true'
  | 'false'
  | '(' complex_formula ')'
  | '!' simple_formula
  | 'I' simple_formula
  | 'N' simple_formula  
  | 'A' simple_formula
  | 'E' simple_formula
  | 'X' INTEGER? '~'? simple_formula
  | 'G' simple_formula
  | 'F' simple_formula
  | 'deadlock'
  | IDENTIFIER '?' ('l0' | 'l1' | 'l2' | 'l3' | 'l4')?
	| complex_expression
		('='
   	| '!='
    | '<'
    | '<='
    | '>'
    | '>='
    ) complex_expression
  ;

complex_formula:
    simple_formula ( ('&'   simple_formula)+
    							 | ('|'   simple_formula)+
    							 | ('xor' simple_formula)+
    							 | ('=>'  simple_formula)
    							 | ('<=>' simple_formula)
    							 | ('U'   simple_formula)
    							 | ('W'   simple_formula)
    							 )?
  ;

complex_expression:
		simple_expression
		( ('+' simple_expression)+
    | ('*' simple_expression)+
    | ('-' simple_expression)
    | ('/' simple_expression)
    )?
	;

simple_expression:
    INTEGER
  | 'bound' '(' IDENTIFIER ')'
  | '|' complex_expression '|'
  | '<' anonymous_bindings '>'
  | 'marking' '(' IDENTIFIER ')'
  | '(' complex_expression ')'
  ;

anonymous_bindings:
    ( IDENTIFIER (',' IDENTIFIER)* )?
  ;