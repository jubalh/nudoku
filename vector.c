#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "vector.h"

#define MULTIPLIER 2

vector* init_vector(int size)
{
	vector* v = (vector*)malloc (sizeof(vector));
	if (v)
	{
		v->data = (void**)malloc(size * sizeof(void*));
		if (v->data)
		{
			v->count = 0;
			v->size = size;
		}
		else
		{
			free(v);
			v = NULL;
		}
	}

	return v;
}

bool vector_add_element(vector* v, void *element)
{
	if (v->count == v->size)
	{
		int new_size = v->size*MULTIPLIER;
		void* p = (void*)realloc(v->data, new_size * sizeof(void*));
		if (p == NULL)
			return false;
		v->data = p;
		v->size = new_size;
	}
	v->data[v->count] = element;
	v->count++;

	return true;
}

void vector_delete_element(vector* v, int index)
{
	if(index > v->size)
		return;

	v->data[index] = NULL;
	if (index < v->count)
		memmove(&v->data[index], &v->data[index+1], sizeof(void*) * (v->size - v->count));
	v->count--;
}

void vector_free(vector *v)
{
	if(v)
	{
		if (v->data)
		{
			free(v->data);
			v->data = NULL;
		}
		free(v);
		v = NULL;
	}
}

