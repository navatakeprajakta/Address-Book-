
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"

// -------- Display All Contacts --------
void listContacts(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("No contacts found.\n");
        return;
    }
    printf("\n%-5s %-20s %-15s %-30s\n", "No.", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < addressBook->contactCount; i++)
        printf("%-5d %-20s %-15s %-30s\n", i+1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    printf("-----------------------------------------------------------\nTotal contacts: %d\n", addressBook->contactCount);
}

// -------- Initialize Address Book --------
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

// -------- Create Contact with Validation --------
void createContact(AddressBook *addressBook) {
    if (addressBook->contactCount >= MAX_CONTACTS) {
        printf("Address book full!\n");
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // clear input buffer

    Contact *newC = &addressBook->contacts[addressBook->contactCount];
    char temp[50];
    int valid;

    // ---- NAME VALIDATION ----
    do {
        valid = 1;
        printf("Enter name: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';

        // name should contain only letters & spaces
        if (strlen(temp) < 2) {
            printf("❌ Name too short! Please enter at least 2 characters.\n");
            valid = 0;
            continue;
        }
        for (int i = 0; temp[i]; i++) {
            if (!isalpha(temp[i]) && temp[i] != ' ') {
                printf("❌ Invalid name! Use only letters and spaces.\n");
                valid = 0;
                break;
            }
        }
    } while (!valid);
    strcpy(newC->name, temp);

    // ---- PHONE VALIDATION ----
    do {
        valid = 1;
        printf("Enter phone (10 digits): ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strlen(temp) != 10) {
            printf("❌ Phone number must be exactly 10 digits.\n");
            valid = 0;
            continue;
        }
        for (int i = 0; i < 10; i++) {
            if (!isdigit(temp[i])) {
                printf("❌ Phone must contain digits only.\n");
                valid = 0;
                break;
            }
        }
        // check duplicate phone
        for (int i = 0; i < addressBook->contactCount && valid; i++) {
            if (strcmp(addressBook->contacts[i].phone, temp) == 0) {
                printf("❌ This phone number already exists!\n");
                valid = 0;
                break;
            }
        }
    } while (!valid);
    strcpy(newC->phone, temp);

    // ---- EMAIL VALIDATION ----
    do {
        valid = 1;
        printf("Enter email: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strpbrk(temp, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
            printf("❌ Use lowercase only in email.\n");
            valid = 0;
            continue;
        }
        if (strchr(temp, '@') == NULL || strstr(temp, ".com") == NULL) {
            printf("❌ Invalid format! Email must contain '@' and end with '.com'.\n");
            valid = 0;
            continue;
        }
        char *at = strchr(temp, '@');
        char *dot = strstr(temp, ".com");
        if (dot - at <= 1) {
            printf("❌ Need at least one character between '@' and '.com'.\n");
            valid = 0;
            continue;
        }
        if (dot != temp + strlen(temp) - 4) {
            printf("❌ Email must end with '.com'.\n");
            valid = 0;
            continue;
        }

        for (int i = 0; temp[i]; i++) {
            char c = temp[i];
            if (!(isalnum(c) || c == '@' || c == '.' || c == '_' || c == '-')) {
                printf("❌ Invalid character '%c' in email.\n", c);
                valid = 0;
                break;
            }
        }

        // check duplicate email
        for (int i = 0; i < addressBook->contactCount && valid; i++) {
            if (strcmp(addressBook->contacts[i].email, temp) == 0) {
                printf("❌ This email already exists!\n");
                valid = 0;
                break;
            }
        }
    } while (!valid);
    strcpy(newC->email, temp);

    addressBook->contactCount++;
    printf("✅ Contact added successfully!\n");
}


// -------- Search Contact --------
void searchContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) { printf("No contacts.\n"); return; }

    int opt; char term[50];
    printf("\nSearch by: 1.Name 2.Phone 3.Email 4.Back\nChoice: ");
    if (scanf("%d", &opt) != 1 || opt < 1 || opt > 4) return;
    if (opt == 4) return;
    getchar();
    printf("Enter search term: ");
    fgets(term, 50, stdin);
    term[strcspn(term, "\n")] = '\0';

    char lowTerm[50]; strcpy(lowTerm, term);
    for (int i = 0; lowTerm[i]; i++) lowTerm[i] = tolower(lowTerm[i]);

    int found = 0;
    printf("\n%-20s %-15s %-30s\n", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < addressBook->contactCount; i++) {
        char name[50], email[50];
        strcpy(name, addressBook->contacts[i].name);
        strcpy(email, addressBook->contacts[i].email);
        for (int j = 0; name[j]; j++) name[j] = tolower(name[j]);
        for (int j = 0; email[j]; j++) email[j] = tolower(email[j]);

        int match = 0;
        if (opt == 1 && strstr(name, lowTerm)) match = 1;
        else if (opt == 2 && strcmp(addressBook->contacts[i].phone, term) == 0) match = 1;
        else if (opt == 3 && strstr(email, lowTerm)) match = 1;

        if (match) {
            found++;
            printf("%-20s %-15s %-30s\n",
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
        }
    }

    if (!found) printf("No matches found.\n");
    else printf("-----------------------------------------------------------\nFound %d match(es).\n", found);
}

// -------- Edit Contact --------
void editContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("No contacts to edit.\n");
        return;
    }

    char phone[20];
    printf("Enter phone number of contact to edit: ");
    scanf("%s", phone);

    int idx = -1;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("❌ Contact not found.\n");
        return;
    }

    Contact *c = &addressBook->contacts[idx];
    printf("\nEditing contact:\n");
    printf("1. Name: %s\n", c->name);
    printf("2. Phone: %s\n", c->phone);
    printf("3. Email: %s\n", c->email);
    printf("4. Cancel\n");

    int choice;
    printf("Choose field to edit: ");
    scanf("%d", &choice);
    getchar(); // clear newline

    char input[50];
    int valid;

    switch (choice) {
        case 1: // ----- Edit Name -----
            do {
                valid = 1;
                printf("Enter new name: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';

                if (strlen(input) < 2) {
                    printf("❌ Name too short! Minimum 2 characters.\n");
                    valid = 0;
                    continue;
                }
                for (int i = 0; input[i]; i++) {
                    if (!isalpha(input[i]) && input[i] != ' ') {
                        printf("❌ Invalid name! Only letters and spaces allowed.\n");
                        valid = 0;
                        break;
                    }
                }
            } while (!valid);
            strcpy(c->name, input);
            printf("✅ Name updated successfully!\n");
            break;

        case 2: // ----- Edit Phone -----
            do {
                valid = 1;
                printf("Enter new phone (10 digits): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';

                if (strlen(input) != 10) {
                    printf("❌ Phone must be exactly 10 digits.\n");
                    valid = 0;
                    continue;
                }
                for (int i = 0; i < 10; i++) {
                    if (!isdigit(input[i])) {
                        printf("❌ Phone must contain digits only.\n");
                        valid = 0;
                        break;
                    }
                }
                // Check for duplicates (excluding current contact)
                for (int i = 0; i < addressBook->contactCount && valid; i++) {
                    if (i != idx && strcmp(addressBook->contacts[i].phone, input) == 0) {
                        printf("❌ This phone already exists.\n");
                        valid = 0;
                        break;
                    }
                }
            } while (!valid);
            strcpy(c->phone, input);
            printf("✅ Phone updated successfully!\n");
            break;

        case 3: // ----- Edit Email -----
            do {
                valid = 1;
                printf("Enter new email: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';

                if (strpbrk(input, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
                    printf("❌ Use lowercase only in email.\n");
                    valid = 0;
                    continue;
                }
                if (strchr(input, '@') == NULL || strstr(input, ".com") == NULL) {
                    printf("❌ Invalid format! Must contain '@' and end with '.com'.\n");
                    valid = 0;
                    continue;
                }

                char *at = strchr(input, '@');
                char *dot = strstr(input, ".com");
                if (dot - at <= 1) {
                    printf("❌ Need at least one character between '@' and '.com'.\n");
                    valid = 0;
                    continue;
                }
                if (dot != input + strlen(input) - 4) {
                    printf("❌ Email must end with '.com'.\n");
                    valid = 0;
                    continue;
                }

                for (int i = 0; input[i]; i++) {
                    char cch = input[i];
                    if (!(isalnum(cch) || cch == '@' || cch == '.' || cch == '_' || cch == '-')) {
                        printf("❌ Invalid character '%c' in email.\n", cch);
                        valid = 0;
                        break;
                    }
                }
                // Check duplicates (excluding same contact)
                for (int i = 0; i < addressBook->contactCount && valid; i++) {
                    if (i != idx && strcmp(addressBook->contacts[i].email, input) == 0) {
                        printf("❌ This email already exists.\n");
                        valid = 0;
                        break;
                    }
                }
            } while (!valid);
            strcpy(c->email, input);
            printf("✅ Email updated successfully!\n");
            break;

        case 4:
            printf("Edit cancelled.\n");
            return;

        default:
            printf("❌ Invalid choice.\n");
            return;
    }

    printf("💾 Contact changes saved successfully!\n");
}

// Delete contact
void deleteContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("No contacts to delete.\n");
        return;
    }

    char phone[20];
    printf("Enter phone number of contact to delete: ");
    scanf("%s", phone);

    int idx = -1;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("❌ Contact not found.\n");
        return;
    }

    Contact *c = &addressBook->contacts[idx];
    printf("\nContact found:\n");
    printf("--------------------------------------------\n");
    printf("Name : %s\n", c->name);
    printf("Phone: %s\n", c->phone);
    printf("Email: %s\n", c->email);
    printf("--------------------------------------------\n");

    char confirm;
    while (1) {
        printf("Are you sure you want to delete this contact? (y/n): ");
        scanf(" %c", &confirm);
        confirm = tolower(confirm);

        if (confirm == 'n') {
            printf("❎ Deletion cancelled. Contact kept safely.\n");
            return;
        } 
        else if (confirm == 'y') {
            break; // proceed with deletion
        } 
        else {
            printf("⚠️ Invalid input! Please type 'y' for yes or 'n' for no.\n");
        }
    }

    // Perform deletion
    for (int i = idx; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;

    printf("🗑️ Contact deleted successfully!\n");
}


