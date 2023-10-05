# Pilar Language

Pilar is a stack-based language for simple operations. 
It supports scopes for each block of instructions: 
symbols created at the block dissapear when the control moves to a block from of a previous scope.

These sketches depict the overall idea (sorry for the non-artistic strokes :)):

[Stack and Symbol Map](https://github.com/paralelodev/pilar/blob/main/doc/StackAndSymbols.jpg)

[CFG and Symbol Map](https://github.com/paralelodev/pilar/blob/main/doc/CFGAndSymbols.jpg)

## Example

This is a code sample in Pilar syntax:

```c
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
```c
Block([Assign("a", IntLiteral(1)),
       Assign("b", IntLiteral(0)),
       If(BinaryOperation(Operators::EQUAL, Fetch("a"), Fetch("b")),
           Print(IntLiteral(0)), Block([]));
       If(BinaryOperation(Operators::GREATER, Fetch("a"), Fetch("b")),
           Print(IntLiteral(1)), Block([]));
       Print I4(BinaryOperation(Operators::ADD, Fetch("a"), Fetch("b")))])
```

Which corresponds to this pseudocode:
```c
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

```bash
cmake -G Ninja -B build .
cd build
ninja
```

## Usage

### Convert DAG to Pilar syntax
```bash
${PILAR_BUILD}/test/dag_002 > example_002.plr
```
[Click here to see the source code for dag_002.cpp](https://github.com/paralelodev/pilar/blob/main/test/dag_002.cpp)

### Run a Pilar (.plr) file with the pilarc frontend
```bash
${PILAR_BUILD}/src/pilarc example_002.plr
```

### Build a Pilar program in memory (experimental)
Programs can be built without the Pilar frontend by using directly the Pilar library:
```bash
${PILAR_BUILD}/test/test_002
```
[Click here to see the source code for test_002.cpp](https://github.com/paralelodev/pilar/blob/main/test/test_002.cpp)

## Test

```bash
${PILAR_BUILD}/test/dag_002 > example_002.plr
diff example_002.plr ${PILAR_SRC}/test/example_002.plr # no difference
${PILAR_BUILD}/src/pilarc example_002.plr # outputs 5 and 9
${PILAR_BUILD}/src/pilarc ${PILAR_SRC}/test/example_002.plr  # outputs 5 and 9
${PILAR_BUILD}test/test_002  # outputs 5 and 9

${PILAR_BUILD}/test/dag_003 > example_003.plr
diff example_003.plr ${PILAR_SRC}/test/example_003.plr # no difference
${PILAR_BUILD}/src/pilarc example_003.plr # outputs 1 and 1
${PILAR_BUILD}/src/pilarc ${PILAR_SRC}/test/example_003.plr # outputs 1 and 1
${PILAR_BUILD}/test/test_003 # outputs 1 and 1
```
