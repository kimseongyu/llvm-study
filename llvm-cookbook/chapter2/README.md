### Toy Grammar

```
numeric_expr    := number

paran_expr      := '(' expression ')'

identifier_expr := identifier
                := identifier '(' expr_list ')'

expr_list       := (empty)
                := expression (',' expression)*

primary         := identifier_expr
                := numeric_expr
                := paran_expr

expression      := primary binoprhs

binoprhs        := (binoperator primary)*
binoperator     := '+' / '-' / '*' / '/'

func_decl       := identifier '(' identifier_list ')'
identifier_list := (empty)
                := (identifier)*

function_defn   := 'def' func_decl expression

toplevel_expr   := expression
```
