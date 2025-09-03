#ifndef _MODIFIER_H_
#define _MODIFIER_H_

#include <stdio.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <stdlib.h>

enum Modifier_Errors {
	INPUT_FILE_NOT_FOUND,
	BACKUP_FILE_NOT_FOUND,
	ERROR_READING_INPUT_FILE,
	ERROR_TARGET_NOT_FOUND,
	ERROR_MULTIPLE_TARGET_LOCATION,
	ERROR_UNKNOWN_MESSAGE_DIGEST_MD5,
	ERROR_MESSAGE_DIGEST_CREATE_FAILED,
	ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED,
	ERROR_MESSAGE_DIGEST_UPDATE_FAILED,
	ERROR_MESSAGE_MESSAGE_DIGEST_FINALIZATION_FAILED
};

class Modifier {
	public:
		
		/**
		 * @brief open file to modify and file to backup the original file
		 */
		int load_files(const char* input_file_name, const char* backup_file_name);

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
		int32_t find_sub_array_within_array(const uint8_t *search_arr, const int32_t search_arr_size, const uint8_t *sub_arr, const int32_t sub_arr_size);

		/**
		 * @param target pointer to the array of bytes you want to replace
		 * @param target_size size of the target array
		 * @param replace_string pointer to the array of bytes that will be written
		 * @param replace_string_size size of the replace array
		 */
		int replace_bytes(const uint8_t* target, const uint32_t target_size, const uint8_t* replace_string, const uint32_t replace_string_size);

		/**
		 * @param input file to calculate the md5 sum for
		 *
		 * @return pointer to string containing the md5 sum
		 * @return NULL when error occured
		 *
		 * @details if the function is succesful, string is allocated inside this function, that need to be freed.
		 */
		int get_md5();
		
		/**
		 * @brief close files and clear md5_checksum
		 */
		~Modifier();
	private:
		FILE* input_file;
		FILE* backup_file;

		char* md5_checksum;
};

#endif

