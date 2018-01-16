#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include "User.h"
#include "Currency.h"
#include "Bill.h"


void makelog(char *, User *, FILE *);

Currency currentCurrency = { "BAM", 1.00 };

int main()
{
	User *currentUser = NULL;
	int option;
	time_t rawtime;
	struct tm *timeinfo;
	currentUser = login();
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	if (currentUser == NULL)
	{
		printf("\nTry again!\n");
		main();
	}
	if (currentUser->userGroup == Admin)
	{
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		system("cls");
		printf("Welcome %s", currentUser->name);
		printf("\nLogin time: %02d.%02d.%02d. - %02d:%02d:%02d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		FILE *adminlog = fopen("adminlog.txt", "a+");
		if (!adminlog)
		{
			printf("Unexpected Error");
			exit(-1);
		}
		makelog("Login", currentUser, adminlog);
		while (1)
		{
			printf("Choose one of options:\n\n");
			printf("[1] Add New User\n[2] Remove User\n[3] Change Users Group\n[4] Print All Users\n[5] Add New Currency\n[6] Remove Currency\n[7] Change Currency\n[8] Update Currency's rate\n[9] Print All Currencies\n[10] Logout\n[0] Close Application\n");
			scanf("%d", &option);
			User *changedUser = NULL;
			switch (option)
			{
			case 1:
				system("cls");
				addNewUser();
				makelog("Add New User", currentUser, adminlog);
				system("cls");
				break;
			case 2:
				system("cls");
				removeUser(currentUser);
				makelog("Remove User", currentUser, adminlog);
				system("cls");
				break;
			case 3:
				system("cls");
				printAllUsers();
				changedUser = changeUserGroup();
				if (changedUser && !strcmp(changedUser->username, currentUser->username))
				{
					printf("\nYour Usergroup has been changed. Logging out.\n");
					Sleep(2000);
					makelog("Logout", currentUser, adminlog);
					currentUser = NULL;
					fclose(adminlog);
					system("cls");
					main();
				}
				makelog("Change Users Group", currentUser, adminlog);
				system("cls");
				break;
			case 4:
				system("cls");
				makelog("Print All Users", currentUser, adminlog);
				printAllUsers();
				printf("\nPress ENTER to continue");
				getchar();
				getchar();
				system("cls");
				break;
			case 5:
				system("cls");
				addNewCurrency();
				makelog("Add New Currency", currentUser, adminlog);
				system("cls");
				break;
			case 6:
				system("cls");
				removeCurrency();
				makelog("Remove Currency", currentUser, adminlog);
				system("cls");
				break;
			case 7:
				system("cls");
				changeCurrency();
				makelog("Change Currency", currentUser, adminlog);
				system("cls");
				printf("\n%s %lf\n", currentCurrency.currency, currentCurrency.currencyRate);
				break;
			case 8:
				system("cls");
				printAllCurrencies();
				updateCurrencyRate(NULL);
				makelog("Update Currency Rate", currentUser, adminlog);
				system("cls");
				break;
			case 9:
				makelog("Print All Currencies", currentUser, adminlog);
				printAllCurrencies();
				printf("\nPress ENTER to continue");
				getchar();
				getchar();
				system("cls");
				break;
			case 10:
				system("cls");
				system("cls");
				makelog("Logout", currentUser, adminlog);
				fclose(adminlog);
				main();
				break;
			default: case 0:
				fclose(adminlog);
				exit(0);
			}
		}
	}
	if (currentUser->userGroup == Analyst)
	{
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		system("cls");
		printf("Welcome %s", currentUser->name);
		printf("\nLogin time: %02d.%02d.%02d. - %02d:%02d:%02d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		FILE *analystlog = fopen("analystlog.txt", "a+");
		if (!analystlog)
		{
			printf("Unexpected Error");
			exit(-1);
		}
		makelog("Login", currentUser, analystlog);
		Node *head = NULL, *tail = NULL;
		loadBills(&head, &tail);
		while (1)
		{
			printf("Choose one of options:\n\n");
			printf("[1] Export Data For Product\n[2] Export Data For Month\n[3] Export Data for Buyer\n[4] Change Currency\n[5] Logout\n[0] Close Application\n");
			scanf("%d", &option);
			switch (option)
			{
			case 1:
				system("cls");
				exportDataForProduct(head, tail, currentCurrency);
				makelog("Export Data For Product", currentUser, analystlog);
				system("cls");
				break;
			case 2:
				system("cls");
				exportDataForMonth(head, tail, currentCurrency);
				makelog("Export Data For Month", currentUser, analystlog);
				system("cls");
				break;
			case 3:
				system("cls");
				exportDataForBuyer(head, tail, currentCurrency);
				makelog("Export Data For Buyer", currentUser, analystlog);
				system("cls");
				break;
			case 4:
				system("cls");
				printAllCurrencies();
				changeCurrency();
				makelog("Change Currency", currentUser, analystlog);
				system("cls");
				printf("\n%s %lf\n", currentCurrency.currency, currentCurrency.currencyRate);
				break;
			case 5:
				system("cls");
				makelog("Logout", currentUser, analystlog);
				fclose(analystlog);
				clearList(&head, &tail);
				main();
			default:case 0:
				fclose(analystlog);
				clearList(&head, &tail);
				exit(0);
			}
		}
	}
}

void makelog(char *message, User *user, FILE *file)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fprintf(file, "%s %s %02d.%02d.%02d. - %02d:%02d:%02d\n", user->username, message, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}
