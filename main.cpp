#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * @param *arr pointer to the first element of an array that will be searched
 * @param arr_size size of searched array
 * @param *sub_arr pointer to the sub array that we want to find
 * @param sub_arr_size size of the sub array
 *
 * @return position where the sub array start in searched array
 * @return -1 if the sub array wasn't found in searched array
 */
uint32_t find_sub_array_within_array(uint8_t *search_arr, uint32_t search_arr_size, uint8_t *sub_arr, uint32_t sub_arr_size) {
	
	for(uint32_t i=0; i < search_arr_size - sub_arr_size; i++) {
		int found = 1;	
		for(uint32_t j=0; j < sub_arr_size; j++) {
			
			if(search_arr[i+j] != sub_arr[j]) {
				found = 0;
				break;
			}

		}
		
		if(found) return i;

	}

	return -1;

}

/**
 * @param file pointer to file that will have the bytes replaced
 * @param *target pointer to the array of bytes you want to replace
 * @param target_size size of the target array
 * @param replace_string array of bytes that will be written
 * @param replace_string_size size of the replace array
 */
void replace_bytes(FILE* file, uint8_t* target, uint32_t target_size, uint8_t* replace_string, uint32_t replace_string_size) {
	
	fseek(file, 0, SEEK_END);

	uint8_t* file_content;
	uint32_t file_size;

	//get file size a copy content of the file to buffer
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	file_content = (uint8_t*)malloc((file_size+1)*sizeof(uint8_t));
	
	if(fread((void*)file_content, sizeof(uint8_t), file_size, file) != file_size) {
		printf("Error while reading file\n");
	}
	file_content[file_size] = '\0';
	fseek(file, 0, SEEK_SET);

	//find target in file
	uint32_t pos = find_sub_array_within_array(file_content, file_size, target, target_size);
	
	if(pos == -1) {
		printf("Target not found\n");
	}
	else {
		//check if there are more than one location of target in file
		uint32_t next_pos = find_sub_array_within_array(file_content+pos+1,file_size - pos,target, target_size);

		if(next_pos != -1) {
			printf("multiple target location within file found\n");
		}
		else {
			fseek(file, pos, SEEK_SET);
			fwrite(replace_string, sizeof(uint8_t), replace_string_size, file);
		}

	}


}

/// \todo copy original file to backup
/// \todo check md5 of the original file before patching
/// \todo readme
int main(int argc, char **argv) {

	if(argc < 2) {
		printf("No file specified\n");
		return 1;
	}
	
	//target and replace string
	uint8_t target[] = { 0x89, 0x15, 0x50, 0x7c, 0x6c, 0x00, //mov dword ptr [DAT_006c7c50], EDX
				0x5b, // POP EBX
				0x8b, 0xe5, //MOV ESP,EBP
				0x5d, //POP EBP
				0xc3, //RET
				0x90, //NOP
				0x90,
				0x90, 
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90, 
				0x90,
				0x90
				}; 
	
	uint8_t replace[] = { 0xc7, 0x05, 0x50, 0x7c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, //MOV dword ptr[DAT_006c7c50], 0x00
				0x5b, //POP EBX
				0x8b, 0xe5, //MOV ESP, EBP
				0x5d, //POP EBP
				0xc3, //RET
				};
	
	uint32_t target_size = 24;
	uint32_t replace_size = 15;

	FILE *input, *output;

	input = fopen(argv[1], "rb+");
	if(input == NULL) {
		printf("Failed to open file %s\n", argv[1]);
		return 1;
	}
	
	replace_bytes(input, target, target_size, replace, replace_size);

	fclose(input);
	input = NULL;

	return 0;

}

