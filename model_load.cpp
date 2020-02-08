// implement below code by using void *, so that we will not required to write show twice for int as well as float

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFFER_SIZE 1024

struct vec_int
{
	int* data;
	int size;
};

struct vec_float
{
	float* data;
	int size;
};

struct vec_int * create_vec_int(void);
void push_vec_int(struct vec_int*, int);
void show_vec_int(struct vec_int*);
void destroy_vec_int(struct vec_int*);

struct vec_float * create_vec_float(void);
void push_vec_float(struct vec_float*, float);
void show_vec_float(struct vec_float*);
void destroy_vec_float(struct vec_float*);

FILE * gp_file;
char buffer[BUFFER_SIZE];

void load_model(void);



struct vec_float * vec_float_ver = NULL;
struct vec_float * vec_float_nor = NULL;
struct vec_float * vec_float_tex = NULL;


int main(void)
{
	struct vec_int * vec_int_ver = NULL;
	struct vec_float * vec_float_ver = NULL;

	vec_int_ver		= create_vec_int();
	vec_float_ver	= create_vec_float();

	for(int i = 0; i < 10; i++)
	{
		push_vec_int(vec_int_ver,(i+1)*10);
		push_vec_float(vec_float_ver,(float)((i+1)*10)/7);
	}

	show_vec_int(vec_int_ver);
	show_vec_float(vec_float_ver);

	destroy_vec_int(vec_int_ver);
	destroy_vec_float(vec_float_ver);

	printf("\n\n");
	load_model();
	
	printf("VERTEX_DATA\n");
	show_vec_float(vec_float_ver);
	printf("\n");
	printf("TEXTURE_DATA\n");
	show_vec_float(vec_float_tex);
	printf("\n");
	printf("FLOAT_DATA\n");
	show_vec_float(vec_float_nor);

	destroy_vec_float(vec_float_ver);
	destroy_vec_float(vec_float_tex);
	destroy_vec_float(vec_float_nor);


	return(0);
}



struct vec_int * create_vec_int(void)
{
	struct vec_int * temp_struct_vec_int = NULL;
	temp_struct_vec_int = (struct vec_int*)malloc(sizeof(struct vec_int));
	memset(temp_struct_vec_int,0,sizeof(struct vec_int));
	return(temp_struct_vec_int);
}

struct vec_float * create_vec_float(void)
{
	struct vec_float * temp_struct_vec_float = NULL;
	temp_struct_vec_float = (struct vec_float*)malloc(sizeof(struct vec_float));
	memset(temp_struct_vec_float,0,sizeof(struct vec_float));
	return(temp_struct_vec_float);
}

void push_vec_int(struct vec_int* temp_struct_vec_int, int new_data)
{
	temp_struct_vec_int->data = (int*)realloc(temp_struct_vec_int->data, ((temp_struct_vec_int->size) + 1)* sizeof(int));
	temp_struct_vec_int->size = temp_struct_vec_int->size + 1;
	temp_struct_vec_int->data[(temp_struct_vec_int->size) - 1] = new_data;
}

void push_vec_float(struct vec_float* temp_struct_vec_float, float new_data)
{
	temp_struct_vec_float->data = (float*)realloc(temp_struct_vec_float->data, ((temp_struct_vec_float->size) + 1)* sizeof(float));
	temp_struct_vec_float->size = temp_struct_vec_float->size + 1;
	temp_struct_vec_float->data[(temp_struct_vec_float->size) - 1] = new_data;
}

void show_vec_int(struct vec_int* temp_struct_vec_int)
{
	for(int i = 0; i < temp_struct_vec_int->size; i++)
	{
		printf("vec_int_ver[%d] : %d \n", i, temp_struct_vec_int->data[i]);
	}
}

void show_vec_float(struct vec_float* temp_struct_vec_float)
{
	for(int i = 0; i < temp_struct_vec_float->size; i++)
	{
		printf("vec_float_ver[%d] : %f \n", i, temp_struct_vec_float->data[i]);
	}
}

void destroy_vec_int(struct vec_int* temp_struct_vec_int)
{
	free(temp_struct_vec_int->data);
	free(temp_struct_vec_int);
	temp_struct_vec_int = NULL;
}

void destroy_vec_float(struct vec_float* temp_struct_vec_float)
{
	free(temp_struct_vec_float->data);
	free(temp_struct_vec_float);
	temp_struct_vec_float = NULL;
}


void load_model()
{
	char * first_token 	= NULL;
	char * token 		= NULL;
	char * space		= " ";

	gp_file = fopen("mesh.obj","r");
	if(gp_file == NULL)
	{
		printf("ERROR : File open fail \n");
		exit(-1);
	}

	vec_float_ver = create_vec_float();
	vec_float_nor = create_vec_float();
	vec_float_tex = create_vec_float();

	while(fgets(buffer,BUFFER_SIZE,gp_file) != NULL)
	{
		first_token = strtok(buffer,space);

		if(strcmp(first_token,"v") == 0)
		{
			while((token = strtok(NULL,space)) != NULL)
			{
				push_vec_float(vec_float_ver,atof(token));
			}
		}
		else if (strcmp(first_token,"vt") == 0)
		{
			while((token = strtok(NULL,space)) != NULL)
			{
				push_vec_float(vec_float_tex,atof(token));
			}
		}
		else if(strcmp(first_token,"vn") == 0)
		{
			while((token = strtok(NULL,space)) != NULL)
			{
				push_vec_float(vec_float_nor,atof(token));
			}
		}
		else if(strcmp(first_token,"f") == 0)
		{
			printf("THINGS REMAINING for F\n");
		}

		memset(buffer,0,sizeof(BUFFER_SIZE));
	}

	fclose(gp_file);
}












