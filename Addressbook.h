#include <vector>		//vector
#include <algorithm>	//find_first_of
#include <unordered_set>//unordered set
#include "Contact.h"	//Contact-class

#define FILENAME "contacts.txt"				//Name of the save file the program stores the contact information on
#define FILE_READ_INI 0						//different 'modes' of reading from savefile, initial program check, regular read/load, and read for only fetching the savefile's list
#define FILE_READ_REG 1
#define FILE_READ_FETCH 2

#define MENU_INIT 1							//Menu items are defined in Addressbook header, as they are referenced in both main() and addressbooks member functions
#define MENU_LOAD 2
#define MENU_SAVE 3
#define MENU_NEW 4
#define MENU_REMOVE 5
#define MENU_SEARCHCITY 6
#define MENU_PRINTALL 7
#define MENU_CLOSE 8

#define CHECK_FOR_NAME "temp"

//As the names are enforced to be unique, this is a universal hasher for Contacts
struct ContactHasher
{
	size_t operator()(const Contact& obj) const
	{
		return std::hash<std::string>()(obj.get_name());
	}
};

//Comparator returns true if the Contacts share the same name
struct ContactComparator
{
	bool operator()(const Contact& a, const Contact& b) const
	{
		if (a.get_name() == b.get_name())
			return true;
		return false;
	}
};

typedef std::unordered_set<Contact, ContactHasher, ContactComparator> Depository;		//typedef makes the unordered_list with additional parameters a bit friendlier

/*This class represents the addressbook that can be saved and read from the savefile. There is only one private member, the list of Contacts 
the program helps users to manage. In hindsight I would probably separate the file management for a separate object, as many of the funcions here relate 
more to file management than this particular object.
--------------------------------------------------------------------------------------------------------------------------------------------------------
Stream operators >> | << are used at this level (they both call the underlying Contact stream operators!)to read from and populate the save file. This was 
a substantial undertaking at my skill level and was perhaps the most challenging coding project yet. Nevertheless I'm happy to finally get it to work, as 
the program writes to the savefile without duplicates. Savefile duplication management was messy but crucially important, as the file can't be endlessly 
appended with the programs contents and simply truncating the file results in it not really being a savefile.
--------------------------------------------------------------------------------------------------------------------------------------------------------
Unordered set was used for containing the Contacts in a list that automatically neglects objects that have the same name as one object already in the set, 
custom hasher and comparators are structed above, although I've overloaded the comparison function for Contact already, so they shouldn't be needed.*/
#pragma once
class Addressbook
{
	friend std::ostream& operator<<(std::ostream& os, const Addressbook& c);
	friend std::istream& operator>>(std::istream& is, Addressbook& a);
public:
	Addressbook() : list{ 0 } { std::unordered_set<Contact, ContactHasher, ContactComparator> setOfStudByName; };
	Addressbook(std::unordered_set<Contact, ContactHasher, ContactComparator> passed_list) { list = passed_list; };

	bool authenticate(const Contact* c) const;			//Function for authenticating a Contact object
	std::unordered_set<Contact, ContactHasher, ContactComparator> get_savefile_contents();

	void empty_file();
	void append_save(Addressbook to_save);		//replaces the list with passed new list, and overwrites the savefile with it
	void overwrite_save(Depository to_save);	//When removing single entries this function is called to overwrite the save with passed Depository-list
	void synchronize();							//Function reads the savefile, compares to the program memory and synchronizes them
	void add_to_book(const Contact c);
	void create_new_contact();
	void remove_contact();
	void clear_contents();
	void read_from_file(int state);
	void print_all();
	void print_contacts_header();
	void search_city_residents();

	std::streamsize get_filesize();
private:
	Depository list;
};