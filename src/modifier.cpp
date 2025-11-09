#include "modifier.h"

Modifier::Modifier() {
	input_file = NULL;
	backup_file = NULL;
	md5_checksum = NULL;
}

int Modifier::load_files(const char* input_file_name, const char* backup_file_name, const char* md5_checksum) {

	int return_value = 0;

	input_file = fopen(input_file_name, "rb+"); //flawfinder: ignore
	if( input_file == NULL) {
		return_value |= ERROR_INPUT_FILE_NOT_FOUND;
	}
	else { 
		//get file size and copy content of the file to buffer
		fseek(input_file, 0, SEEK_END);
		file_size = ftell(input_file);
		fseek(input_file, 0, SEEK_SET);
		
		return_value |= this->get_md5();

		if( (return_value == 0) && (strcmp(md5_checksum, this->md5_checksum) != 0) ) {
			return_value |= ERROR_MD5_SUM_DOESNT_MATCH;
		}
	}

	backup_file = fopen(backup_file_name, "wb+"); //flawfinder: ignore
	if( backup_file == NULL) {
		return_value |= ERROR_CANNOT_OPEN_BACKUP_FILE;
	}

	return return_value;
}
	
int32_t Modifier::find_sub_array_within_array(const uint8_t *search_arr, const int32_t search_arr_size, const uint8_t *sub_arr, const int32_t sub_arr_size) {
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

int Modifier::replace_bytes(const uint8_t* target, const uint32_t target_size, const uint8_t* replace_string, const uint32_t replace_string_size) {
	uint8_t* file_content;

	file_content = (uint8_t*)malloc((file_size)*sizeof(uint8_t));
	if(file_content == NULL) {
		free(file_content);
		return ERROR_INPUT_BUFFER_ALLOCATION;
	}

	unsigned int bytes_read = fread((void*)file_content, sizeof(uint8_t), file_size, input_file);

	if( bytes_read != file_size ) {
		free(file_content);
		return ERROR_READING_INPUT_FILE;
	}
	fseek(input_file, 0, SEEK_SET);
	
	//create backup file of the original file
	/// \bug if this isn't reached the backup file is empty
	if( fwrite(file_content, sizeof(uint8_t), file_size, backup_file) < file_size ) {
		free(file_content);
		return ERROR_WRITING_BACKUP_FILE;
	}

	//find target in file
	int32_t pos = find_sub_array_within_array(file_content, file_size, target, target_size);
	
	if(pos == -1) {
		free(file_content);
		return ERROR_TARGET_NOT_FOUND;
	}
	else {
		//check if there are more than one location of target in file
		int32_t next_pos = find_sub_array_within_array(file_content+pos+1,file_size - pos,target, target_size);

		if(next_pos != -1) {
			free(file_content);
			return ERROR_MULTIPLE_TARGET_LOCATION;
		}
		else {
			fseek(input_file, pos, SEEK_SET);
			if( fwrite(replace_string, sizeof(uint8_t), replace_string_size, input_file) < replace_string_size) {
				free(file_content);
				return ERROR_MODIFYING_FILE;
			}
		}

	}

	free(file_content);

	return 0;
}

int Modifier::get_md5() {
	int return_value = 0;

	EVP_MD_CTX *mdctx;
	const EVP_MD *md;
	unsigned char md_value[EVP_MAX_MD_SIZE]; //flawfinder: ignore
	unsigned int md_len;

	md = EVP_get_digestbyname("md5");
	if (md == NULL) {
		return_value |= ERROR_UNKNOWN_MESSAGE_DIGEST_MD5;
		return return_value;
	}

	mdctx = EVP_MD_CTX_new();
	if (mdctx == NULL) {
		return_value |= ERROR_MESSAGE_DIGEST_CREATE_FAILED;
		return return_value;
	}

	if (!EVP_DigestInit_ex2(mdctx, md, NULL)) {
		return_value |= ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED;
		EVP_MD_CTX_free(mdctx);
		return return_value;
	}

	int32_t file_buffer_size = 1024;
	char file_buffer[file_buffer_size]; //flawfinder: ignore
	int bytes_read;

	do {
		bytes_read = fread(file_buffer, sizeof(char), file_buffer_size, input_file);
		if (!EVP_DigestUpdate(mdctx, file_buffer, bytes_read) ) {
			return_value |= ERROR_MESSAGE_DIGEST_UPDATE_FAILED;
			EVP_MD_CTX_free(mdctx);
			return return_value;
    		}

	} while( bytes_read > 0);

	fseek(input_file, 0, SEEK_SET);
	
	if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
		return_value |= ERROR_MESSAGE_DIGEST_FINALIZATION_FAILED;
		EVP_MD_CTX_free(mdctx);
        	return return_value;
    	}

	EVP_MD_CTX_free(mdctx);

	md5_checksum = (char*)malloc((md_len *2)*sizeof(char));

	for (unsigned int i = 0; i < md_len; i++) {
		sprintf(&md5_checksum[i*2], "%02x", md_value[i]); //flawfinder: ignore
	}
	
	return return_value;
}

Modifier::~Modifier() {
	if(input_file != NULL) fclose(input_file);
	if(backup_file != NULL) fclose(backup_file);

	if(md5_checksum != NULL) free(md5_checksum);
}

