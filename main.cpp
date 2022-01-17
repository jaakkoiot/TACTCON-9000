#include "Addressbook.h"			//Addressbook-class
#include "InputParser.h"			//Input-parsing static functions used for reading user input (contains iostream[cin, cout endl], string[string, getline], sstream[stringstream] & others)

#define PROG_NAME "TACTCON9000"

/*Main program contains only the bare minimum of code, the main menu structure of the program and initial savefile check. All of the function 
calls are tied to the Addressbook-class, which the main program holds an object of. During the later parts of the programming the Addressbook-class grew to 
slightly uncomfortable size, and going forward I would probably try separating the file saving/syncing/management to a different class like the InputParser*/
int main() {
	int menu_choice = 0;
	Addressbook contacts;
	std::cout << "Welcome to \"" << PROG_NAME << "\" contact information storing application!\n" << std::endl;
	std::cout << "This program stores contact information (name, email address, city of residence, phone number and kin status) in a save file: \"" << FILENAME << "\".\n\nThe file is a regular text file located at the directory of the program in human readable format.\n" << std::endl;
	contacts.read_from_file(FILE_READ_INI);
	
	do {
		std::cout << "________________________________________________________________________________________________________________" << std::endl;
		std::cout << "\nPress " << MENU_INIT << "-" << MENU_CLOSE << "\n\n1: Clear all contact information from program memory / init state\n2: Load contact information from savefile\n3: Save program's contact information to savefile\n4: Create a new entry\n5: Remove a contact\n6: Search contacts in a specific city\n7: Print all contact information\n8: End program" << std::endl;
		menu_choice = InputHandler<int>::read_input();
		switch (menu_choice) {
		case MENU_INIT:
			contacts.clear_contents();
			break;
		case MENU_LOAD:
			contacts.read_from_file(FILE_READ_REG);
			break;
		case MENU_SAVE:
			contacts.synchronize();
			break;
		case MENU_NEW:
			contacts.create_new_contact();
			break;
		case MENU_REMOVE:
			contacts.remove_contact();
			break;
		case MENU_SEARCHCITY:
			contacts.search_city_residents();
			break;
		case MENU_PRINTALL:
			contacts.print_all();
			break;
		case MENU_CLOSE:
			std::cout << "\nProgram shutting down..." << std::endl;
			break;
		}
	} while (menu_choice != MENU_CLOSE);

	return 1;
}

