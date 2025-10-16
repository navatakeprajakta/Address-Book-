
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
    int c; while ((c = getchar()) != '\n' && c != EOF);

    Contact *newC = &addressBook->contacts[addressBook->contactCount];
    char temp[50]; int valid;

    // ---- Name ----
    printf("Enter name: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    strcpy(newC->name, temp);

    // ---- Phone ----
    do {
        valid = 1;
        printf("Enter phone (10 digits): ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        if (strlen(temp) != 10) { printf("Must be 10 digits!\n"); valid = 0; continue; }
        for (int i = 0; i < 10; i++) if (!isdigit(temp[i])) valid = 0;
        for (int i = 0; i < addressBook->contactCount && valid; i++)
            if (strcmp(addressBook->contacts[i].phone, temp) == 0) { printf("Phone exists!\n"); valid = 0; }
    } while (!valid);
    strcpy(newC->phone, temp);

    // ---- Email ----
    do {
        valid = 1;
        printf("Enter email: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strpbrk(temp, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) { printf("Use lowercase only.\n"); valid = 0; continue; }
        if (strchr(temp, '@') == NULL || strstr(temp, ".com") == NULL) { printf("Invalid format.\n"); valid = 0; continue; }
        char *at = strchr(temp, '@'), *dot = strstr(temp, ".com");
        if (dot - at <= 1) { printf("Need chars between '@' and '.com'\n"); valid = 0; continue; }
        if (dot != temp + strlen(temp) - 4) { printf("Must end with .com\n"); valid = 0; continue; }

        for (int i = 0; temp[i]; i++) {
            char c = temp[i];
            if (!(isalnum(c) || c == '@' || c == '.' || c == '_' || c == '-')) {
                printf("Invalid char '%c'\n", c); valid = 0; break;
            }
        }
        for (int i = 0; i < addressBook->contactCount && valid; i++)
            if (strcmp(addressBook->contacts[i].email, temp) == 0) { printf("Email exists!\n"); valid = 0; }
    } while (!valid);
    strcpy(newC->email, temp);

    addressBook->contactCount++;
    printf("✅ Contact added!\n");
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
        printf("Contact not found.\n");
        return;
    }

    Contact *c = &addressBook->contacts[idx];
    printf("Editing contact:\n1. Name: %s\n2. Phone: %s\n3. Email: %s\n4. Cancel\n", 
           c->name, c->phone, c->email);

    int choice;
    printf("Choose field to edit: ");
    scanf("%d", &choice);
    getchar();

    char input[50];
    switch (choice) {
        case 1:
            printf("Enter new name: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strcpy(c->name, input);
            printf("Name updated!\n");
            break;

        case 2:
            printf("Enter new phone (10 digits): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strcpy(c->phone, input);
            printf("Phone updated!\n");
            break;

        case 3:
            printf("Enter new email: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strcpy(c->email, input);
            printf("Email updated!\n");
            break;

        case 4:
            printf("Edit cancelled.\n");
            return;

        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("Contact updated successfully!\n");
}


// -------- Delete Contact --------
void deleteContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) { printf("No contacts.\n"); return; }

    char phone[20]; printf("Enter phone to delete: ");
    scanf("%s", phone);

    int idx = -1;
    for (int i = 0; i < addressBook->contactCount; i++)
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) idx = i;

    if (idx == -1) { printf("Contact not found.\n"); return; }

    for (int i = idx; i < addressBook->contactCount - 1; i++)
        addressBook->contacts[i] = addressBook->contacts[i + 1];

    addressBook->contactCount--;
    printf("Contact deleted.\n");
}

