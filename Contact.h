#include "InputParser.h"
#include <fstream>
#include <vector>

#define TEMPLATE_STRING "template"	//template value for the string members

#define NAME_POS 0
#define EMAIL_POS 1
#define CITY_POS 2
#define PHONE_POS 3
#define RELATIVE_POS 4

/*Basic data entity of the program, Contact contains persons name, email, phone number, city and a flag if they are a relative or not.
In a more complex system with multiple data points personal information could be further separated into Person-class with Contact holding additional 
handles like email and phone, but as this program is quite simple it's implementation is a single class*/

#pragma once
class Contact
{
	friend std::ostream& operator<<(std::ostream& os, const Contact& c);	//Outstream operator for printing contact info
	friend std::istream& operator>>(std::istream& is, Contact& p);			//Instream operator >> for parsing Contact-objects from stream
	friend bool operator==(const Contact& lhs, const Contact& rhs);			//Equal to (=) operator overload compares the objects name privare variables

public:
	Contact(std::string nm = TEMPLATE_STRING, std::string em = TEMPLATE_STRING, std::string nmbr = TEMPLATE_STRING, std::string ct = TEMPLATE_STRING, bool relat = false) : name(nm), email(em), phone_nmbr(nmbr), city(ct), relative(relat) {};

	void set_name(std::string nm) { name = nm; };
	void read_data();		//Populate member variables from user input
	void clear_data() { name = "", email = "", phone_nmbr = "", city = ""; std::cout << "Entry cleared.\n" << std::endl; };	//Cleared "empty" entries will not be saved to the savefile or Addressbook's list
	std::string is_relative() const { std::string output; relative ? output = STRING_IS_RELATIVE : output = STRING_NOT_RELATIVE; return output; };	//Used to represent relative bool status as a string

	bool is_valid() const;													//Function to check all member values are populated and no template strings are present
	bool operator<(const Contact& evaluate) const;							//Less than operator requred for vector sorting when removing duplicates, returns the Contact with shorter combined string lengths (completely arbitrary as there's no actual way to compare strings in a less than manner)

	std::string get_city() const { return this->city; };
	std::string get_name() const { return this->name; };
	bool get_relative() const { return relative; };
private:
	std::string name;
	std::string email;
	std::string phone_nmbr;
	std::string city;
	bool relative;
};
