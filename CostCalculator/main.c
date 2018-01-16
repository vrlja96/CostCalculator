#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include "User.h"
#include "Currency.h"
#include "Bill.h"


void makelog(char *, User *, FILE *);
//int main()
//{
//	Currency currentCurrency = { "CHF", 0.603};
//	Node *head = NULL, *tail = NULL;
//	loadBills(&head, &tail);
//	exportDataForBuyer(head, tail, currentCurrency);
//	exportDataForMonth(head, tail, currentCurrency);
//	exportDataForProduct(head, tail, currentCurrency);
//}

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
		printf("\nLogin time: %02d.%02d.%02d. - %02d:%02d:%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
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
			printf("[1] Add New User\n[2] Remosve User\n[3] Change Users Group\n[4] Print All Users\n[5] Add New Currency\n[6] Remove Currency\n[7] Change Currency\n[8] Update Currency's rate\n[9] Print All Currencies\n[10] Logout\n[0] Close Application\n");
			scanf("%d", &option);
			switch (option)
			{
			case 1:
				addNewUser();
				makelog("Add New User", currentUser, adminlog);
				system("cls");
				break;
			case 2:
				removeUser();
				makelog("Remove User", currentUser, adminlog);
				system("cls");
				break;
			case 3:
				changeUserGroup();
				makelog("Change Users Group", currentUser, adminlog);
				system("cls");
				break;
			case 4:
				system("cls");
				makelog("Print All Users", currentUser, adminlog);
				printAllUsers();
				break;
			case 5:
				addNewCurrency();
				makelog("Add New Currency", currentUser, adminlog);
				system("cls");
				break;
			case 6:
				removeCurrency();
				makelog("Remove Currency", currentUser, adminlog);
				system("cls");
				break;
			case 7:
				changeCurrency();
				makelog("Change Currency", currentUser, adminlog);
				system("cls");
				printf("\n%s %lf\n", currentCurrency.currency, currentCurrency.currencyRate);
				break;
			case 8:
				updateCurrencyRate(NULL);
				makelog("Update Currency Rate", currentUser, adminlog);
				system("cls");
				break;
			case 9:
				system("cls");
				makelog("Print All Currencies", currentUser, adminlog);
				printAllCurrencies();
				break;
			case 10:
				system("cls");
				makelog("Logout", currentUser, adminlog);
				fclose(adminlog);
				main();
				break;
			default: case 0:
				fclose(adminlog);
				return 0;
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
				exportDataForProduct(head, tail, currentCurrency);
				makelog("Export Data For Product", currentUser, analystlog);
				system("cls");
				break;
			case 2:
				exportDataForMonth(head, tail, currentCurrency);
				makelog("Export Data For Month", currentUser, analystlog);
				system("cls");
				break;
			case 3:
				exportDataForBuyer(head, tail, currentCurrency);
				makelog("Export Data For Buyer", currentUser, analystlog);
				system("cls");
				break;
			case 4:
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
				return 0;
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
