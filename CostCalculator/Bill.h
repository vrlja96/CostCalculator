#pragma once
#include <Windows.h>
#include <string.h>
#include "dirent.h"

#define Format1 1
#define Format2 2
#define Format3 3
#define Format4 4
#define Format5 5

#define Format int


typedef struct _product
{
	char productCode[15], productName[50];
	double quantity, singleProductPrice, totalProductPrice;
}Product;

typedef struct _Bill
{
	Product *products;
	int numblerOfProducts;
	char date[15], buyerName[50];
	double total, PDV, totalPrice;
}Bill;

typedef struct _node
{
	Bill bill;
	struct _node *next, *prev;
}Node;

int isDateLegitimate(char *); //Check if date is legitimate(returns 1 if true, 0 if false)
Format checkBillFormat(FILE *, char *); //Bill Format Check
char **readAllBills(int *); //Return names of all files in directory Bills and number of bills
void removeSubString(char *, char *); //Remove second string from first one

Bill readFormat_1_Bill(FILE *); //Reading Format1 Bill
Bill readFormat_2_Bill(FILE *); //Reading Format2 Bill
Bill readFormat_3_Bill(FILE *); //Reading Format3 Bill
Bill readFormat_4_Bill(FILE *); //Reading Format4 Bill
Bill readFormat_5_Bill(FILE *, char *); //Reading Format5 Bill

Product readProduct(char *); //Reading product from a line in file, whole line is parameter
Product readProductFormat_5(char *); //Reading product from a line in file, if bill is .csv
int validateBill(Bill, char *); //Return 1 if bill is valid or 0 if bill is not valid
Bill *loadBills(int *); //Loads all bills into system
void archiveBill(char *); //Move bill in archive after loading

void add(Node **, Node**, Bill); //Add bill in list
void clearList(Node **, Node **); //Clear list