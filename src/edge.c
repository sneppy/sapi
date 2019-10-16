#include <stdlib.h>

#include <edge.h>

EdgeNode * make_edge()
{
	EdgeNode * new_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
	if (new_edge)
	{
		// Init edge
		new_edge->parent = NULL;
		new_edge->first = NULL;
		new_edge->last = NULL;
		new_edge->next = NULL;

		return new_edge;	
	}
	else return NULL;
}

void dispose_edge(EdgeNode * edge)
{
	// Sanity check
	if (!edge) return;

	// Recursively dispose children
	EdgeNode * it = edge->first; for (; it; it = it->next) dispose_edge(it);

	// Finally dispose self
	free(edge);
}

EdgeNode * edge_add_child(EdgeNode * edge, EdgeNode * child)
{
	if (edge)
	{
		// Don't add empty child
		if (child)
		{
			// Set child parent
			child->parent = edge;

			EdgeNode * prev = edge->last;
			if (prev == NULL)
				edge->first = edge->last = child;
			else
			{
				prev->next = child;
				edge->last = child;
			}
		}
		
		return edge;
	}
	else return NULL;
}

EdgeNode * parse_edge_str(char ** src, EdgeNode * root)
{
	#define NEXT(n) (*src) += n;
	#define PEAK(n) (*src)[n]
	#define COMMA ','
	#define L_PAR '('
	#define R_PAR ')'

	unsigned int num_par = 1;
	char * cursor = *src;
	EdgeNode * curr_edge = NULL;

	while (num_par && PEAK(0) != '\0')
	{
		switch (PEAK(0))
		{
		case L_PAR:
		case R_PAR:
		case COMMA:
		{
			// Create node
			// Don't consume character
			curr_edge = make_edge();
			edge_add_child(root, curr_edge);
			curr_edge->value = cursor;

			goto special;

			break;
		}

		default:
			NEXT(1);
			break;
		}

		// Next character
		continue;
		
	special:
		{
			char cc = PEAK(0);
			PEAK(0) = '\0';
			switch (cc)
			{
			case L_PAR:
			{
				NEXT(1);

				// Start new scope
				if (parse_edge_str(src, curr_edge) == NULL)
					// Propagate value error
					return NULL;
				
				break;
			}
			
			case R_PAR:
			{
				NEXT(1);

				// End current scope
				--num_par;
				break;
			}

			default: // COMMA
				NEXT(1);
				break;
			}
		}

		// Reset cursor
		cursor = *src;
	}

	if (cursor != *src)
	{
		// Commit pending value
		curr_edge = make_edge();
		edge_add_child(root, curr_edge);
		curr_edge->value = cursor;
	}

	if (num_par != 0)
		// We have a pending par, throw value error
		return NULL;
	
	return root;
	
	#undef COMMA
	#undef R_PAR
	#undef L_PAR
	#undef PEAK
	#undef NEXT
}