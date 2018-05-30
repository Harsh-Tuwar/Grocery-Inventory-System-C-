
//Name: Harsh Tuwar
//Student ID: 154-156-160
//Email ID: htuwar@myseneca.ca
//Final Assembly

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// Define statements:
#define LINEAR 1
#define FORM 0
#define STOCK 1
#define CHECKOUT 0
#define MAX_ITEM_NO 500
#define MAX_QTY 999
#define SKU_MAX 999
#define SKU_MIN 100
#define DATAFILE "items.txt"

// Constant TAX value:
const double TAX = 0.13;

// Item structure:
struct Item {
	double price;
	int sku;
	int isTaxed;
	int quantity;
	int minQuantity;
	char name[21];
};

// Function Prototypes Milestone 1:
// tools
void welcome(void);
void printTitle(void);
void printFooter(double gTotal);
void flushKeyboard(void);
void pause(void);
int getInt(void);
double getDouble(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDoubleLimited(double lowerLimit, double upperLimit);

// app interface
int yes(void);
void GroceryInventorySystem(void);
int menu(void);

// Function Prototypes Milesone 2:
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
struct Item itemEntry(int sku);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int noOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);

// Function Prototypes Milesone 3:
void search(const struct Item item[], int NoOfRecs);
void updateItem(struct Item* itemptr);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);

// Function Prototypes Milesone 4:
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);

// Main Function for the Final Project:
int main(void)
{
	GroceryInventorySystem();
	return 0;
}

// Function Implementations Milestone 1:
void welcome(void)
{
	printf("---=== Grocery Inventory System ===---\n\n");
}

void printTitle(void)
{
	printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn\n");
	printf("----+---+--------------------+--------+-----+-----+-----+------------|---\n");
}

void printFooter(double gTotal)
{
	printf("--------------------------------------------------------+----------------\n");
	if (gTotal > 0) {
		printf("                                           Grand Total: |%12.2lf\n", gTotal);
	}
}

void flushKeyboard(void)
{
	while (getchar() != '\n')
		; // Empty statement intentional
}

void pause(void)
{
	printf("Press <ENTER> to continue...");
	flushKeyboard();
}

int getInt(void)
{
	int Value;
	char NL = 'x';

	// Gets a valid integer from the keyboard.
	// It should not let the user pass unless a valid integer is entered.
	//
	while (NL != '\n') {
		scanf("%d%c", &Value, &NL);
		if (NL != '\n') {
			flushKeyboard(); // Clear the Keyboard
			printf("Invalid integer, please try again: ");
		}
	}

	return Value;
}

int getIntLimited(int lowerLimit, int upperLimit)
{
	int Value;
	Value = getInt(); // Receive a valid integer

					  // Make sure the integer entered is within the limits
					  //
	while (Value < lowerLimit || Value > upperLimit) {
		printf("Invalid value, %d < value < %d: ", lowerLimit, upperLimit);
		Value = getInt();
	}

	return Value;
}

double getDouble(void)
{
	double Value;
	char NL = 'x';

	// Gets a valid double from the keyboard.
	// It should not let the user pass unless a valid double is entered.
	//
	while (NL != '\n') {
		scanf("%lf%c", &Value, &NL);
		if (NL != '\n') {
			flushKeyboard(); // Clear the Keyboard
			printf("Invalid number, please try again: ");
		}
	}

	return Value;
}

double getDoubleLimited(double lowerLimit, double upperLimit)
{
	double Value;
	Value = getDouble(); // Receive a valid integer

						 // Make sure the integer entered is within the limits
						 //
	while (Value < lowerLimit || Value > upperLimit) {
		printf("Invalid value, %lf< value < %lf: ", lowerLimit, upperLimit);
		Value = getDouble();
	}

	return Value;
}

int yes(void)
{
	char var; // the single character from the user
	int output; // the return value of the function

	scanf("%c", &var);
	flushKeyboard(); // clear the keyboard

					 //If the character is anything other than 'Y''y''N''n', read it again.
					 //
	while (var != 'Y' && var != 'y' && var != 'N' && var != 'n') {
		printf("Only (Y)es or (N)o are acceptable: ");
		scanf("%c", &var);
		flushKeyboard(); // clear the keyboard
	}

	if (var == 'Y' || var == 'y')
		output = 1;
	else if (var == 'N' || var == 'n')
		output = 0;

	return output;
}

int menu(void)
{
	printf("1- List all items\n");
	printf("2- Search by SKU\n");
	printf("3- Checkout an item\n");
	printf("4- Stock an item\n");
	printf("5- Add new item or update item\n");
	printf("6- delete item\n");
	printf("7- Search by name\n");
	printf("0- Exit program\n");
	printf("> ");

	return getIntLimited(0, 7);
}

void GroceryInventorySystem(void)
{
	int menuOpt;
	int flag = 0; // the flag for the while loop
	struct Item item[MAX_ITEM_NO];
	int NoOfRec;

	welcome();
	if (loadItems(item, DATAFILE, &NoOfRec)) {

		// Display menus and receive user's selection
		//
		while (flag == 0) {
			menuOpt = menu();

			switch (menuOpt) {
			case 1:
				listItems(item, NoOfRec);
				pause();
				break;
			case 2:
				search(item, NoOfRec);
				pause();
				break;
			case 3:
				adjustQuantity(item, NoOfRec, CHECKOUT);
				if (saveItems(item, DATAFILE, NoOfRec) == 0)
					printf("could not update data file %s\n", DATAFILE);
				pause();
				break;
			case 4:
				adjustQuantity(item, NoOfRec, STOCK);
				if (saveItems(item, DATAFILE, NoOfRec) == 0)
					printf("could not update data file %s\n", DATAFILE);
				pause();
				break;
			case 5:
				addOrUpdateItem(item, &NoOfRec);
				if (saveItems(item, DATAFILE, NoOfRec) == 0)
					printf("could not update data file %s\n", DATAFILE);
				pause();
				break;
			case 6:
				printf("Delete Item under construction!\n");
				pause();
				break;
			case 7:
				printf("Search by name under construction!\n");
				pause();
				break;
			case 0:
				printf("Exit the program? (Y)es/(N)o): ");
				if (yes() == 1)

					flag = 1; // set flag to 1 to end the loop
			}
		}

	}
	else {
		printf("Could not read from %s.\n", DATAFILE);
	}
}

// Function Implementations Milestone 2:
double totalAfterTax(struct Item item)
{
	double totalPrice;

	if (item.isTaxed)
		totalPrice = item.price * item.quantity * (1 + TAX);
	else
		totalPrice = item.price * item.quantity;

	return totalPrice;
}

int isLowQuantity(struct Item item)
{
	int output; // return value

	if (item.quantity <= item.minQuantity)
		output = 1;
	else
		output = 0;

	return output;
}

struct Item itemEntry(int sku)
{
	struct Item item;

	item.sku = sku;
	printf("        SKU: %d\n", item.sku);
	printf("       Name: ");
	scanf("%20[^\n]", item.name);
	flushKeyboard();
	printf("      Price: ");
	item.price = getDoubleLimited(0.01, 1000.00);
	printf("   Quantity: ");
	item.quantity = getIntLimited(1, MAX_QTY);
	printf("Minimum Qty: ");
	item.minQuantity = getIntLimited(0, 100);
	printf("   Is Taxed: ");
	item.isTaxed = yes();

	return item;
}

void displayItem(struct Item item, int linear)
{
	// Display in LINEAR format
	if (linear) {
		printf("|%3d|%-20s|%8.2lf|  ", item.sku, item.name, item.price);

		if (item.isTaxed)
			printf("%3s", "Yes");
		else
			printf("%3s", "No");

		printf("| %3d | %3d |%12.2lf|", item.quantity, item.minQuantity, totalAfterTax(item));

		if (isLowQuantity(item))
			printf("***\n");
		else
			printf("\n");

		// Display in FORM format
	}
	else {
		printf("        SKU: %d\n", item.sku);
		printf("       Name: %s\n", item.name);
		printf("      Price: %.2lf\n", item.price);
		printf("   Quantity: %d\n", item.quantity);
		printf("Minimum Qty: %d\n", item.minQuantity);

		if (item.isTaxed)
			printf("   Is Taxed: %s\n", "Yes");
		else
			printf("   Is Taxed: %s\n", "No");

		if (isLowQuantity(item))
			printf("WARNING: Quantity low, please order ASAP!!!\n");
	}
}

void listItems(const struct Item item[], int noOfItems)
{
	int i;
	double total = 0; // grand total

	printTitle();

	for (i = 0; i < noOfItems; i++) {
		printf("%-4d", i + 1); // Print row number
		displayItem(item[i], LINEAR);
		total = total + totalAfterTax(item[i]);
	}

	printFooter(total);
}
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index)
{
	int i;
	int output; // return value

	for (i = 0; i < NoOfRecs; i++) {
		if (item[i].sku == sku) {
			*index = i;
			output = 1;
			break; // Once found, exit the loop
		}
		else {
			output = 0;
		}
	}

	return output;
}

// Function Implementations Milestone 3:
void search(const struct Item item[], int NoOfRecs)
{
	int skuInput;
	int index; // index of the found item
	int flag;

	printf("Please enter the SKU: ");
	skuInput = getIntLimited(SKU_MIN, SKU_MAX);
	flag = locateItem(item, NoOfRecs, skuInput, &index);

	if (flag) // item found
		displayItem(item[index], FORM);
	else // item not found
		printf("Item not found!\n");
}

void updateItem(struct Item* itemptr)
{
	struct Item item;

	printf("Enter new data:\n");
	item = itemEntry(itemptr->sku);
	printf("Overwrite old data? (Y)es/(N)o: ");

	if (yes()) {
		// overwrite target item
		strcpy(itemptr->name, item.name);
		itemptr->price = item.price;
		itemptr->quantity = item.quantity;
		itemptr->minQuantity = item.minQuantity;
		itemptr->isTaxed = item.isTaxed;
		printf("--== Updated! ==--\n");
	}
	else {
		printf("--== Aborted! ==--\n");
	}
}

void addItem(struct Item item[], int *NoOfRecs, int sku)
{
	struct Item newItem;

	if (*NoOfRecs == MAX_ITEM_NO) {
		printf("Can not add new item; Storage Full!\n");
	}
	else {
		newItem = itemEntry(sku);
		printf("Add Item? (Y)es/(N)o: ");

		if (yes()) {
			// add a new item to the item array
			strcpy(item[*NoOfRecs].name, newItem.name);
			item[*NoOfRecs].sku = newItem.sku;
			item[*NoOfRecs].price = newItem.price;
			item[*NoOfRecs].quantity = newItem.quantity;
			item[*NoOfRecs].minQuantity = newItem.minQuantity;
			item[*NoOfRecs].isTaxed = newItem.isTaxed;
			(*NoOfRecs)++;
			printf("--== Added! ==--\n");
		}
		else {
			printf("--== Aborted! ==--\n");
		}
	}
}

void addOrUpdateItem(struct Item item[], int* NoOfRecs)
{
	int skuInput;
	int index; // index of the found item
	int flag;

	printf("Please enter the SKU: ");
	skuInput = getIntLimited(SKU_MIN, SKU_MAX);
	flag = locateItem(item, *NoOfRecs, skuInput, &index);

	if (flag) {
		displayItem(item[index], FORM);
		printf("Item already exists, Update? (Y)es/(N)o: ");

		if (yes())
			updateItem(&item[index]);
		else
			printf("--== Aborted! ==--\n");

	}
	else {
		addItem(item, NoOfRecs, skuInput);
	}
}

void adjustQuantity(struct Item item[], int NoOfRecs, int stock)
{
	int skuInput;
	int index; // index of the found item
	int flag;
	int qtyInput;

	printf("Please enter the SKU: ");
	skuInput = getIntLimited(SKU_MIN, SKU_MAX);
	flag = locateItem(item, NoOfRecs, skuInput, &index);

	if (flag) {
		displayItem(item[index], FORM);

		// stock item
		if (stock == STOCK) {
			printf("Please enter the quantity to stock; Maximum of %d or 0 to abort: ",
				MAX_QTY - item[index].quantity);
			qtyInput = getIntLimited(0, MAX_QTY - item[index].quantity);

			if (qtyInput == 0) {
				printf("--== Aborted! ==--\n");
			}
			else {
				item[index].quantity = item[index].quantity + qtyInput;
				printf("--== Stocked! ==--\n");
			}

			if (isLowQuantity(item[index]))
				printf("Quantity is low, please reorder ASAP!!!\n");

			// checkout item
		}
		else if (stock == CHECKOUT) {
			printf("Please enter the quantity to checkout; Maximum of %d or 0 to abort: ",
				item[index].quantity);
			qtyInput = getIntLimited(0, item[index].quantity);

			if (qtyInput == 0) {
				printf("--== Aborted! ==--\n");
			}
			else {
				item[index].quantity = item[index].quantity - qtyInput;
				printf("--== Checked out! ==--\n");
			}

			if (isLowQuantity(item[index]))
				printf("Quantity is low, please reorder ASAP!!!\n");

		}

	}
	else {
		printf("SKU not found in storage!\n");
	}
}

// Function Implementations Milestone 4:
void saveItem(struct Item item, FILE* dataFile)
{
	fprintf(dataFile, "%d,%d,%d,%.2lf,%d,%s\n", item.sku, item.quantity, item.minQuantity, item.price,
		item.isTaxed, item.name);
}

int loadItem(struct Item* item, FILE* dataFile)
{
	int rvalue; // return value
	int readNo;

	readNo = fscanf(dataFile, "%d,%d,%d,%lf,%d,%[^\n]%*c", &(item->sku), &(item->quantity), &(item->minQuantity), &(item->price), &(item->isTaxed), item->name);

	if (readNo == 6)
		rvalue = 1;
	else
		rvalue = 0;

	return rvalue;
}

int saveItems(const struct Item item[], char fileName[], int NoOfRecs)
{
	FILE *fp = NULL;
	int rvalue;
	int i;

	fp = fopen(fileName, "w");

	if (fp != NULL) {

		for (i = 0; i < NoOfRecs; i++)
			saveItem(item[i], fp);

		fclose(fp);
		rvalue = 1;
	}
	else {
		rvalue = 0;
	}

	return rvalue;
}

int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr)
{
	FILE *fp;
	int rvalue;
	*NoOfRecsPtr = 0;

	fp = fopen(fileName, "r");

	if (fp != NULL) {

		while (loadItem(&item[*NoOfRecsPtr], fp))
			(*NoOfRecsPtr)++; // count the number of item read

		fclose(fp);
		rvalue = 1;
	}
	else {
		rvalue = 0;
	}

	return rvalue;
}
