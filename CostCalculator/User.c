#define _CRT_SECURE_NO_WARNINGS
#include "User.h"

void addNewUser()
{
	FILE *users = fopen("users.txt", "a+");
	User newUser;
	char pin1[15] = { 0 }, pin2[15] = { 0 }, group[20] = { 0 };
	if (users)
	{
		printf("Add New User\nName: ");
		scanf("%s", newUser.name);
		printf("Surname: ");
		scanf("%s", newUser.surname);
		while (1)
		{
			printf("Username: ");
			scanf("%s", newUser.username);
			if (isUsernameExisting(newUser.username) >= 0)
			{
				printf("Username already exist. Enter unique username.\n");
			}
			else
			{
				break;
			}
		}
		printf("Insert 4-digit pin: ");
		strcpy(pin1, inputPin());
		if (strlen(pin1) != 4)
		{
			printf("\nError!\n");
			return;
		}
		do {
			printf("\nEnter 4-digit pin again: ");
			strcpy(pin2, inputPin());
		} while (strcmp(pin1, pin2));
		newUser.pin = xcrc32(pin1, 4, 16);
		printf("\nUser Group(Admin or Analyst): ");
		while (1)
		{
			scanf("%s", group);
			if (!strcmp(group, "admin") || !strcmp(group, "Admin") || !strcmp(group, "ADMIN"))
			{
				newUser.userGroup = Admin;
				break;
			}
			else if (!strcmp(group, "analyst") || !strcmp(group, "ANALYST") || !strcmp(group, "Analyst"))
			{
				newUser.userGroup = Analyst;
				break;
			}
			else
			{
				printf("Enter User Group Again.");
			}
		}
		if (newUser.userGroup == Admin)
		{
			fprintf(users, "%-25s %-25s %-25s %-15x %-5s\n", newUser.name, newUser.surname, newUser.username, newUser.pin, "Admin");
		}
		else
		{
			fprintf(users, "%-25s %-25s %-25s %-15x %-7s\n", newUser.name, newUser.surname, newUser.username, newUser.pin, "Analyst");
		}
		fclose(users);
	}
	else
	{
		printf("Error while opening file.");
	}
}