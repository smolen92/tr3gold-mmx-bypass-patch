#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param *arr pointer to the first element of an array that will be searched
 * @param arr_size size of searched array
 * @param *sub_arr pointer to the sub array that we want to find
 * @param sub_arr_size size of the sub array
 *
 * @return position where the sub array start in searched array
 * @return -1 if the sub array wasn't found in searched array
 */
uint32_t find_sub_array_within_array(uint8_t *search_arr, int32_t search_arr_size, uint8_t *sub_arr, int32_t sub_arr_size) {
	
	for(int32_t i=0; i < search_arr_size - sub_arr_size; i++) {
		int found = 1;	
		for(int32_t j=0; j < sub_arr_size; j++) {
			
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
 * @param target pointer to the array of bytes you want to replace
 * @param target_size size of the target array
 * @param replace_string pointer to the array of bytes that will be written
 * @param replace_string_size size of the replace array
 * @param backup file that will be used for backup of the original file
 *
 * @detail backup - use NULL if you don't want to create backup file
 */
void replace_bytes(FILE* input, uint8_t* target, uint32_t target_size, uint8_t* replace_string, uint32_t replace_string_size, FILE* backup) {

	if(input == NULL) {
		printf("No file specified\n");
		return;
	}

	uint8_t* file_content;
	uint32_t file_size;

	//get file size and copy content of the file to buffer
	fseek(input, 0, SEEK_END);
	file_size = ftell(input);
	fseek(input, 0, SEEK_SET);
	
	file_content = (uint8_t*)malloc((file_size)*sizeof(uint8_t));
	
	if(fread((void*)file_content, sizeof(uint8_t), file_size, input) != file_size) {
		printf("Error while reading file\n");
	}
	fseek(input, 0, SEEK_SET);
	
	//create backup of the original file
	if(backup != NULL) {
		fwrite(file_content, sizeof(uint8_t), file_size, backup);
	}
	else {
		printf("No backup file will be created. Continue [y/N]: ");
		/// \todo check input
		char c = getchar();
		if( (c != 'y') && (c != 'Y') ) return;
	}

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
			fseek(input, pos, SEEK_SET);
			fwrite(replace_string, sizeof(uint8_t), replace_string_size, input);
		}

	}


}

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
	
	int32_t target_size = 24;
	int32_t replace_size = 15;

	FILE *input, *backup;

	input = fopen(argv[1], "rb+");
	if(input == NULL) {
		printf("Failed to open file %s\n", argv[1]);
		return 1;
	}
	
	char *backup_file_path;
	backup_file_path = (char*)malloc((strlen(argv[1])+strlen(".bak"))*sizeof(char));
	if( backup_file_path == NULL) {
		printf("Allocation for backup file path failed\n");
		return 1;
	}
	
	strcpy(backup_file_path, argv[1]);
	strcpy(&backup_file_path[strlen(argv[1])], ".bak"); 

	backup = fopen(backup_file_path, "wb");

	replace_bytes(input, target, target_size, replace, replace_size, backup);

	fclose(input);
	input = NULL;

	fclose(backup);
	backup = NULL;

	return 0;

}

