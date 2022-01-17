#include "Addressbook.h"

//function that every new read contact gets passed through, checks that there are no template strings present as member variables and that there are no entries with the same name already present
bool Addressbook::authenticate(const Contact* c) const
{
	if (c->is_valid()) {
		for (auto i : list) {
			if (i == *c) {
				return false;
			}
		}
		std::vector<Contact> file_contents;
		for (auto n : file_contents) {
			if (n == *c) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//function to fetch the address list/depository of the savefile, calls the stream operator to stream file into Addressbook (and to individual contact objects)
Depository Addressbook::get_savefile_contents()
{
	std::fstream file;
	Addressbook file_content;
	file.open(FILENAME);
	if (!file) {
		std::cerr << "Can't access the savefile.\nUnable to receive saved contacts.\n" << std::endl;
	}
	else {
		std::streamsize size = get_filesize();
		file >> file_content;
	}
	return file_content.list;
}

//appends the savefile with the passed Addressbook object, which will get streamed to the file << (stream calls Contact-object stream for each member of the set)
void Addressbook::append_save(Addressbook to_save)
{
	std::ofstream file;
	file.open(FILENAME, std::ostream::out | std::ostream::app);
	if (!file) {
		std::cerr << "Error accessing file.\n" << std::endl;
		return;
	}
	else {
		std::cout << "File opened." << std::endl;
		if (to_save.list.size() > 0) {
			file << to_save;
			std::cout << "Entries saved to file." << std::endl;
			file.close();
		}
		else {
			std::cout << "There's nothing to save. Start by creating some contacts (main menu 4)." << std::endl;
			file.close();
		}
	}
}

//empties file by opening it with std::ofstream::out | trunc to truncate while writing nothing
void Addressbook::empty_file()
{
	std::ofstream ofs;
	ofs.open(FILENAME, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

//Overwrites the savefile opening it in truncating mode / empty_file() is preserved as a separate function call as it's also used elsewhere
void Addressbook::overwrite_save(Depository to_save)
{
	std::ofstream ofs;
	ofs.open(FILENAME, std::ofstream::out | std::ofstream::trunc);
	
	for (auto c : to_save) {
		ofs << c;
	}
	std::cout << "Savefile overwritten." << std::endl;
}

/*Megafunction synchronizes the save file list and the running program list together while discarding duplicates. This was perhaps the hardest to get working properly, 
and as a result there's a high chance some redundant loops are called through the different depositories/lists. Looking back most of my problems syncing the 
savefile came from opening the savefile in incorrect std::ofstream mode.*/
void Addressbook::synchronize()
{
	Depository savelist = get_savefile_contents();	//fetch savefile's content from function call also used elsewhere
	Depository thislist = this->list;				//combination vector, at first holding only this Addressbook's vector

	Depository newprogramlist;						//new depository/list that will become this running program's list
	Depository newsavefilelist;						//likewise a new depository/list for saved contacts, that will be saved at the end of the function

	bool check;										//bool flag for checking if there are entries not present in the program list

	for (auto x : thislist) {
		newprogramlist.insert(x);
	}
	for (auto n : newprogramlist) {
		newsavefilelist.insert(n);
	}

	//first go through savefile and select objects not present in programlist
	for (auto a : savelist) {
		newprogramlist.insert(a);
		check = (thislist.find(a) != thislist.end());
		if (check) {
			auto result = newsavefilelist.insert(a);
		}
	}

	for (auto b : newsavefilelist) {
		newprogramlist.insert(b);
	}

	this->list = newprogramlist;					//set 'this' (running programs) set to the resulting set
	Addressbook newsavebook(newsavefilelist);		//creating new Addressbook-object with the new set
	append_save(newsavebook);						//append savefile contents with the passed new Addressbook
}

//adds a passed contact to depository and asks wether the user wants to save it in the savefile
void Addressbook::add_to_book(const Contact c)
{
	list.insert(c);
	std::cout << "Entry added to program memory!\nSave new contact to file? y/n\n" << std::endl;
	bool savechoice = InputHandler<void>::read_boolean();
	if (savechoice) { synchronize(); }
	else { std::cout << "The entry will be lost if you close the program without saving program memory to file.\nYou can perform saving from main menu option: " << MENU_SAVE << "." << std::endl; }

}

//creating new contact via the Contact-class read_data()
void Addressbook::create_new_contact()
{
	Contact new_entry;
	bool success = false;
	new_entry.read_data();
	std::cout << "Confirm the information is correct: y/n\n" << std::endl;
	success = InputHandler<void>::read_boolean();

	//authenticate lets the user know there's a duplicate present, instead of just discarding the object, "for my next project", the user would be asked if this or the duplicate will be saved
	if (authenticate(&new_entry)) {
		success ? add_to_book(new_entry) : new_entry.clear_data();
	}
	else {
		std::cout << "There is already an entry with the same name.\nEntry will not be saved." << std::endl;
		new_entry.clear_data();
	}
}

/*to remove a specific Contact entry, the program first synchronizes the state of program entriesand savefile entries, and then changes the program's list to 
reflect the change, and then overwrites the savefile with the new list. */
void Addressbook::remove_contact()
{
	bool erased = false;
	Depository savefile_list;
	std::string searched;

	std::cout << "Name of the contact you wish to remove?" << std::endl;
	searched = InputHandler<void>::read_string(READ_NAME);
	const Contact to_remove(searched);

	for (auto n : list) {
		std::string candidate;
		candidate = n.get_name();
		if (candidate == searched) {
			std::cout << "Found an entry for name: " << searched << std::endl;
			list.erase(n);
			erased = true;
			std::cout << "Removed." << std::endl;
			//break is called here as the names are unique / for-loop goes out of range when erasing a member
			break;
		}
	}

	if (!erased) {
		std::cout << "No matches loaded into program memory. Remove from " << searched << " from savefile? y/n" << std::endl;
	}
	else {
		std::cout << "Remove the entry from savefile also? y/n" << std::endl;
	}
	bool choice = InputHandler<void>::read_boolean();

	//If user so wishes, the entry will also get removed from the savefile (which is the place it matters most by all accounts)
	if (choice) {
		erased = false;
		std::fstream file;
		savefile_list = get_savefile_contents();

		auto f_iterator = savefile_list.begin();
		for (auto s : savefile_list) {
			std::string candidate;
			candidate = s.get_name();
			if (candidate == searched) {
				std::cout << "Found an entry for name: " << searched << std::endl;
				savefile_list.erase(s);
				erased = true;
				std::cout << "Removed." << std::endl;
				//break is called here as the names are unique / for-loop goes out of range when erasing a member
				break;
			}
		}
		if (!erased) {
			std::cout << "No matches found in the save file either. Returning to menu..." << std::endl;
		}
		else {
			std::cout << "Saving the changes to file..." << std::endl;
			overwrite_save(savefile_list);
		}
	}

}

void Addressbook::clear_contents()
{
	std::vector<Contact>::size_type no_contacts(0);
	auto s = [this]() {list.clear(); return "Program memory cleared\n"; };	//Lambda for clearing contacts and printing notification, placed in conditional statement
	list.size() == no_contacts ? std::cout << "There are no entries to clear!\n" << std::endl : std::cout << s();

	bool clearsavefile;
	std::cout << "Do you wish to also clear the savefile? y/n:" << std::endl;
	clearsavefile = InputHandler<void>::read_boolean();
	if (clearsavefile) {
		empty_file();
		std::cout << "Savefile cleared of contact information." << std::endl;
	}
}

void Addressbook::read_from_file(int state)
{
	std::fstream file(FILENAME);
	if (!file) {
		std::cout << "Can't locate existing save file.\nWould you like to create a new one? y/n:" << std::endl;
		bool choice = InputHandler<void>::read_boolean();
		if (choice) {
			std::cout << "Creating a new savefile." << std::endl;
			file.open(FILENAME, std::ios::out);
			file ? std::cout << "File created.\n" : std::cout << "Error creating file.\n";
			std::cout << std::endl;
		}
		else {
			std::cout << "Process terminated." << std::endl;
		}
	}
	else {
		std::streamsize size = get_filesize();
		if (size <= 2) { std::cout << "The file is currently empty" << std::endl; };

		if (state == FILE_READ_REG && size > 2) {
			this->list = get_savefile_contents();
			if (list.size() > 0) {
				std::cout << "Loaded savefile. There are " << list.size() << " elements in the program memory.\nDo you want to print them? y/n:\n" << std::endl;
				bool choice = InputHandler<void>::read_boolean();
				if (choice)
					print_all();
				else
					std::cout << "Returning to menu." << std::endl;
			}
			else {
				std::cout << "Savefile is empty." << std::endl;
			}
		}
	}
	file.close();

	return;
}

void Addressbook::print_all()
{
	if (list.size() == 0) {
		std::cout << "There is nothing to print. Would you like to load the savefile into program memory? y/n:" << std::endl;
		bool choice = InputHandler<void>::read_boolean();
		if (choice)
			read_from_file(FILE_READ_REG);
		return;
	}
	std::cout << std::left;
	print_contacts_header();
	for (auto &x : list) {
		if (x.get_relative()) {
			std::cout << x << std::endl;
		}
	}
	for (auto& x : list) {
		if (!x.get_relative()) {
			std::cout << x << std::endl;
		}
	}
	return;
}

//header of the screen showing print / sreach results
void Addressbook::print_contacts_header()
{
	std::cout << std::left;
	std::cout << std::setw(NAME_LEN) << "Name: " << std::setw(EMAIL_LEN) << "Email: " << std::setw(NAME_LEN) << "City: " << std::setw(PHONENUMBER_LEN) << "Phone: " << std::setw(6) << "Relative: \n" << std::endl;
}

void Addressbook::search_city_residents()
{
	std::cout << "Enter name of the city:\n" << std::endl;
	std::string city = InputHandler<void>::read_string(READ_NAME);

	std::vector<Contact> city_residents;	//compiling search results into a vector

	for (Contact c : list)
		if (c.get_city() == city) {
			city_residents.push_back(c);
		}

	if (city_residents.size() == 0) {
		std::cout << "There are no contacts located in: " << city << std::endl;
	}
	else {
		std::cout << "Found " << city_residents.size() / sizeof(Contact) << " matches:" << std::endl;
		print_contacts_header();
		for (auto& x : city_residents) {
			std::cout << x << std::endl;
		}
	}
}

//Function used to get file size in bytes
std::streamsize Addressbook::get_filesize()
{
	std::ifstream file;
	file.open(FILENAME, std::ios::in | std::ios::binary);
	file.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize size = file.gcount();
	file.clear();
	file.seekg(0, std::ios_base::beg);
	return size;
}

/*Outstream operator << the program's data is checked against the contents of the file, and entities not present in the file will be saved (streamed onto it)*/
std::ostream& operator<<(std::ostream& os, const Addressbook& c)
{
	std::stringstream ss;

	Addressbook checker;
	auto checklist = checker.get_savefile_contents();

	for (auto x : c.list) {
		if (checklist.find(x) != checklist.end()) {

		}
		else {
			ss << x;
			os << ss.str();
		}
	}
	return os;
}

/*In istream operator >> Data is read from file line by line into a string buffer 'line', that is then converted to istringstream for the Contact object the program populates,
there are single object variables spread out into a single line on the text file, and for this reason the lines are fetched with getline first before streaming them to
the objects. So there's actually two stream operators in play, both Addressbook & Contact.*/
std::istream& operator>>(std::istream& is, Addressbook& a)
{
	Addressbook instreambook;
	Depository instreamlist;
	std::string line;
	while (std::getline(is, line)) {
		Contact c;
		std::istringstream iss_line(line);
		iss_line >> c;
		a.list.insert(c);
	}
	return is;
}
