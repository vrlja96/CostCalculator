#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _currency
{
	char currency[4];
	double currencyRate;
}Currency;	//All currency rates are given for BAM (KM)

Currency currentCurrency;	//Currency when the app starts

Currency *readAllCurrencies(int *); //Reading all currencies from currency.txt
void addNewCurrency();	//Adding unexisting currency, or updating existing
void updateCurrencyRate(Currency *); // Update existing currency rate
void printCurrenciesInFile(Currency *, int); //Print Currencies in file
void setDefaultCurrency(); //Set default currency to BAM
int changeCurrency(); //Changing current currency. Just input currency name.
int removeCurrency(); //Admin can remove currency from list
void printAllCurrencies(); //Print currencies on stdout