# dhps-lib
Generic algorithms and datastructures for C.

### datastructures
- [ ] dynamic hashtable
- [x] dynamic array / list
- [x] heap queue
  - [x] heap sort -> sort.c
- [x] circular queue
- [x] stack
- [ ] string
- [ ] linked list
  - [ ] singly linked
  - [ ] doubly linked
- [ ] binary tree

### algorithms
- [x] insertion sort
- [x] quicksort
- [ ] prime number sieve
- [ ] binary search

### graphs and functions
- [ ] graph structure
- [ ] dijkstra's algorithm
- [ ] bellman ford
- [ ] prim's algorithm
- [ ] breadth first search
- [ ] depth first search
- [ ] topological sorting

### Naming conventions and semantics
Macro's:
  - All caps name.
  - Multiline do ... while(0)

Functions:
  - Static functions are prefixed with "\__".
  - Newline for first bracket.

Structs:
  - Snake case name suffixed with a "\_t"
  - No typedef

General:
  - Use booleans
  - Follow this, mostly: https://userpages.umbc.edu/~rostamia/cbook/CodingStyle.html.
