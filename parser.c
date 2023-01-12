#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
FILE * fptr;

struct ArrayTable {
	char name[10]; // array name
	char size[5]; // array size
	char type[10]; // int, float, etc.
};
struct ArrayTable AT[20];


void read(char *file, char *name);
void declare(char *array, char *n, char *type);
void print(char *name);
void copy(char *destination, char *source);
void arithmetic(char *array, char *array2, char *array3, char *op);
void initialize(char* array, char *number);
bool StartsWith(const char *a, const char *b);

int main(void) {
    int i=0;
    int numProgs=0;
    char line[255];
	char * token;
	int count = 0;
	int size = 0;
	
	
	//find line size
	FILE *line_size;
    line_size = fopen("source.c", "r");
    while(fgets(line, sizeof line, line_size)!=NULL) {
        size++;
    }
	fclose(line_size);

    char* lineArray[size]; // +100 is not to get segmentation error or appear something wrong character

    FILE *file;
    file = fopen("source.c", "r");
	fptr = fopen("expanded.c","w");
	

    while(fgets(line, sizeof line, file)!=NULL) {
        lineArray[i]=strdup(line);
        i++;
    }


    for (int j=0 ; j<size; j++) {
        if(lineArray[j]!=NULL && StartsWith(lineArray[j], "	$")){
			if(lineArray[j]!=NULL && StartsWith(lineArray[j], "	$dec")){
				token = strtok(lineArray[j], "$(,)=+-/* ;");
				token = strtok(NULL, "$(,)=+-/* ;");
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				strcpy(AT[count].name, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_size
				strcpy(AT[count].size, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_type
				strcpy(AT[count].type, token);
				declare(AT[count].name, AT[count].size, AT[count].type);
				//printf("%s",declare(AT[count].name, AT[count].size, AT[count].type));
				count++;
			}
			else if(lineArray[j]!=NULL && StartsWith(lineArray[j], "	$read")){
				token = strtok(NULL, "$(,)=+-/* ;");
				token = strtok(lineArray[j], "$(,)=+-/* ;"); //read
				token = strtok(NULL, "$(,)=+-/* ;"); //file_name
				char * tmp = token; //get file_name to tmp
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				read(tmp, token);
				//printf("%s",read(tmp, token)); // tmp = file_name, token = array_name
			}
			else if(lineArray[j]!=NULL && StartsWith(lineArray[j], "	$print")){
				token = strtok(NULL, "$(,)=+-/* ;");
				token = strtok(lineArray[j], "$(,)=+-/* ;"); //print
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				print(token);
				//printf("%s",print(token));
			}
			else{
				token = strtok(lineArray[j], "$(,)= ;"); // array_name
				token = strtok(NULL, "$(,)=+-/* ;");
				char *a_name = token;
				token = strtok(NULL, "$(,)= ;");
				if(isdigit(token[0])){// check token starts with digit or not.
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						//$x = 5 + $y
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						arithmetic(a_name, a_name2, token, operation);
						//printf("%s", arithmetic(a_name, a_name2, token, operation));
						
					}
					else{
						//$x = 5;
						initialize(a_name, a_name2);
						//printf("%s", initialize(a_name, a_name2));
					}
				} 
				else{
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						arithmetic(a_name, a_name2, token, operation);
						//printf("%s", arithmetic(a_name, a_name2, token, operation));
						
						/*if(isdigit(token[0])){
							char *number = token; //$A = $B + 5
							printf("%s", arithmetic(a_name, a_name2, number, operation));
							
						}
						else{
							char * a_name3 = token; //$A = $A + $B
							printf("%s", arithmetic(a_name, a_name2, a_name3, operation));
						}*/
					}
					else if(StartsWith(token, "\n")){
						//printf("%s",copy(a_name, a_name2));//$x = $y
						copy(a_name, a_name2);
					}
				}
			}
        }
        else if(lineArray[j]!=NULL && StartsWith(lineArray[j], "$")){
			if(lineArray[j]!=NULL && StartsWith(lineArray[j], "$dec")){
				token = strtok(lineArray[j], "$(,)=+-/* ;");
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				strcpy(AT[count].name, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_size
				strcpy(AT[count].size, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_type
				strcpy(AT[count].type, token);
				declare(AT[count].name, AT[count].size, AT[count].type);
				count++;
			}
			else if(lineArray[j]!=NULL && StartsWith(lineArray[j], "$read")){
				token = strtok(lineArray[j], "$(,)=+-/* ;"); //read
				token = strtok(NULL, "$(,)=+-/* ;"); //file_name
				char * tmp = token; //get file_name to tmp
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				read(tmp, token); // tmp = file_name, token = array_name
			}
			else if(lineArray[j]!=NULL && StartsWith(lineArray[j], "$print")){
				token = strtok(lineArray[j], "$(,)=+-/* ;"); //print
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				print(token);
			}
			else{
				token = strtok(lineArray[j], "$(,)= ;"); // array_name
				char *a_name = token;
				token = strtok(NULL, "$(,)= ;");
				if(isdigit(token[0])){// check token starts with digit or not.
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						//$x = 5 + $y
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						arithmetic(a_name, a_name2, token, operation);
						
					}
					else{
						initialize(a_name, a_name2);
					}
				} 
				else{
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						arithmetic(a_name, a_name2, token, operation);
						
						/*if(isdigit(token[0])){
							char *number = token; //$A = $B + 5
							printf("%s", arithmetic(a_name, a_name2, number, operation));
							
						}
						else{
							char * a_name3 = token; //$A = $A + $B
							printf("%s", arithmetic(a_name, a_name2, a_name3, operation));
						}*/
					}
					else if(StartsWith(token, "\n")){
						copy(a_name, a_name2);//$x = $y
					}
				}
			}
        }
		else if(lineArray[j]!=NULL){
			
			//printf("%s", lineArray[j]); //printf yerine texte yazdırma yapılacak
			fprintf(fptr,"%s\n",lineArray[j]);
		}
    }

    fclose(file);
	fclose(fptr);
    return 0;
}

void read(char *file, char *name)  {
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(name, AT[i].name))){
			break;
		}
	}
	
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	FILE *ff=fopen(\"");
	strcat(string, file);
	strcat(string, "\", \"r\");\n");
	strcat(string,"	for (int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string,"; i++)\n");
	strcat(string,"		fscanf(ff, \"%d\", &");
	strcat(string, name);
	strcat(string, "[i]);\n	close(ff);\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);
}

void declare(char *array, char *n, char *type)  {
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	");
	strcat(string, type);
	strcat(string, " ");
	strcat(string, array);
	strcat(string, "[");
	strcat(string, n);
	strcat(string, "];\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);
}

void print(char *name)  {
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(name, AT[i].name))){
			break;
		}
	}
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for (int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n");
	strcat(string, "		printf(\"%d \", &");
	strcat(string, AT[i].name);
	strcat(string, "[i]);\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);
}

void copy(char *destination, char *source)  {
	
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(destination, AT[i].name))){
			break;
		}
	}
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, destination);
	strcat(string, "[i] = ");
	strcat(string, source);
	strcat(string, "[i];\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);
}

void arithmetic(char *array, char *array2, char *array3, char *op)  {
	
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(array, AT[i].name))){
			break;
		}
	}
		
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, array);
	strcat(string, "[i] =" );
	strcat(string, array2);
	if(!(isdigit(array2[0])))
		strcat(string, "[i]");
	strcat(string, " + ");
	strcat(string, array3);
	if(isdigit(array3[0]))
		strcat(string, ";\n");
	else
		strcat(string, "[i];\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);
}

void initialize(char *array,char *number){

	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(array, AT[i].name))){
			break;
		}
	}

	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, array);
	strcat(string, "[i] = ");
	strcat(string, number);
	strcat(string, ";\n");
	fptr = freopen(NULL,"a",fptr);
	fprintf(fptr,"%s",string);


}

bool StartsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}