# knobloc
dumb memory allocator
# properties
- fixed block size
- header only implementation
- O(1) allocation
- O(1) free
- minimal administrative memory required
- user provided initial pool
# implementation
simple double linked ring list with linked in header to eliminate edge cases for insertion and removal
header only, no need to link to anything
# warning
example only, no real use case known for this type of allocator

