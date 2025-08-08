#include <stdio.h>

int main(int argc, char **argv) {
	
	FILE *input;

	const char *file_path[] = { "./test.txt", "/home/u3605/test.txt"};

	for(int i=0; i < 2; i++) {
		input = fopen(file_path[i], "r");
		if(input != NULL) break;
	}

	if(input == NULL) {
		printf("Error\n");
		return 1;
	}

	fclose(input);
	input = NULL;

	return 0;
}

