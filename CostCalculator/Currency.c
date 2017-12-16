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

}