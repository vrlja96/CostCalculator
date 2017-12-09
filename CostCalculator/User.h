#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include "crc32.h" //Adding Crypto algorithm 

#define Admin 1
#define Analyst 2
#define Group int

typedef struct _user
{
	char name[25], surname[25], username[25];
	unsigned  pin;
	Group userGroup;
}User; //This will represent all users who can log in.

void addNewUser(); //Adding new user in users.txt. Every PIN will be crypted and the result is pasted in .txt file
User* readAllRegisteredUsers(int *); //Reading all registered users from users.txt
char *inputPin(); //Masking PIN with asterisks
int isUsernameExisting(char *);  //Returns position in user.txt file if user with specified username already exists, or -1 if user doesnt exist