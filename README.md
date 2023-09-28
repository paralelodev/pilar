# Pilar Language

Pilar is a stack-based language for simple operations. E.g.:

```
.entry
push 1
print
goto .end

.end
push 4
print
```

Outputs:

```
1
4
```

## Build Instructions

```
cmake -G Ninja -B build .
cd build
ninja
```

## Usage

```
test/test_001
```
