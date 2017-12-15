#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _currency
{
	char currency[4];
	double currencyRate;
}Currency;	//All currency rates are given for BAM (KM)

Currency *readAllCurrencies(int *); //Reading all currencies from currency.txt
void addNewCurrency();	//Adding unexisting currency, or updating existing
void updateCurrencyRate(Currency *); // Update existing currency rate