#ifndef _MODIFIER_H_
#define _MODIFIER_H_

#include <stdint.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>

enum Modifier_Errors {
	ERROR_INPUT_FILE_NOT_FOUND = 0x01,
	ERROR_CANNOT_OPEN_BACKUP_FILE = 0x02, 
	ERROR_READING_INPUT_FILE = 0x04,
	ERROR_TARGET_NOT_FOUND = 0x08,
	ERROR_MULTIPLE_TARGET_LOCATION = 0x10,
	ERROR_UNKNOWN_MESSAGE_DIGEST_MD5 = 0x20,
	ERROR_MESSAGE_DIGEST_CREATE_FAILED = 0x40,
	ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED = 0x80,
	ERROR_MESSAGE_DIGEST_UPDATE_FAILED = 0x100,
	ERROR_MESSAGE_DIGEST_FINALIZATION_FAILED = 0x200,
	ERROR_WRITING_BACKUP_FILE = 0x400,
	ERROR_MODIFYING_FILE = 0x800,
	ERROR_MD5_SUM_DOESNT_MATCH = 0x1000
};

/**
 * @brief class for handling and modifying file
 */
class Modifier {
	public:
	
		Modifier();

		/**
		 * @brief open file to modify and file to backup the original file
		 *
		 * @param input_file_name name of input file
		 * @param backup_file_name name of the backup file
		 * @param md5_checksum md5 checksum that is check against
		 *
		 * @return 0 on success
		 * @return bitwise or of error from modifiers error
		 */
		int load_files(const char* input_file_name, const char* backup_file_name, const char* md5_checksum);

		/**
		 * @brief find the first occurrence of sub array within array
		 *
		 * @param search_arr pointer to the first element of an array that will be searched
		 * @param search_arr_size size of searched array
		 * @param sub_arr pointer to the sub array that we want to find
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
		 *
		 * @return bitwise or of error from modifiers error
		 * @return 0 on success
		 */
		int replace_bytes(const uint8_t* target, const uint32_t target_size, const uint8_t* replace_string, const uint32_t replace_string_size);

		/**
		 * @return bitwise or of error from modifiers error
		 * @return 0 on success
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
		uint32_t file_size;
};

#endif

