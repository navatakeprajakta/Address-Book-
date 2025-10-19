**📒 Address Book(C Project)**
**Overview**

This is a C-based Address Book application that helps users manage their contacts from the terminal.
You can add, search, edit, delete, and list contacts.
All contact data is stored in a CSV file (Addressbook.csv) and saved automatically when you exit the program.

**Features**
✅ Create New Contacts
Stores Name, Phone, and Email
Validates inputs before adding
Prevents duplicate phone numbers and emails

✅ Search Contacts
Search by Name, Phone, or Email
Case-insensitive matching for names and emails

✅ Edit Contacts
Update Name, Phone, or Email
Includes validation for all fields

✅ Delete Contacts
Delete using a phone number
Asks for confirmation before deleting

✅ Save on Exit
All contacts are written to Addressbook.csv when you choose option 6. Exit
Data automatically loads back the next time the program starts

**📁 Project Structure**
**AddressBook/**
<br>
│
<br>
├── main.c              # Menu & program control
<br>
├── contact.c           # Core operations (create, search, edit, delete)
<br>
├── contact.h           # Data structures & function declarations
<br>
├── file.c              # File I/O for saving and loading contacts
<br>
├── file.h              # Declarations for file functions
<br>
├── populate.c          # (Optional) Dummy data population
<br>
├── populate.h          # Header for dummy data
<br>
├── Addressbook.csv     # CSV file storing saved contacts
<br>
└── README.md           # Project documentation

⚙️ How to Compile and Run
🖥️ Using GCC

Open a terminal inside your project folder and run:
gcc main.c contact.c file.c -o addressbook
./addressbook

**** Menu Options****
Address Book Menu:
1. Create contact
2. Search contact
3. Edit contact
4. Delete contact
5. List all contacts
6. Exit

After choosing 6. Exit, all changes are saved to Addressbook.csv.

** Validation Rules**
Field	Validation
Name	Only letters and spaces (min 2 characters)
Phone	Exactly 10 digits, digits only, no duplicates
Email	Lowercase only, must contain ‘@’ and end with ‘.com’, unique

** Example Run**
Address Book Menu:
1. Create contact
2. Search contact
3. Edit contact
4. Delete contact
5. List all contacts
6. Exit
Enter your choice: 1

Enter name: Jane Doe
<br>
Enter phone (10 digits): 9876543210
<br>
Enter email: jane.doe@gmail.com
<br>
✅ Contact added successfully!

Enter your choice: 6
Saving and Exiting...
Contacts saved to Addressbook.csv


**👨‍💻 Developed By
**
Name: Prajakta Navatake 
Date: October 2025
Language: C (using GCC)
Data Persistence: CSV File I/O

**🏁 Future Enhancements**
Add auto-save after every create/edit/delete
Implement sorting (by name or phone)
Add password protection
Support JSON or SQLite storage
