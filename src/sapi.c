#include <python3.6/Python.h>

#include <edge.h>

static PyObject * edge_to_dict(EdgeNode * edge)
{
	// Create new dict for node
	PyObject * dict = PyDict_New();
	
	if (edge == NULL)
		// Return new empty dict
		return dict;
	else
	{
		EdgeNode * it = edge->first; for (; it; it = it->next)
		{
			PyObject * key = PyUnicode_FromString(it->value);
			PyDict_SetItem(dict, key, edge_to_dict(it));
		}

		return dict;
	}
}

static PyObject * sapi_parse_edges_str(PyObject * module, PyObject * vargs)
{
	const char * edge_str = NULL;
	if (!PyArg_ParseTuple(vargs, "s", &edge_str))
		return NULL;
	
	if (edge_str == NULL)
		return PyErr_Format(PyExc_ValueError, "null string found");

	if (*edge_str == '\0')
		// Empty string given, return empty dict
		return PyDict_New();

	// Create usable buffer
	size_t len = strlen(edge_str);
	char * src = (char*)malloc(len + 1);
	memcpy(src, edge_str, len + 1); // Copy '\0' as well

	// Create starting edge node
	// Root node is empty, has no value
	EdgeNode * root = make_edge();

	// Start parsing string
	parse_edge_str(&src, root);

	// Create dict
	return edge_to_dict(root);
}

static PyMethodDef exported_methods[] = {
	{
		"parse_edges_str",
		&sapi_parse_edges_str,
		METH_VARARGS,
		"Given a string of edges - e.g. 'user(posts, "
		"friends(likes)) - generates the corresponding"
		"dictionary object"
	},

	// Close list
	NULL
};

static PyModuleDef sapi_module = {
	PyModuleDef_HEAD_INIT,/* Base */
	"sapi",/* Name */
	"N/A",/* Doc */
	-1,/* Size */
	exported_methods/* Methods */
};

PyMODINIT_FUNC PyInit_sapi()
{
	Py_Initialize();
	return PyModule_Create(&sapi_module);
}