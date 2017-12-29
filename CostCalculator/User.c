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

char* inputPin()
{
	char c, *pin = (char *)calloc(15, sizeof(char));
	int pos = 0;
	do {
		c = _getch();

		if (isprint(c) && c >= 0x30 && c <= 0x39)
		{
			pin[pos++] = c;
			printf("%c", '*');
		}
		else if (c == 8 && pos)
		{
			pin[pos--] = '\0';
			printf("%s", "\b \b");
		}
	} while (c != 13);
	return pin;
}

int removeUser(User *loggedUser)
{
	int numberOfRegisteredUsers = 0;
	User *users = readAllRegisteredUsers(&numberOfRegisteredUsers), toDeleteUser;
	printAllUsers();
	printf("Deleting user account\n");
	printf("Name: ");
	scanf("%s", toDeleteUser.name);
	printf("Surname: ");
	scanf("%s", toDeleteUser.surname);
	printf("Username: ");
	scanf("%s", toDeleteUser.username);
	if (!strcmp(loggedUser->username, toDeleteUser.username))
	{
		printf("You can not remove yourself!");
		Sleep(3000);
		return 0;
	}
	int i = isUsernameExisting(toDeleteUser.username);
	if (i >= 0 && !strcmp(users[i].name, toDeleteUser.name) && !strcmp(users[i].surname, toDeleteUser.surname))
	{
		char answer;
		getchar();
		printf("Do you really want do delete user[Y/N]");
		scanf("%c", &answer);
		if (answer == 'Y' || answer == 'y')
		{
			for (int j = i; j < numberOfRegisteredUsers - 1; ++j)
				users[j] = users[j + 1];
			printAllUsersInFile(users, numberOfRegisteredUsers - 1);
			return 1;
		}
		return 0;
	}
	printf("No such user.");
	Sleep(3000);
	return 0;
}

User * readAllRegisteredUsers(int *numberOfAllUsers)
{
	int numberOfRegisteredUsers = 0, temp = 1;
	User *allUsers = (User *)calloc(temp, sizeof(User));
	FILE *users = fopen("users.txt", "r");
	char group[8] = { 0 };
	if (users)
	{
		while (fscanf(users, "%s %s %s %x %s\n", allUsers[numberOfRegisteredUsers].name, allUsers[numberOfRegisteredUsers].surname, allUsers[numberOfRegisteredUsers].username, &allUsers[numberOfRegisteredUsers].pin, group) != EOF)
		{
			if (!strcmp(group, "Admin"))
			{
				allUsers[numberOfRegisteredUsers].userGroup = Admin;
			}
			else
			{
				allUsers[numberOfRegisteredUsers].userGroup = Analyst;
			}
			++numberOfRegisteredUsers;
			if (numberOfRegisteredUsers == temp)
			{
				allUsers = (User *)realloc(allUsers, sizeof(User) * (temp *= 2));
			}

		}
		if (numberOfRegisteredUsers == 0)
		{
			*numberOfAllUsers = 0;
			return NULL;
		}
		allUsers = (User *)realloc(allUsers, sizeof(User) * (1 + numberOfRegisteredUsers));
		(*numberOfAllUsers) = numberOfRegisteredUsers;
		fclose(users);
		return allUsers;
	}
	else
	{
		printf("Error while opening users.txt");
		Sleep(3000);
		exit(-1);
	}
	return NULL;
}

void printAllUsersInFile(User *users, int numberOfUsers)
{
	FILE *file = fopen("users.txt", "w");
	if (!file)
	{
		printf("Error while opening users.txt");
		return;
	}
	for (int i = 0; i < numberOfUsers; ++i)
	{
		if (users[i].userGroup == Admin)
		{
			fprintf(file, "%-25s %-25s %-25s %-15x %-5s\n", users[i].name, users[i].surname, users[i].username, users[i].pin, "Admin");
		}
		else
		{
			fprintf(file, "%-25s %-25s %-25s %-15x %-7s\n", users[i].name, users[i].surname, users[i].username, users[i].pin, "Analyst");
		}
	}
	fclose(file);
}

int isUsernameExisting(char *username)
{
	int numberOfAllUsers;
	User *allUsers = readAllRegisteredUsers(&numberOfAllUsers);
	for (int i = 0; i < numberOfAllUsers; ++i)
	{
		if (!strcmp(allUsers[i].username, username))
		{
			return i;
		}
	}
	return -1;
}

void printAllUsers()
{
	int numberOfUsers = 0;
	User *allUsers = readAllRegisteredUsers(&numberOfUsers);
	if (!allUsers)
	{
		return;
	}
	printf("All Users:\n");
	printf("%-25s %-25s %-25s %-20s\n=========================================================================================\n", "Name", "Surname", "Username", "User Group");
	for (int i = 0; i < numberOfUsers; ++i)
	{
		if (allUsers[i].userGroup == Admin)
		{
			printf("%-25s %-25s %-25s %-7s\n", allUsers[i].name, allUsers[i].surname, allUsers[i].username, "Admin");
		}
		else
		{
			printf("%-25s %-25s %-25s %-7s\n", allUsers[i].name, allUsers[i].surname, allUsers[i].username, "Analyst");
		}
	}
	printf("=========================================================================================\n");
}
User* changeUserGroup()
{
	int numberOfUsers;
	User *users = readAllRegisteredUsers(&numberOfUsers);
	char name[25] = { 0 }, surname[25] = { 0 }, group[15] = { 0 }, username[25] = { 0 };
	printf("Enter Users Name: ");
	scanf("%s", name);
	printf("Enter Users Surname: ");
	scanf("%s", surname);
	printf("Enter Users Username: ");
	scanf("%s", username);
	int i = isUsernameExisting(username);
	if (i >= 0 && !strcmp(users[i].name, name) && !strcmp(users[i].surname, surname))
	{
		printf("Enter new group for %s %s [Admin/Analyst]: ", name, surname);
		scanf("%s", group);
		if (!strcmp(group, "Admin") || !strcmp(group, "ADMIN") || !strcmp(group, "admin"))
		{
			if (users[i].userGroup == Admin)
			{
				printf("\nUser is already admin.");
				Sleep(3000);
				return NULL;
			}
			else
			{
				users[i].userGroup = Admin;
				printf("\nUserGroup changed successfully");
				Sleep(3000);
				printAllUsersInFile(users, numberOfUsers);
				return &users[i];
			}
		}
		else if (!strcmp(group, "Analyst") || !strcmp(group, "ANALYST") || !strcmp(group, "analyst"))
		{
			if (users[i].userGroup == Analyst)
			{
				printf("\nUser is already Analyst.");
				Sleep(3000);
				return NULL;
			}
			else
			{
				users[i].userGroup = Analyst;
				printf("\nUserGroup changed successfully");
				printAllUsersInFile(users, numberOfUsers);
				Sleep(3000);
				return &users[i];
			}
		}
		else
		{
			printf("Wrong UserGroup");
			Sleep(3000);
			return NULL;
		}
	}
	return 0;
}

User* login()
{
	int numberOfRegisteredUsers = 0;
	unsigned pin;
	char username[25];
	User *allUsers = readAllRegisteredUsers(&numberOfRegisteredUsers);
	printf("Login:\nUsername: ");
	scanf("%s", username);
	printf("PIN: ");
	char *pins = inputPin();
	pin = xcrc32(pins, 4, 16);
	int position = isUsernameExisting(username);
	if ((position >= 0) && allUsers[position].pin == pin)
	{
		printf("Login Successful.");
		Sleep(2000);
		return &allUsers[position];
	}
	else
	{
		printf("Wrong username or PIN!");
		Sleep(3000);
		return NULL;
	}
}