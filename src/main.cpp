#include "modifier.h"
#include "gui.h"

/// \file

/// \cond
int main(int argc, char **argv) {
	//argumets
	bool cli = false;
	
	char default_file_name[] = "tr3gold.exe";
	char *exe_file_name;

	exe_file_name = default_file_name;

	if(argc > 1) {
		for(int i=1; i < argc; i++) {
			if(strcmp("--cli", argv[i]) == 0) {
				cli = true;
			}

			if(strncmp("-i=", argv[i], 3) == 0) {
				if(strlen(argv[i]) > strlen("-i=")) exe_file_name = &argv[i][3]; //flawfinder: ignore
			}
		}
	}

	//target and replace string
	const uint8_t target[] = { 0x89, 0x15, 0x50, 0x7c, 0x6c, 0x00, //mov dword ptr [DAT_006c7c50], EDX
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
	
	const uint8_t replace[] = { 0xc7, 0x05, 0x50, 0x7c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, //MOV dword ptr[DAT_006c7c50], 0x00
				0x5b, //POP EBX
				0x8b, 0xe5, //MOV ESP, EBP
				0x5d, //POP EBP
				0xc3, //RET
				};
	
	const int32_t target_size = 24;
	const int32_t replace_size = 15;
	
	const char compatible_md5sum[] = "7c820c372f3ca0b7e97e09cc91a0f033";
	
	//Modifier setup
	Modifier tr3gold_modifier;
	
	int files_status = tr3gold_modifier.load_files(exe_file_name, "tr3gold.bak", compatible_md5sum);

	//gui/cli selection	
	TTF_Font* font;
	Gui gui;
	
	int gui_return_value = gui.gui_init(&font);
	if( gui_return_value != 0 ) {
		return gui_return_value;
	}
	
	if( font == NULL) {
		fprintf(stderr, "font NULL\n");
	}

	//main loop variables setup
	bool running;
	cli ? running = false : running = true;

	bool patch_button_pressed = false;
	bool promt_yes_button_pressed = false;
	bool promt_no_button_pressed = false;

	//Scene setup
	SDL_Color white_color = {0xFF,0xFF,0xFF,0xFF};
	SDL_Color red_color = {0xFF, 0x00, 0x00, 0xFF};
	SDL_Color green_color = {0x00, 0xFF, 0x00, 0xFF};
	SDL_Color yellow_color = {0xFF,0xFF,0x00,0xFF};
	bool patch_button_state = true;

	int main_scene_row = 0;
	int promt_scene_row = 0;
	
	Scene main_scene, promt_scene;
	if(!cli) main_scene.add_text(new Text("Tr3gold MMX bypass patch", FONT_SIZE*(main_scene_row++), white_color, font));
	
	if(files_status != 0) {
		if( files_status & ERROR_INPUT_FILE_NOT_FOUND) {
			main_scene.add_text(new Text("Error: Input file not found", FONT_SIZE*(main_scene_row++), red_color, font));
			patch_button_state = false;
		}

		if( files_status & ERROR_CANNOT_OPEN_BACKUP_FILE) {
			promt_scene.add_text(new Text("Error: Cannot open backup file", FONT_SIZE*(promt_scene_row++), red_color, font));
		}

		if( files_status & ERROR_UNKNOWN_MESSAGE_DIGEST_MD5) {
			promt_scene.add_text(new Text("Error: Unknown message digest md5", FONT_SIZE*(promt_scene_row++), red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_CREATE_FAILED) {
			promt_scene.add_text(new Text("Error: Digest create failed", FONT_SIZE*(promt_scene_row++), red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED) {
			main_scene.add_text(new Text("Error: Digest initialization failed", FONT_SIZE*(main_scene_row++), red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_UPDATE_FAILED) {
			promt_scene.add_text(new Text("Error: Message digest update failed", FONT_SIZE*(promt_scene_row++), red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_FINALIZATION_FAILED) {
			promt_scene.add_text(new Text("Error: Message digest finalization failed", FONT_SIZE*(promt_scene_row++), red_color, font));
		}
	
		if( files_status & ERROR_MD5_SUM_DOESNT_MATCH) {
			promt_scene.add_text(new Text("Error: Md5 checksum doesn't match", FONT_SIZE*(promt_scene_row++), red_color, font));
		}

		if(!cli) main_scene.add_text(new Text("Warning: Some test didn't pass", FONT_SIZE*(main_scene_row++), yellow_color, font));
		
		main_scene.add_button(new Button(600,100, 120,40, patch_button_state, &patch_button_pressed, "./assets/button.png", "Patch", gui.renderer));
		
		if(!cli) {
			promt_scene.add_button(new Button(450, 100, 120, 40, true, &promt_no_button_pressed, "./assets/button.png", "No", gui.renderer));
			promt_scene.add_button(new Button(600, 100, 120, 40, true, &promt_yes_button_pressed, "./assets/button.png", "Yes",  gui.renderer));
			promt_scene.add_text(new Text("Try to patch the file anyway?", FONT_SIZE*(promt_scene_row++), white_color, font));
		}
	}
	else {
		main_scene.add_button(new Button(600,100,120,40,true,&patch_button_pressed, "./assets/button.png", "Patch", gui.renderer));
		main_scene.add_text(new Text("Every test passed", FONT_SIZE*(main_scene_row++), green_color, font));
	}

	gui.current_scene = &main_scene;
	
	//main loop
	do {
		(cli) ? patch_button_pressed = true : patch_button_pressed = false;
		promt_no_button_pressed = false;
		promt_yes_button_pressed = false;

		gui.input(&running);

		//logic
		gui.logic();
	
		if( patch_button_pressed && (files_status != 0) ) {
			if (cli) {
				promt_scene.print();
				printf("Try to patch the file anyway [y/N]?: ");
				char c = getchar(); //flawfinder: ignore
				if( (c == 'y') || (c == 'Y') ) files_status = 0; 
			}
			else {
				gui.current_scene = &promt_scene;
			}
		}

		if( (patch_button_pressed && (files_status == 0) ) || promt_yes_button_pressed )   {
			int modifier_return_value = tr3gold_modifier.replace_bytes(target, target_size, replace, replace_size);
			
			if(modifier_return_value != 0) {
				main_scene.clear();
				main_scene_row = 0;
				main_scene.add_text(new Text("Error while patching the file", FONT_SIZE*(main_scene_row++), red_color, font));
			
				if( modifier_return_value & ERROR_INPUT_BUFFER_ALLOCATION) main_scene.add_text(new Text("Error: failed to allocate bugger for input file", FONT_SIZE*(main_scene_row++), red_color, font));
				if( modifier_return_value & ERROR_READING_INPUT_FILE) main_scene.add_text(new Text("Error: while reading input file", FONT_SIZE*(main_scene_row++), red_color, font));
				if( modifier_return_value & ERROR_MULTIPLE_TARGET_LOCATION) main_scene.add_text(new Text("Error: Multiple target location found", FONT_SIZE*(main_scene_row++), red_color, font));
				if( modifier_return_value & ERROR_WRITING_BACKUP_FILE) main_scene.add_text(new Text("Error: while creating backup file", FONT_SIZE*(main_scene_row++), red_color, font));
				if( modifier_return_value & ERROR_TARGET_NOT_FOUND) main_scene.add_text(new Text("Error: Target bytes were not found", FONT_SIZE*(main_scene_row++), red_color, font));
				if( modifier_return_value & ERROR_MODIFYING_FILE) main_scene.add_text(new Text("Error: Couldn't modify the file", FONT_SIZE*(main_scene_row++), red_color, font));
				
				gui.current_scene = &main_scene;
			}
			else {
				main_scene.clear();
				main_scene_row = 0;
				main_scene.add_text(new Text("Sucess, the file was patched", FONT_SIZE*(main_scene_row++), green_color, font));
				gui.current_scene = &main_scene;
			}

		}

		if(promt_no_button_pressed) {
			gui.current_scene = &main_scene;
		}
		
		//render
		if(cli) {
			gui.print();
		}
		else {
			gui.render(font);
		}

	} while(running);
	
	main_scene.clear();
	promt_scene.clear();

	return 0;
}
/// \endcond

