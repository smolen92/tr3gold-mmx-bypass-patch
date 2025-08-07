#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

/**
 * @brief find the first occurrence of sub array within array
 *
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

/**
 * @param input file to calculate the md5 sum for
 *
 * @return pointer to string containing the md5 sum
 * @return NULL when error occured
 *
 * @details if the function is succesful, string is allocated inside this function, that need to be freed.
 */
char* get_md5(FILE* input) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	md = EVP_get_digestbyname("md5");
	if (md == NULL) {
		printf("Unknown message digest md5");
		return NULL;
	}

	mdctx = EVP_MD_CTX_new();
	if (mdctx == NULL) {
		printf("Message digest create failed.\n");
		return NULL;
	}

	if (!EVP_DigestInit_ex2(mdctx, md, NULL)) {
		printf("Message digest initialization failed.\n");
		EVP_MD_CTX_free(mdctx);
		return NULL;
	}

	int32_t file_buffer_size = 1024;
	char file_buffer[file_buffer_size];
	int bytes_read;

	do {
		bytes_read = fread(file_buffer, sizeof(char), file_buffer_size, input);
		if (!EVP_DigestUpdate(mdctx, file_buffer, bytes_read) ) {
			printf("Message digest update failed.\n");
			EVP_MD_CTX_free(mdctx);
			return NULL;
    		}

	} while( bytes_read > 0);

	if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
		printf("Message digest finalization failed.\n");
		EVP_MD_CTX_free(mdctx);
        	return NULL;
    	}

	EVP_MD_CTX_free(mdctx);

	char *return_char = (char*)malloc((md_len *2)*sizeof(char));

	for (int i = 0; i < md_len; i++) {
		sprintf(&return_char[i*2], "%02x", md_value[i]);
	}

	return return_char;
}

/**
 * @param files array of files to be freed
 * @param files_count count of files to be freed
 * @param strings array of strings to be freed
 * @param strings_count count of strings to be freed
 */
void free_resources(FILE** files, uint32_t files_count, char** strings, uint32_t strings_count) {
	for(int i=0; i < files_count; i++) {
		fclose(files[i]);
		files[i] = NULL;
	}

	for(int i=0; i < strings_count; i++) {
		free(strings[i]);
		strings[i] = NULL;
	}
}

enum files_names {
	INPUT,
	BACKUP,
	MAX_FILES_NAMES
};

enum strings_names {
	BACKUP_FILE_PATH,
	INPUT_FILE_MD5SUM,
	MAX_STRINGS_NAME
};

/// \todo test on windows - it need to install openssl lib - add this to instructions
int main(int argc, char **argv) {

	if(argc < 2) {
		printf("Usage: tr3gold-mmx-bypass.exe <path-to-the-executable>\n");
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
	
	char compatible_md5sum[] = "7c820c372f3ca0b7e97e09cc91a0f033";

	FILE *files[MAX_FILES_NAMES];
	char *strings[MAX_STRINGS_NAME];
	uint32_t files_count = 0;
	uint32_t strings_count = 0;

	files[INPUT] = fopen(argv[1], "rb+");
	if(files[INPUT] == NULL) {
		printf("Failed to open file %s\n", argv[1]);
		perror("Error");
		return 1;
	}
	
	files_count++;

	strings[BACKUP_FILE_PATH] = (char*)malloc((strlen(argv[1])+strlen(".bak"))*sizeof(char));
	if( strings[BACKUP_FILE_PATH] == NULL) {
		printf("Allocation for backup file path failed\n");
		free_resources(files, files_count, strings, strings_count);
		return 1;
	}
	
	strings_count++;

	strcpy(strings[BACKUP_FILE_PATH], argv[1]);
	strcpy(&strings[BACKUP_FILE_PATH][strlen(argv[1])], ".bak"); 

	files[BACKUP] = fopen(strings[BACKUP_FILE_PATH], "ab");
	if( files[BACKUP] == NULL ) {
		printf("Failed to open file %s\n", strings[BACKUP_FILE_PATH]);
		perror("Error");
		free_resources(files, files_count, strings, strings_count);
		return 1;
	}

	files_count++;

	strings[INPUT_FILE_MD5SUM] = get_md5(files[INPUT]);
	if( strings[INPUT_FILE_MD5SUM] == NULL ) {
		printf("Failed to calculate md5sum for input file\n");
		free_resources(files, files_count, strings, strings_count);
		return 1;
	}
	
	strings_count++;

	if( strcmp(compatible_md5sum, strings[INPUT_FILE_MD5SUM]) != 0) {
		printf("Input file is not compatible/wasn't tested. Continue? [y/N]: ");
		char c = getchar();
		if( (c != 'y') && (c != 'Y') ) {
			free_resources(files, files_count, strings, strings_count);
			return 1;
		}
	}
	
	replace_bytes(files[INPUT], target, target_size, replace, replace_size, files[BACKUP]);
	
	free_resources(files, files_count, strings, strings_count);
	
	return 1;

}

