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

