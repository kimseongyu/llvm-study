### Toy Grammar

```
numeric_expr    := number

identifier_expr := identifier
                := identifier '(' expr_list ')'

expr_list       := (empty)
                := expression (',' expression)*

paran_expr      := '(' expression ')'

if_expr         := 'if' expression  'then' expression 'else' expression

for_expr        := 'for' identifier '=' expression ',' expression (',' expression)? 'in' expression

primary         := numeric_expr
                := identifier_expr
                := paran_expr
                := if_expr
                := for_expr

unary_expr      := primary
                := unary_operator unary_expr

binoprhs        := (binoperator primary)*
binoperator     := '+' / '-' / '*' / '/' / '<'

expression      := unary_expr binoprhs

func_decl       := identifier '(' identifier_list ')'
                := 'unary' ascii_char '(' identifier_list ')'
                := 'binary' ascii_char numeric_precedence? '(' identifier_list ')'

numeric_precedence := number

identifier_list := (empty)
                := (identifier)*

function_defn   := 'def' func_decl expression

toplevel_expr   := expression
```
