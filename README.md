# ICPC-2018-The-Comma-Sprinkler-Solution
This is my solution for an assignment I got in my Computer Systems and Software Design class. The assignment is based on the ICPC 2018: The Comma Sprinkler problem (detailed problem description: https://judge.icpc.global/problems/comma).

Problem summary: add commas to a piece of text according to the following rules.
1. If a word anywhere in the text is preceded by a comma, find all occurrences of that word in the text, and put a comma before each of those occurrences, except in the case where such an occurrence is the first word of a sentence or already preceded by a comma.
2. If a word anywhere in the text is succeeded by a comma, find all occurrences of that word in the text, and put a comma after each of those occurrences, except in the case where such an occurrence is the last word of a sentence or already succeeded by a comma.
3. Apply rules 1 and 2 repeatedly until no new commas can be added using either of them.

I used a directed graph to solve this problem. Each word in the text is a vertex in the graph. Between each word in the text is either a space, a comma followed by a space, or a period followed by a space so the edge between any two vertices in the graph holds the punctuation (comma or period) or space that connects the words in those two vertices. The direction of the edge indicates the order in which the words appear in the text.

I implemented my solution in C++, using a class to encapsulate the graph and a hashmap to implement the graph. In the hashmap, each key is a string (a word from the text) and each value is a struct that stores all edges (comma, period, space) and the corresponding adjacent vertices of the current vertex. The edges are stored in three vectors: one for incoming edges that hold a comma or a space (words preceding the current word), one for outgoing edges that hold a comma or a space (words succeeding the current word) and one for edges (whether incoming or outgoing) that hold a period.

To apply the comma sprinkler rules, if a vetex has any edge that holds a comma, all its edges (incoming and outgoing) that hold a space are changed to hold a comma. This is done for every vertex starting from the beginning of the graph. The process is repeated until there are no more edges to change. After applying the rules, the piece of text is output into a new text file. 

The assignment gave eight test text files on which to apply the rules and eight corresponding text files containing the result of applying the Comma Sprinkler rules. My solution was required to compare my result text file to the given result file.
