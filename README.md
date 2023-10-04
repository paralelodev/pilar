# Pilar Language

Pilar is a stack-based language for simple operations. 

## Example

This is a code sample in Pilar syntax:

```
.entry
push 1
store r0
push 0
store r1
load r0
load r1
==
choose .then0 .continue0

.then0
push 0
print
goto .continue0

.continue0
load r0
load r1
>
choose .then1 .continue1

.then1
push 1
print
goto .continue1

.continue1
load r0
load r1
+
print
goto .exit

.exit
```

It outputs:

```
1
1
```

It corresponds to this DAG:
```
Block([Assign("a", IntLiteral(1)),
       Assign("b", IntLiteral(0)),
       If(BinaryOperation(Operators::EQUAL, Fetch("a"), Fetch("b")),
           Print(IntLiteral(0)), Block({}));
       If(BinaryOperation(Operators::GREATER, Fetch("a"), Fetch("b")),
           Print(IntLiteral(1)), Block([]));
       Print I4(BinaryOperation(Operators::ADD, Fetch("a"), Fetch("b")))])
```

Which corresponds to this pseudocode:
```
a = 1
b = 0
if b == a {
    print 0
}
if a > b {
    print 1
}
print a + b
```

## Build Instructions

```
cmake -G Ninja -B build .
cd build
ninja
```

## Test

```
<Pilar build folder>/test/dag_002 > example_002.plr
diff example_002.plr <Pilar sources folder>/test/example_002.plr # no difference
<Pilar build folder>/src/pilarc example_002.plr # outputs 5 and 9
<Pilar build folder>/src/pilarc <Pilar sources folder>/test/example_002.plr  # outputs 5 and 9
<Pilar build folder>test/test_002  # outputs 5 and 9

<Pilar build folder>/test/dag_003 > example_003.plr
diff example_003.plr <Pilar sources folder>/test/example_003.plr # no difference
<Pilar build folder>/src/pilarc example_003.plr # outputs 1 and 1
<Pilar build folder>/src/pilarc <Pilar sources folder>/test/example_003.plr # outputs 1 and 1
<Pilar build folder>/test/test_003 # outputs 1 and 1
```
