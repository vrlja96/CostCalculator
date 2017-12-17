#define _CRT_SECURE_NO_WARNINGS
#include "Currency.h"

Currency * readAllCurrencies(int * numOfCurrencies)
{
	FILE *currencyFile = fopen("currency.txt", "r");
	if (!currencyFile)
	{
		printf("Can not open \"currency.txt\" .");
		return NULL;
	}
	int numberOfCurrencies = 0, temp = 1;
	Currency *currencies = (Currency *)calloc(temp, sizeof(Currency));
	while (fscanf(currencyFile, "%s %lf\n", currencies[numberOfCurrencies].currency, &currencies[numberOfCurrencies].currencyRate) != EOF)
	{
		if (++numberOfCurrencies == temp)
		{
			currencies = (Currency *)realloc(currencies, sizeof(Currency) * (temp *= 2));

		}
	}
	if (numberOfCurrencies == 0)
	{
		*numOfCurrencies = 0;
		return NULL;
	}
	currencies = (Currency *)realloc(currencies, sizeof(Currency) * numberOfCurrencies);
	*numOfCurrencies = numberOfCurrencies;
	fclose(currencyFile);
	return currencies;
}

void addNewCurrency()
{
	int numberOfCurrencies;
	Currency newCurrency, *currencies = readAllCurrencies(&numberOfCurrencies);
	FILE *currencyFile = fopen("currency.txt", "a+");
	if (!currencyFile)
	{
		printf("Error. Can not open \"currency.txt\" ");
		return;
	}
	printf("Adding new Currency.\n Name (3 characters): ");
	scanf("%s", newCurrency.currency);
	for (int i = 0; i < numberOfCurrencies; ++i)
	{
		if (!strcmp(currencies[i].currency, newCurrency.currency))
		{
			printf("Currency Rate: ");
			scanf("%lf", &newCurrency.currencyRate);
			if (newCurrency.currencyRate == currencies[i].currencyRate)
			{
				printf("Error! Currency with same rate already exist.");
				fclose(currencyFile);
				return;
			}
			printf("Currency already exists. Updating currency rate.");
			fclose(currencyFile);
			updateCurrencyRate(&newCurrency);
			return;
		}
	}
	printf("Currency Rate: ");
	scanf("%lf", &newCurrency.currencyRate);
	fprintf(currencyFile, "%-4s %-5.2lf\n", newCurrency.currency, newCurrency.currencyRate);
	fclose(currencyFile);
}


void updateCurrencyRate(Currency *currency)
{
	int numberOfCurrencies;
	Currency *currencies = readAllCurrencies(&numberOfCurrencies), toUpdateCurrency;
	if (currency)
	{
		for (int i = 0; i < numberOfCurrencies; ++i)
		{
			if (!strcmp(currencies[i].currency, currency->currency))
			{
				currencies[i].currencyRate = currency->currencyRate;
				break;
			}
		}
		printCurrenciesInFile(currencies, numberOfCurrencies);
		return;
	}
	printf("Enter Currency you want to update: ");
	scanf("%s", toUpdateCurrency.currency);
	for (int i = 0; i < numberOfCurrencies; ++i)
	{
		if (!strcmp(toUpdateCurrency.currency, currencies[i].currency))
		{
			printf("Enter new Rate for %s", currencies[i].currency);
			scanf("%lf", &toUpdateCurrency.currencyRate);
			if (toUpdateCurrency.currencyRate == currencies[i].currencyRate)
			{
				printf("Error! Same Rate! Exiting");
				return;
			}
			currencies[i].currencyRate = toUpdateCurrency.currencyRate;
			printf("Updating successful");
			printCurrenciesInFile(currencies, numberOfCurrencies);
			return;
		}
	}
	char answer;
	getchar();
	printf("%s does not exist. Do you want to add it[Y/N]", toUpdateCurrency.currency);
	scanf("%c", &answer);
	if (answer == 'Y' || answer == 'y')
	{
		addNewCurrency();
		return;
	}
	return;
}

void printCurrenciesInFile(Currency *currencies, int numberOfCurrencies)
{
	FILE *file = fopen("currency.txt", "w");
	if (!file)
	{
		printf("Error! Can not open currency file");
		return;
	}
	for (int i = 0; i < numberOfCurrencies; ++i)
	{
		fprintf(file, "%-4s %-5.2lf\n", currencies[i].currency, currencies[i].currencyRate);
	}
	fclose(file);
}


int changeCurrency()
{
	int numberOfCurrencies;
	char currency[10] = { 0 };
	Currency *currencies = readAllCurrencies(&numberOfCurrencies);
	printAllCurrencies();
	printf("Input Currency Name (3 characters): ");
	scanf("%s", &currency);
	for (int i = 0; i < numberOfCurrencies; ++i)
	{
		if (!strcmp(currency, currencies[i].currency))
		{
			strcpy(currentCurrency.currency, currency);
			currentCurrency.currencyRate = currencies[i].currencyRate;
			return 1;
		}
	}
	printf("No such Currency in currency list.");
	return 0;
}

void setDefaultCurrency()
{
	strcpy(currentCurrency.currency, "BAM");
	currentCurrency.currencyRate = 1.0;
}