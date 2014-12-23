#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector {
	int size;
	int count;
	void** data;
} vector;

vector* init_vector(int size);
bool vector_add_element(vector* v, void *element);
void vector_delete_element(vector* v, int index);
void vector_free(vector *v);

#endif // VECTOR_H

