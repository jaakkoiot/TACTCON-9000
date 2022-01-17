#include "Contact.h"

//gathers data from user input to populate member variables, works closely with InputParser
void Contact::read_data()
{
	std::cout << "Enter contact name:" << std::endl;
	this->name = InputHandler<void>::read_string(READ_NAME);	//passed variables set the functionality of the validation (allowed symbols + ranges), name and city share their requirements
	std::cout << "Enter " << this->name << "'s email address: " << std::endl;
	this->email = InputHandler<void>::read_string(READ_EMAIL);	//Passed is_email variable is true as email is more lenient with more special characters and numbers but no spaces allowed
	std::cout << "Enter " << this->name << "'s city of residence: " << std::endl;
	this->city = InputHandler<void>::read_string(READ_NAME);	//ity follows the same restrictions as contact name ^
	std::cout << "Enter " << this->name << "'s phone number: " << std::endl;
	this->phone_nmbr = InputHandler<void>::read_string(READ_PHONENUMBER);	//Only numbers and +, - allowed for phone numbers
	
	std::cout << "Is " << this->name << " your relative? y/n" << std::endl;
	this->relative = InputHandler<void>::read_boolean();
	std::cout << *this << std::endl;
}

//Function for checking a contact object if it has all member variabes populated and not templates
bool Contact::is_valid() const
{
	if (name != TEMPLATE_STRING && !name.empty() && email != TEMPLATE_STRING && !email.empty() && city != TEMPLATE_STRING && !city.empty() && phone_nmbr != TEMPLATE_STRING && !phone_nmbr.empty()) {
		return true;	//Return true if all member variables are populated and no template strings are found => the object is read whole
	}
	else {
		return false;	//In case of a false entry the Contact will not be added to addressbook
	}
}

/*Output stream operator that handles writing Contact information to file (a single line for a single object member variables).
It is generally bad practice to change stream's functionality depending on whether it is writing to a file or somewhere else, but
it's implemented here to keep code concise and stream is not being directed to anything other than file/cout by this program anyways.
There's a very subtle change between the modes, as only the delimiters are added to file stream. This helps keep the text file easier to read 
by a user, as the individual data fields are spaced evenly in the text.*/
std::ostream& operator<<(std::ostream& os, const Contact& c)
{
	if (&os == &std::cout) {
		//cout printing without delimiters:
		os << std::left;
		os << std::setw(NAME_LEN) << c.name  << std::setw(EMAIL_LEN) << c.email  << std::setw(NAME_LEN) << c.city << std::setw(PHONENUMBER_LEN) << c.phone_nmbr << c.is_relative() << std::endl;
	}
	else {
		//file printing with delimiters:
		os << std::left;
		os << std::setw(NAME_LEN) << c.name << STRING_DELIM << std::setw(EMAIL_LEN) << c.email << STRING_DELIM << std::setw(NAME_LEN) << c.city << STRING_DELIM << std::setw(PHONENUMBER_LEN) << c.phone_nmbr << STRING_DELIM << c.is_relative() << STRING_DELIM << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, Contact& c)
{
	std::string field;
	std::vector<std::string> separated_fields;


	while (std::getline(is, field, STRING_DELIM)) {		//split string on delimiters
		separated_fields.push_back(field);
	}
	//place data members in place & remove trailing whitespace to make comparison between user input and loaded data possible in the program
	c.name = separated_fields[NAME_POS];				//name
	c.name = InputHandler<void>::remove_trailing_whitespace(c.name);

	c.email = separated_fields[EMAIL_POS];				//email
	c.email = InputHandler<void>::remove_trailing_whitespace(c.email);

	c.city = separated_fields[CITY_POS];				//city
	c.city = InputHandler<void>::remove_trailing_whitespace(c.city);

	c.phone_nmbr = separated_fields[PHONE_POS];			//phone number
	c.phone_nmbr = InputHandler<void>::remove_trailing_whitespace(c.phone_nmbr);

	//relative bool stat is set based on the saved string
	if (separated_fields[RELATIVE_POS] == STRING_IS_RELATIVE) {
		c.relative = true;
	}
	else {
		c.relative = false;
	}

	return is;
}

/*Comparison operator compares the name parameters of the objects, as it is the only parameter the program enforces the need to be unique. Unique
parameter name is useful when searching for specific object to remove from the list, as each name holder is inherently unique.*/
bool operator==(const Contact& lhs, const Contact& rhs)
{
	return lhs.name == rhs.name;
}

//lesser than operator returns true if *this object has lesser combined member length than the comparison object, in other words it's an arbitrary comparison
bool Contact::operator<(const Contact& evaluate) const
{
	size_t thislength = name.size() + email.size() + city.size() + phone_nmbr.size();
	size_t thatlength = evaluate.name.size() + evaluate.email.size() + evaluate.city.size() + evaluate.phone_nmbr.size();

	if (thislength < thatlength)
		return true;
	else
		return false;
}