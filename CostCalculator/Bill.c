#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Bill.h"

int isDateLegitimate(char *date)
{
	int d, m, y;
	int daysinmonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int legit = 0;
	char day[3] = { date[0], date[1], 0 }, month[3] = { date[3], date[4], 0 }, year[5] = { date[6], date[7], date[8], date[9], 0 };
	d = atoi(day);
	m = atoi(month);
	y = atoi(year);
	// leap year checking, if ok add 29 days to february
	if (y % 400 == 0 || (y % 100 != 0 && y % 4 == 0))
		daysinmonth[1] = 29;
	// days in month checking
	if (m > 0 && m < 13)
	{
		if (d > 0 && d <= daysinmonth[m - 1])
			legit = 1;
	}

	return legit;
}

char ** readAllBills(int *numberOfFilesInDirectory)
{
	DIR *directory;
	struct dirent *file;
	int maxCapacity = 2;
	char **files = (char **)calloc(maxCapacity, sizeof(char *));
	int numberOfFiles = 0;
	directory = opendir("Bills");
	if (!directory)
	{
		*numberOfFilesInDirectory = numberOfFiles;
		printf("Can not open Bills directory.");
		return NULL;
	}
	while (file = readdir(directory))
	{
		if (strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
		{
			files[numberOfFiles] = (char*)calloc(file->d_namlen + 1, sizeof(char));
			strcpy(files[numberOfFiles++], file->d_name);
			if (numberOfFiles == maxCapacity)
				files = (char **)realloc(files, sizeof(char *) * (maxCapacity *= 2));
		}
	}
	files = (char **)realloc(files, sizeof(char *) * numberOfFiles);
	*numberOfFilesInDirectory = numberOfFiles;
	return files;
}

Format checkBillFormat(FILE *bill, char *filename)
{
	if (strstr(filename, ".csv"))
		return Format5;
	char checkFourthRow[1000] = { 0 }, ignore[1000];
	//Escape first three rows

	fgets(ignore, sizeof(ignore), bill);
	fgets(ignore, sizeof(ignore), bill);
	fgets(ignore, sizeof(ignore), bill);
	//Check fourth row
	fgets(checkFourthRow, sizeof(checkFourthRow), bill);
	if (strstr(checkFourthRow, "Maloprodajni racun"))
		return Format2;
	if (strstr(checkFourthRow, "Datum"))
		return Format3;
	if (strstr(checkFourthRow, "OSI Market"))
		return Format4;
	if (strstr(checkFourthRow, "Racun"))
		return Format1;
	return 0;
}

void removeSubString(char *string1, char *string2)  // removing string s2 from string s1
{
	int i = 0, j, k;
	while (string1[i])
	{
		for (j = 0; string2[j] && string2[j] == string1[i + j]; ++j);
		if (!string2[j])
		{
			for (k = i; string1[k + j]; ++k)
				string1[k] = string1[k + j];
			string1[k] = 0;
		}
		else
			++i;
	}
}

Bill readFormat_1_Bill(FILE *file)
{
	int numberOfProducts = 0, temp = 2;
	Bill bill;
	bill.products = (Product *)calloc(temp, sizeof(Product));
	char *info = (char *)calloc(1024, sizeof(char));

	//Read Buyer Name
	fgets(info, 1024, file);
	removeSubString(info, "Kupac: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.buyerName, info);
	free(info);
	info = (char *)calloc(1024, sizeof(char));

	//Read Date
	fgets(info, 1024, file);
	removeSubString(info, "Datum: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.date, info);

	//Escape Rows that have no useful information
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Products
	while (1)
	{
		if (numberOfProducts == temp)
			bill.products = (Product *)realloc(bill.products, sizeof(Product) * (temp *= 2));

		info = (char *)calloc(1024, sizeof(char));
		fgets(info, 1024, file);
		if (strstr(info, "--------"))
			break;
		Product product = readProduct(info);
		bill.products[numberOfProducts] = product;
		++numberOfProducts;
		free(info);
	}
	bill.numblerOfProducts = numberOfProducts;
	bill.products = (Product *)realloc(bill.products, sizeof(Product) * numberOfProducts);
	info = (char *)calloc(1024, sizeof(char));

	//Read Price
	fgets(info, 1024, file);
	removeSubString(info, "Ukupno: ");
	info[strlen(info) - 1] = 0;
	bill.total = atof(info);
	free(info);

	//Read PDV
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "PDV: ");
	info[strlen(info) - 1] = 0;
	bill.PDV = atof(info);
	free(info);

	//Read Total Price
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Ukupno za placanje: ");
	info[strlen(info) - 1] = 0;
	bill.totalPrice = atof(info);
	free(info);
	return bill;
}

Bill readFormat_2_Bill(FILE *file)
{
	int numberOfProducts = 0, temp = 2;
	Bill bill;
	bill.products = (Product *)calloc(temp, sizeof(Product));
	char *info = (char *)calloc(1024, sizeof(char));

	//Escape rows that have no useful information
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Buyers Name
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Kupac: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.buyerName, info);

	//Esc
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Products
	while (1)
	{
		if (numberOfProducts == temp)
			bill.products = (Product *)realloc(bill.products, sizeof(Product) * (temp *= 2));

		info = (char *)calloc(1024, sizeof(char));
		fgets(info, 1024, file);
		if (strstr(info, "--------"))
			break;
		Product product = readProduct(info);
		bill.products[numberOfProducts] = product;
		++numberOfProducts;
		free(info);
	}
	bill.numblerOfProducts = numberOfProducts;
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	free(info);

	//Read Price
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Ukupno: ");
	info[strlen(info) - 1] = 0;
	bill.total = atof(info);
	free(info);

	//Read PDV
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "PDV: ");
	info[strlen(info) - 1] = 0;
	bill.PDV = atof(info);
	free(info);

	//Read Total price
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Ukupno za placanje: ");
	info[strlen(info) - 1] = 0;
	bill.totalPrice = atof(info);
	free(info);

	//Read Date
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Datum: ");
	info[strlen(info)] = 0;
	strcpy(bill.date, info);
	free(info);

	return bill;
}

Bill readFormat_3_Bill(FILE *file)
{
	int numberOfProducts = 0, temp = 2;
	Bill bill;
	bill.products = (Product *)calloc(temp, sizeof(Product));
	char *info = (char *)calloc(1024, sizeof(char));

	//Escape
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Buyer Name
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Kupac: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.buyerName, info);
	free(info);

	//Read Date
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	removeSubString(info, "Datum: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.date, info);

	//Escape
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Products
	while (1)
	{
		if (numberOfProducts == temp)
			bill.products = (Product *)realloc(bill.products, sizeof(Product) * (temp *= 2));

		info = (char *)calloc(1024, sizeof(char));
		fgets(info, 1024, file);
		if (strstr(info, "--------"))
			break;
		Product product = readProduct(info);
		bill.products[numberOfProducts] = product;
		++numberOfProducts;
		free(info);
	}
	bill.numblerOfProducts = numberOfProducts;
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	info[strlen(info) - 1] = 0;
	char *pdv = strstr(info, "PDV");
	char tmp[25] = { 0 };
	strcpy(tmp, "\t\t");
	strcat(tmp, pdv);
	removeSubString(info, tmp);
	removeSubString(pdv, "PDV: ");
	bill.PDV = atof(pdv);

	removeSubString(info, "Ukupno: ");
	bill.total = atof(info);

	//Ecs
	fgets(info, 1024, file);

	//Read Total Price
	fgets(info, 1024, file);
	removeSubString(info, "Ukupno za placanje: ");
	info[strlen(info)] = 0;
	bill.totalPrice = atof(info);
	return bill;
}

Bill readFormat_4_Bill(FILE *file)
{
	int numberOfProducts = 0, temp = 2;
	Bill bill;
	bill.products = (Product *)calloc(temp, sizeof(Product));
	char *info = (char *)calloc(1024, sizeof(char));

	//Read Buyer Name
	fgets(info, 1024, file);
	removeSubString(info, "Kupac: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.buyerName, info);
	free(info);
	info = (char *)calloc(1024, sizeof(char));

	//Read Date
	fgets(info, 1024, file);
	removeSubString(info, "Datum: ");
	info[strlen(info) - 1] = 0;
	strcpy(bill.date, info);

	//Escape Rows that have no useful information
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	fgets(info, 1024, file);
	free(info);

	//Read Products
	while (1)
	{
		if (numberOfProducts == temp)
			bill.products = (Product *)realloc(bill.products, sizeof(Product) * (temp *= 2));

		info = (char *)calloc(1024, sizeof(char));
		fgets(info, 1024, file);
		if (strstr(info, "--------"))
			break;
		Product product = readProduct(info);
		bill.products[numberOfProducts] = product;
		++numberOfProducts;
		free(info);
	}
	bill.numblerOfProducts = numberOfProducts;

	//Read Total
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	info[strlen(info) - 1] = 0;
	removeSubString(info, "Ukupno: ");
	bill.total = atof(info);
	free(info);

	//Read PDV
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	info[strlen(info) - 1] = 0;
	removeSubString(info, "PDV: ");
	bill.PDV = atof(info);

	//Esc
	fgets(info, 1024, file);
	free(info);

	//Read Total Price
	info = (char *)calloc(1024, sizeof(char));
	fgets(info, 1024, file);
	info[strlen(info) - 1] = 0;
	removeSubString(info, "Ukupno za placanje: ");
	bill.totalPrice = atof(info);
	return bill;
}

Bill readFormat_5_Bill(FILE *file, char *path)
{
	Bill bill;
	char filename[256] = { 0 }, buyerName[128] = { 0 }, *date;
	int temp = 2, numberOfProducts = 0;
	bill.total = 0;
	bill.products = (Product *)calloc(temp, sizeof(Product));
	char *info = (char *)calloc(1024, sizeof(char));
	strcpy(filename, path);
	removeSubString(filename, "Bills\\\\");
	removeSubString(filename, ".csv");
	for (int i = 0; filename[i] != '#'; ++i)
		buyerName[i] = filename[i];
	removeSubString(buyerName, "Kupac");
	strcpy(bill.buyerName, buyerName);
	date = strstr(filename, "#");
	int i = 0;
	for (; date[i]; ++i)
		date[i] = date[i + 1];
	date[i - 1] = 0;

	strcpy(bill.date, date);

	fgets(info, 1024, file);
	free(info);

	//Read Products
	while (1)
	{
		if (numberOfProducts == temp)
			bill.products = (Product *)realloc(bill.products, sizeof(Product) * (temp *= 2));

		info = (char *)calloc(1024, sizeof(char));
		if (!fgets(info, 1024, file))
			break;
		Product product = readProductFormat_5(info);
		bill.total += product.totalProductPrice;
		bill.products[numberOfProducts] = product;
		++numberOfProducts;
		free(info);
	}
	bill.numblerOfProducts = numberOfProducts;
	bill.PDV = (bill.total * 17) / 100;
	bill.totalPrice = bill.total + bill.PDV;
	return bill;
}

Product readProduct(char *productLine)
{
	Product product;
	char numbers[10] = { 0 };
	int i = 0, k;

	for (; productLine[i] != 32 && i < strlen(productLine); ++i)
		product.productName[i] = productLine[i];
	product.productName[i] = 0;

	++i;

	for (k = 0; productLine[i] != 32 && productLine[i] != '=' && i < strlen(productLine); ++i, ++k)
		product.productCode[k] = productLine[i];
	product.productCode[k] = 0;

	while (productLine[i] < 0x30 || productLine[i] > 0x39)
		i++;

	k = 0;

	while (productLine[i] >= 0x30 && productLine[i] <= 0x39)
		numbers[k++] = productLine[i++];

	product.quantity = atof(numbers);
	//Quantity done!

	while (productLine[i] < 0x30 || productLine[i] > 0x39)
		i++;
	while (k)
		numbers[k--] = 0;
	numbers[0] = 0;

	while (productLine[i] >= 0x30 && productLine[i] <= 0x39)
		numbers[k++] = productLine[i++];
	product.singleProductPrice = atof(numbers);
	//Single Product Price done!

	while ((productLine[i] < 0x30 || productLine[i] > 0x39) && productLine[i])
		i++;
	while (k)
		numbers[k--] = 0;
	numbers[0] = 0;

	while (productLine[i] >= 0x30 && productLine[i] <= 0x39)
		numbers[k++] = productLine[i++];
	product.totalProductPrice = atof(numbers);
	return product;
}

Product readProductFormat_5(char *productLine)
{
	int i = 0;
	Product product;
	char numbers[25] = { 0 };
	while ((productLine[i] >= 0x41 && productLine[i] <= 0x5A) || (productLine[i] >= 0x61 && productLine[i] <= 0x7A))
	{
		product.productName[i] = productLine[i];
		++i;
	}
	product.productName[i] = 0;

	int k = 0;
	while (productLine[i] != ',')
	{
		product.productCode[k] = productLine[i];
		++k; ++i;
	}
	product.productCode[k] = 0;
	k = 0;
	++i;
	//Read Quantity
	while (productLine[i] != ',')
	{
		numbers[k] = productLine[i];
		++k; ++i;
	}
	numbers[k] = 0;
	product.quantity = atof(numbers);

	while (k)
		numbers[k--] = 0;
	numbers[0] = 0;
	//Read Single Product Price
	++i;
	while (productLine[i] != ',')
	{
		numbers[k] = productLine[i];
		++k; ++i;
	}
	numbers[k] = 0;
	product.singleProductPrice = atof(numbers);

	while (k)
		numbers[k--] = 0;
	numbers[0] = 0;
	//Read Total Product Price
	++i;
	while (productLine[i])
	{
		numbers[k] = productLine[i];
		++k; ++i;
	}
	numbers[k] = 0;
	product.totalProductPrice = atof(numbers);
	return product;
}

int validateBill(Bill bill, char *path)
{
	char filename[256] = { 0 };
	strcpy(filename, path);
	char errorFilename[256] = { 0 };
	removeSubString(filename, "Bills\\\\");
	strcpy(errorFilename, "ErrorLog\\\\");
	strcat(errorFilename, filename);
	removeSubString(errorFilename, ".txt");
	removeSubString(errorFilename, ".csv");
	strcat(errorFilename, "_error.txt");
	if (!isDateLegitimate(bill.date)) //Check Date
	{
		FILE *errorFile = fopen(errorFilename, "w");
		if (errorFile)
		{
			fprintf(errorFile, "Date is not legitimate");
			fclose(errorFile);
			return 0;
		}
	}
	if (bill.totalPrice != bill.PDV + bill.total) //Check Bill Price
	{
		FILE *errorFile = fopen(errorFilename, "w");
		if (errorFile)
		{
			fprintf(errorFile, "The prices are not correct");
			fclose(errorFile);
			return 0;
		}
	}
	for (int i = 0; i < bill.numblerOfProducts; ++i) //Check each product
	{
		if (bill.products[i].totalProductPrice != bill.products[i].quantity * bill.products[i].singleProductPrice)
		{
			FILE *errorFile = fopen(errorFilename, "w");
			if (errorFile)
			{
				fprintf(errorFile, "Price of %s is not correct", bill.products[i].productName);
				fclose(errorFile);
				return 0;
			}
		}
		return 1;
	}
}

Bill* loadBills(int *numberOfValidBills)
{
	int numberOfBills = 0, j = 0; //j - iterator
	char **files = readAllBills(&numberOfBills);
	Bill *bills = (Bill *)calloc(numberOfBills, sizeof(Bill));
	for (int i = 0; i < numberOfBills; ++i)
	{
		char path[256] = { 0 };
		strcpy(path, "Bills\\\\");
		strcat(path, files[i]);
		FILE *file = fopen(path, "r");
		if (file)
		{
			Format format = checkBillFormat(file, path);
			fseek(file, 0, SEEK_SET);
			if (format == Format1)
			{
				bills[j] = readFormat_1_Bill(file);
				++j;
			}
			else if (format == Format2)
			{
				bills[j] = readFormat_2_Bill(file);
				++j;
			}
			else if (format == Format3)
			{
				bills[j] = readFormat_3_Bill(file);
				++j;
			}
			else if (format == Format4)
			{
				bills[j] = readFormat_4_Bill(file);
				++j;
			}
			else if (format == Format5)
			{
				bills[j] = readFormat_5_Bill(file, path);
				++j;
			}
			else
			{
				char errorFilePath[256] = { 0 };
				strcpy(errorFilePath, "ErrorLog\\\\");
				strcat(errorFilePath, path);
				removeSubString(errorFilePath, "Bills\\\\");
				removeSubString(errorFilePath, ".txt");
				removeSubString(errorFilePath, ".csv");
				strcat(errorFilePath, "_error.txt");
				FILE *errorFile = fopen(errorFilePath, "w");
				if (errorFile)
				{
					fprintf(errorFile, "Unkonwn format");
					fclose(errorFile);
				}
			}
		}
		fclose(file);
	}
	*numberOfValidBills = j;
	return bills;
}