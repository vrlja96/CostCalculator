#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include "User.h"
#include "Currency.h"
#include "Bill.h"

User *currentUser;

int main()
{
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
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		fprintf(adminlog, "%s Login %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		while (1)
		{
			printf("Choose one of options:\n\n");
			printf("[1] Add New User\n[2] Remosve User\n[3] Change Users Group\n[4] Print All Users\n[5] Add New Currency\n[6] Remove Currency\n[7] Change Currency\n[8] Update Currency's rate\n[9] Print All Currencies\n[10] Logout\n[0] Close Application\n");
			scanf("%d", &option);
			switch (option)
			{
			case 1:
				addNewUser();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Add New User %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 2:
				removeUser();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Remove User %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 3:
				changeUserGroup();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Change User Group %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 4:
				system("cls");
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Print All Users %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				printAllUsers();
				break;
			case 5:
				addNewCurrency();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Add New Currency %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 6:
				removeCurrency();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Remove Currency %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 7:
				changeCurrency();
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Change Currency %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				printf("\n%s %lf\n", currentCurrency.currency, currentCurrency.currencyRate);
				break;
			case 8:
				updateCurrencyRate(NULL);
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Update Currency Rate %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				system("cls");
				break;
			case 9:
				system("cls");
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Print All Currencies %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				printAllCurrencies();
				break;
			case 10:
				system("cls");
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				fprintf(adminlog, "%s Logout %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
				fclose(adminlog);
				main();
				break;
			default:
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
		printf("Analyst part of application is under constrquction.");
		FILE *analystlog = fopen("analystlog.txt", "a+");
		if (!analystlog)
		{
			printf("Unexpected Error");
			exit(-1);
		}
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		fprintf(analystlog, "%s Login %02d.%02d.%02d. - %02d:%02d:%02d\n", currentUser->username, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		fclose(analystlog);
		main();
	}
}
