# TACTCON 9000 - Contact information & address manager

TACTCON9000 helps users manage their contacts on the personal computer
with a user-friendly interface. Contacts hold information about the name,
email address, city of residence, phone number and kin status in relation to
the user. This information is stored in a save file located at the directory of the
program, which contains human-readable and editable text. New contacts can
be saved and recalled at will, and the program asks the user if they want to
synchronize the changes between the save file and program memory when
adding or removing entries.

Each contact holds a unique name, which is used as an ID when removing
entries from the program memory or save file. Additionally users can perform
a search for contacts in a given city, or even wipe the whole plate clean save
files and all.

- Included files (main.cpp, Contact-class [Contact.h / Contact.cpp], Addressbook-class [Addressbook.h / Addressbook.cpp], InputHandler)

- template class with static functions for input parsing [InputHandler.h])

- Every time the program is run, if it doesn’t find an existing save file, it will ask the user if they want to create a new file (named contacts.txt in the
  directory of the source code).
  
- Save file contains contacts in rows, with semicolons separating data fields.

- The program uses overloaded streaming operators << | >> for both the Addressbook and Contact classes. They allow for streaming
