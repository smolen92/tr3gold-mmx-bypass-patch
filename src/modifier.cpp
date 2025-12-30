#include "modifier.h"

Modifier::Modifier() {
	input_file = NULL;
	backup_file = NULL;
	file_content = NULL;
}

int Modifier::load_files(const char* input_file_name, const char* md5_checksum) {

	int return_value = 0;

	input_file = fopen(input_file_name, "rb+"); //flawfinder: ignore
	if( input_file == NULL) {
		return ERROR_INPUT_FILE_NOT_FOUND;
	}
	else { 
		//get file size and copy content of the file to buffer
		fseek(input_file, 0, SEEK_END);
		file_size = ftell(input_file);
		fseek(input_file, 0, SEEK_SET);
		
		file_content = (uint8_t*)malloc((file_size)*sizeof(uint8_t));
		if(file_content == NULL) {
			return ERROR_INPUT_BUFFER_ALLOCATION;
		}

		unsigned int bytes_read = fread((void*)file_content, sizeof(uint8_t), file_size, input_file);

		if( bytes_read != file_size ) {
			return ERROR_READING_INPUT_FILE;
		}
		fseek(input_file, 0, SEEK_SET);
	
		this->get_md5();

		if( (return_value == 0) && (strcmp(md5_checksum, this->md5_checksum) != 0) ) {
			return_value |= ERROR_MD5_SUM_DOESNT_MATCH;
		}
	}
	
	char* backup_file_name;
	uint32_t input_file_name_length = strlen(input_file_name); //flawfinder: ignore

	backup_file_name = (char*)malloc((input_file_name_length+1)*sizeof(char));
	if(backup_file_name == NULL) {
		return_value |= ERROR_CANNOT_OPEN_BACKUP_FILE;
	}
	else {
		strncpy(backup_file_name, input_file_name,input_file_name_length);
		//change extension from exe to bak
		strcpy(&backup_file_name[input_file_name_length-3], "bak"); //flawfinder: ignore

		backup_file = fopen(backup_file_name, "wbx"); //flawfinder: ignore
		if( backup_file == NULL) {
			return_value |= ERROR_CANNOT_OPEN_BACKUP_FILE;
		}
		else {
			//create backup file of the original file
			if( fwrite(file_content, sizeof(uint8_t), file_size, backup_file) < file_size ) {
				return_value |= ERROR_WRITING_BACKUP_FILE;
			}
	
			fclose(backup_file);
			backup_file = NULL;
		}
		

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
	//find target in file
	int32_t pos = find_sub_array_within_array(file_content, file_size, target, target_size);
	
	if(pos == -1) {
		return ERROR_TARGET_NOT_FOUND;
	}
	else {
		//check if there are more than one location of target in file
		int32_t next_pos = find_sub_array_within_array(file_content+pos+1,file_size - pos,target, target_size);

		if(next_pos != -1) {
			return ERROR_MULTIPLE_TARGET_LOCATION;
		}
		else {
			fseek(input_file, pos, SEEK_SET);
			if( fwrite(replace_string, sizeof(uint8_t), replace_string_size, input_file) < replace_string_size) {
				return ERROR_MODIFYING_FILE;
			}
		}

	}

	return 0;
}

void Modifier::get_md5() {

	uint32_t s[] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22, 
			5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20, 
			4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23, 
			6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

	uint32_t k[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 
			0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
			0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 
			0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
			0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 
			0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
			0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 
			0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
			0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 
			0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
			0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 
			0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
			0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 
			0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
			0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 
			0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};


	uint32_t a0 = 0x67452301;
	uint32_t b0 = 0xefcdab89;
	uint32_t c0 = 0x98badcfe;
	uint32_t d0 = 0x10325476;

	uint32_t chunk_count = (file_size/64) + 1;
	uint32_t padding = 64 - (file_size%64);

	if(padding <= 8) {
		chunk_count += 1;
		padding += 64;
	}

	uint8_t padding_buffer[padding];

	padding_buffer[0] = 0x80;
	memset(&padding_buffer[1], 0x00, padding-1);
	uint32_t padding_buffer_index = 0;
	
	uint64_t bytes_read_total = 0;

	for(uint64_t i=1; i <= chunk_count; i++) {
		uint8_t buffer[64];

		if( i < chunk_count ) {
			
			memcpy(buffer, &file_content[(i-1)*64], 64);
			bytes_read_total += 64;
		}
		else {
			uint64_t remaining_bytes = file_size%64;
			memcpy(buffer, &file_content[(i-1)*64], remaining_bytes);
			
			if(remaining_bytes < 64) {
				memcpy(&buffer[remaining_bytes], &padding_buffer[padding_buffer_index], 64-remaining_bytes);
				padding_buffer_index = 64-remaining_bytes;
			}
		}

		//last chunk
		if(i == chunk_count) {
			for(int j=0; j < 8; j++) {	
				buffer[j+56] = ((file_size*8) >> (j*8)) & 0xFF;
			}
		}
		
		uint32_t M[16];
		for(int j=0; j < 16; j++) {
			M[j] =  (uint32_t)buffer[j*4]             +
				((uint32_t)buffer[(j*4)+1] << 8 ) +
				((uint32_t)buffer[(j*4)+2] << 16) +
				((uint32_t)buffer[(j*4)+3] << 24);
		}

		uint32_t A = a0;
		uint32_t B = b0;
		uint32_t C = c0;
		uint32_t D = d0;

		for(uint64_t j=0; j < 64; j++) {
			uint32_t F,g;

			if(j < 16) {
				F = (B & C) | ((~B) & D);
				g = j;
			}
			else if(j < 32) {
				F = (D & B) | ((~D) & C);
				g = (5*j + 1)%16;
			}
			else if(j < 48) {
				F = B ^ C ^ D;
				g = (3*j + 5)%16;
			}
			else {
				F = C ^ (B | (~D));
				g = (7*j)%16;
			}

			F += A + k[j] + M[g];
			A = D;
			D = C;
			C = B;
			B += (F << s[j]) | (F >> (32-s[j]) );

		}

		a0 += A;
		b0 += B;
		c0 += C;
		d0 += D;
	}
	
	sprintf(md5_checksum, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
			a0 & 0xFF, (a0 >> 8) & 0xFF, (a0 >> 16) & 0xFF, (a0 >> 24) & 0xFF,
			b0 & 0xFF, (b0 >> 8) & 0xFF, (b0 >> 16) & 0xFF, (b0 >> 24) & 0xFF,
			c0 & 0xFF, (c0 >> 8) & 0xFF, (c0 >> 16) & 0xFF, (c0 >> 24) & 0xFF,
			d0 & 0xFF, (d0 >> 8) & 0xFF, (d0 >> 16) & 0xFF, (d0 >> 24) & 0xFF);

	//printf("%s\n", md5_checksum);

}

Modifier::~Modifier() {
	if(input_file != NULL) fclose(input_file);
	input_file = NULL;

	free(file_content);
	file_content = NULL;
}

