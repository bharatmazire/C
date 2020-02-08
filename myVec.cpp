#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct vec_int
{
	int* data;
	int size;
};


struct vec_int * create_vec_int(void);
void push_vec_int(struct vec_int*, int);
void show_vec_int(struct vec_int*);
void destroy_vec_int(struct vec_int*);

int main(void)
{
	struct vec_int * vec_int_ver = NULL;

	vec_int_ver	= create_vec_int();

	for(int i = 0; i < 10; i++)
	{
		push_vec_int(vec_int_ver,(i+1)*10);
	}

	show_vec_int(vec_int_ver);

	destroy_vec_int(vec_int_ver);
	
	return(0);
}



struct vec_int * create_vec_int(void)
{
	struct vec_int * temp_struct_vec_int = NULL;
	temp_struct_vec_int = (struct vec_int*)malloc(sizeof(struct vec_int));
	memset(temp_struct_vec_int,0,sizeof(struct vec_int));
	return(temp_struct_vec_int);
}

void push_vec_int(struct vec_int* temp_struct_vec_int, int new_data)
{
	temp_struct_vec_int->data = (int*)realloc(temp_struct_vec_int->data, ((temp_struct_vec_int->size) + 1)* sizeof(int));
	temp_struct_vec_int->size = temp_struct_vec_int->size + 1;
	temp_struct_vec_int->data[(temp_struct_vec_int->size) - 1] = new_data;
}

void show_vec_int(struct vec_int* temp_struct_vec_int)
{
	for(int i = 0; i < temp_struct_vec_int->size; i++)
	{
		printf("vec_int_ver[%d] : %d \n", i, temp_struct_vec_int->data[i]);
	}
}

void destroy_vec_int(struct vec_int* temp_struct_vec_int)
{
	free(temp_struct_vec_int->data);
	free(temp_struct_vec_int);
	temp_struct_vec_int = NULL;
}
