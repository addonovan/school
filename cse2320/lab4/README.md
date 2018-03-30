CSE 2320 Lab #4
===

There's unfortanately no fun context for this lab, :(.

Edges of a graph are given as input along with their weights, in the form 
`finish start value`, where `start` and `finish` are both strings. These
edges must be used to generate a hashmap of all the nodes in the graph,
and then a minimal spanning tree shall be generated using the nodes and edges.

Constraints:
- Neither `finish` nor `start` will ever be more than 25 characters
- Kruskal's algorithm must be employed to generate the MST
- The hash map must employ double hashing

