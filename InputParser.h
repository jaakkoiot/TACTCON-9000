#include <iostream>			//cin, cout, endl
#include <string>			//string, getline, string hash, etc
#include <sstream>			//stringstream
#include <iomanip>			//setw

#define NAME_LEN 20			//max width of all read strings for member variables name, email, city, phonenumber (NOTE: name and city share their length)
#define EMAIL_LEN 35
#define PHONENUMBER_LEN 17
#define BOOLEAN_LEN 1

#define MIN_NAME 4			//min width for inputs
#define MIN_EMAIL 7
#define MIN_PHONE 7

#define READ_EMAIL 0		//user input parsing options (name & city share the same requirements)
#define READ_NAME 1
#define READ_PHONENUMBER 2

//string representations used for storing the Contact boolean value 'relative' to the savefile string ans printing it out
#define STRING_IS_RELATIVE "relative"
#define STRING_NOT_RELATIVE	"unrelative"
#define STRING_DELIM ':'	//delimiter char used in separating the savefile Contact member strings

#define WHITESPACE " \n\r\t\f\v"

/*This file 'InputParser.h' is not a header file in the traditional sense, as it contains static functions that include the definition in addition to declaration, 
the reason it's not "Input.parser" is because my IDE plays nice with a traditional .h file format. Most of the definitions are found here because they are 
largely in use parsing user input with different functionality, this file is also shared among most of the other files, so the common includes are here 
(inclusions only used in different parts of the program are located in the respective headers)*/

#pragma once
template<typename T>			/*The Template type is not utilised to it's fullest potential as most of the input reads are strings with differt specifications (like whitespace allowed)*/
class InputHandler
{
public:
    static T read_input();						//general templated input reading function
	static bool read_boolean();					//boolean read is a special case as it transforms the user's input into a different type, bool
	static std::string read_string(int type);	/*Passed type variable is used when checking for different input for name, email and phone number in the function.
												Type 0 = email, type 1 = name/city, type 2 = phone number, type 3 bool y/n | Y/N*/
	static std::string remove_trailing_whitespace(const std::string &str);
};

//This is a general input fetching function that can populate most variable types (ie. int, char, float, string cutting off at whitespace)
template<typename T>
inline T InputHandler<T>::read_input()
{
	std::string inputfield;
	T r;
	while (true)
	{
		std::getline(std::cin, inputfield);
		std::stringstream myStream(inputfield);
		if ((myStream >> r))
			break;
		std::cerr << "Invalid input, please try again" << std::endl;
	}
	return r;
}

template<typename T>
inline bool InputHandler<T>::read_boolean()
{
	std::string inputfield;
	while (true) {
		std::getline(std::cin, inputfield);
		if ((inputfield.find_first_not_of("yYnN") != std::string::npos) || inputfield.size() > BOOLEAN_LEN)
		{
			std::cerr << "Invalid input. Only a single character y/n is allowed.\nTry again:" << std::endl;
		}
		else {
			if (inputfield.find('y') != std::string::npos || inputfield.find('Y') != std::string::npos) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}

//The allowed characters and ranges are defined in this reading function as part of an if-statement (fails if find-first_not_off() is elsewhere than end point, checks ranges)
template<typename T>
inline std::string InputHandler<T>::read_string(int type)
{
	if (type == READ_NAME) {
		std::cout << "(Expected input: a-z | A-Z | space | - | ' | ´ are allowed. Max length: " << NAME_LEN << ", min length: " << MIN_NAME << ")" << std::endl;
	}
	else if (type == READ_EMAIL) {
		std::cout << "(Expected input: a-z | A-Z | 0-9 | _ | - | + | ' | ´ | @ | ! | ? | . | & | % are allowed. Max length: " << EMAIL_LEN << ", min length: " << MIN_EMAIL << ")" << std::endl;
	}
	else if (type == READ_PHONENUMBER){
		std::cout << "(Expected input: + | 0-9 | space | - are allowed. Max length: " << PHONENUMBER_LEN << ", min length: " << MIN_PHONE << ")" << std::endl;
	}

	std::string inputfield;
	while (true)
	{
		std::getline(std::cin, inputfield);
		if (type == READ_EMAIL) {
			if ((inputfield.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-+´'@!?.&%1234567890") != std::string::npos) || inputfield.size() > EMAIL_LEN  || inputfield.size() < MIN_EMAIL)
			{
				std::cerr << "Invalid input. Only alphabets (a-z/A-Z), numbers (0-9) and special symbols _, -,+, ´, ', @, !, ?, ., &, % are allowed.\nMax length is: " << EMAIL_LEN << " and min length is: " << MIN_EMAIL <<" characters.\nTry again:" << std::endl;
			}
			else {
				return inputfield;
			}
		}
		else if (type == READ_NAME) {
			if ((inputfield.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ -´'") != std::string::npos) || inputfield.size() > NAME_LEN || inputfield.size() < MIN_NAME)
			{
				std::cerr << "Invalid input. Only alphabets (a-z/A-Z), space and -, ´, ' are allowed.\nMax length is: " << NAME_LEN << " and min length is: " << MIN_NAME << " character.\nTry again:" << std::endl;
			}
			else {
				return inputfield;
			}
		}
		else if (type == READ_PHONENUMBER){
			if ((inputfield.find_first_not_of("+-0123456789 ") != std::string::npos) || inputfield.size() > PHONENUMBER_LEN || inputfield.size() < MIN_PHONE)
			{
				std::cerr << "Invalid input. Allowed symbols are numbers 0-9, symbols + and - and space.\nMax length is: " << PHONENUMBER_LEN << " and min length is: " << MIN_PHONE << " characters.\nTry again:" << std::endl;
			}
			else {
				return inputfield;
			}
		}
	}
}

template<typename T>
std::string InputHandler<T>::remove_trailing_whitespace(const std::string &str)
{
	size_t end = str.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}
