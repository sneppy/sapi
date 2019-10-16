#pragma once

/**
 * C structure used to represent an edge node
 */
struct EdgeNode
{
	// Parent node
	struct EdgeNode * parent;

	// First children
	struct EdgeNode * first;

	// Last children
	struct EdgeNode * last;

	// Right sibling (i.e. next child)
	struct EdgeNode * next;

	// String value
	char * value;
};
typedef struct EdgeNode EdgeNode;

/**
 * Create a new edge node
 */
extern EdgeNode * make_edge();

/**
 * Release edge node
 * 
 * @param [in] edge edge to deallocate
 */
extern void free_edge(EdgeNode * edge);

/**
 * Append node as child to edge
 * 
 * @param [in] child child edge
 * @return ptr to upper node
 */
extern EdgeNode * edge_add_child(EdgeNode * edge, EdgeNode * child);

/**
 * Parse string starting from src and
 * edge. When a opening parenthesis is
 * encountered we recursively invoke
 * the function on the newly created
 * edge
 * 
 * @param [in] src source edges string
 * @param [in] root edge subtree root node
 */
extern EdgeNode * parse_edge_str(char ** src, EdgeNode * root);