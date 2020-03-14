// CRM <Project4.cpp>
// EE 312 Project 4 submission by
// Nishanth Navali
// nan686
// Slip days used: <0>
// Spring 2020

#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS]; // global customer array
int num_customers = 0; // number of customers counteer


int INVENTORY_BOTTLES = 0; // global bottles in inventory
int INVENTORY_RATTLES = 0; // global rattles in inventory
int INVENTORY_DIAPERS = 0; // global diapers in inventory

// create strings to compare

String bottles;
String rattles;
String diapers;

void createStrings() {
    bottles = StringCreate("Bottles");
    rattles = StringCreate("Rattles");
    diapers = StringCreate("Diapers");
}
void clearStrings() {
    StringDestroy(&bottles);
    StringDestroy(&rattles);
    StringDestroy(&diapers);
}

// output 1 for bottles, 2 for rattles, 3 for diapers, and 0 for none

int checkItem(String* item_name) {
    createStrings();
    int out = 0;
    if(StringIsEqualTo(item_name, &bottles)) {       out = 1;}
    else if(StringIsEqualTo(item_name, &rattles)) {  out = 2;}
    else if(StringIsEqualTo(item_name, &diapers)) {  out = 3;}
    clearStrings();
    return out;
}

// return index of customer in array, -1 if not there

int findCustomer(String *input_name) {
    int customerIndex = -1;
    for(int i = 0; i < num_customers; i++) {
        if(StringIsEqualTo(input_name, &customers[i].name)) { customerIndex = i;}
    }
    return customerIndex;
}

/* clear the inventory and reset the customer database to empty */

void reset(void) {
    for(int i = 0; i < num_customers; i++) {
        StringDestroy(&customers[i].name);
        customers[i].bottles = 0;
        customers[i].rattles = 0;
        customers[i].diapers = 0;
    }
    num_customers = 0;
    INVENTORY_RATTLES = 0;
    INVENTORY_DIAPERS = 0;
    INVENTORY_BOTTLES = 0;
}

// prints a summary of bottles, rattles, and diapers purchasers (HELPER METHOD)

void summarizeBottles() {
    int maxVal = 0;
    int maxIndex = -1;
    for(int i = 0; i < num_customers; i++) {
        if(customers[i].bottles > maxVal) {
            maxIndex = i;
            maxVal = customers[i].bottles;
        }
    }
    if(maxIndex == -1) {
        printf("no one has purchased any Bottles\n");
    }
    else {
        StringPrint(&customers[maxIndex].name);
        printf(" has purchased the most Bottles (%d)\n", customers[maxIndex].bottles);
    }
}

void summarizeRattles() {
    int maxVal = 0;
    int maxIndex = -1;
    for(int i = 0; i < num_customers; i++) {
        if(customers[i].rattles > maxVal) {
            maxIndex = i;
            maxVal = customers[i].rattles;
        }
    }
    if(maxIndex == -1) {
        printf("no one has purchased any Rattles\n");
    }
    else {
        StringPrint(&customers[maxIndex].name);
        printf(" has purchased the most Rattles (%d)\n", customers[maxIndex].rattles);
    }
}

void summarizeDiapers() {
    int maxVal = 0;
    int maxIndex = -1;
    for(int i = 0; i < num_customers; i++) {
        if(customers[i].diapers > maxVal) {
            maxIndex = i;
            maxVal = customers[i].diapers;
        }
    }
    if(maxIndex == -1) {
        printf("no one has purchased any Diapers\n");
    }
    else {
        StringPrint(&customers[maxIndex].name);
        printf(" has purchased the most Diapers (%d)\n", customers[maxIndex].diapers);
    }
}

// Prints a summary of inventory stock and highest purchasers of each item (FORMATTING)

void processSummarize() {

    printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n",
            INVENTORY_BOTTLES, INVENTORY_DIAPERS, INVENTORY_RATTLES);
    printf("we have had a total of %d different customers\n", num_customers);

    summarizeBottles();
    summarizeDiapers();
    summarizeRattles();

}

// output formatting for purchasing errors

void purchaseError(String* name) {
    printf("Sorry ");
    StringPrint(name);
    printf(" we only have ");
}

// customer purchases

void processPurchase() {

    // inputs
    String cname;
    String item;
    int purchase;

    // compare val to determine which item is purchased
    int compare;

    // read inputs
    readString(&cname);
    readString(&item);
    readNum(&purchase);

    // clear memory after checking which item
    compare = checkItem(&item);
    StringDestroy(&item);

    // check if customer already exists
    int customer_index = findCustomer(&cname);

    // error check for if the purchase amount is negative
    if(purchase < 0) {
        StringDestroy(&cname);
        return;
    }

    // check for stock

    if(compare == 1 && purchase > INVENTORY_BOTTLES) {
        purchaseError(&cname);
        printf("%d Bottles\n", INVENTORY_BOTTLES);
        StringDestroy(&cname);
        return;
    }
    else if(compare == 2 && purchase > INVENTORY_RATTLES) {
        purchaseError(&cname);
        printf("%d Rattles\n", INVENTORY_RATTLES);
        StringDestroy(&cname);
        return;
    }
    else if(compare == 3 && purchase > INVENTORY_DIAPERS) {
        purchaseError(&cname);
        printf("%d Diapers\n", INVENTORY_DIAPERS);
        StringDestroy(&cname);
        return;
    }

    // create new customer if new customer, else clear memory of name

    if(customer_index == -1) {
        // create new customer
        num_customers++;
        customer_index = num_customers - 1;
        customers[customer_index].name = cname;
    }
    else {
        StringDestroy(&cname);
    }

    // Actually purchase stuff w inventory edits and customer edits
    if(compare == 1) {
        customers[customer_index].bottles += purchase;
        INVENTORY_BOTTLES -= purchase;
    }
    else if(compare == 2) {
        customers[customer_index].rattles += purchase;
        INVENTORY_RATTLES -= purchase;
    }
    else if(compare == 3) {
        customers[customer_index].diapers += purchase;
        INVENTORY_DIAPERS -= purchase;
    }
    else {
        printf("Something went wrong in processPurchase\n");
    }
}

void processInventory() {

    String item;
    int new_stock;
    int compare;

    // read item type and number of items recieved
    readString(&item);
    readNum(&new_stock);

    if(new_stock < 0) {
        StringDestroy(&item);
        return;
    }

    // check if item is sold
    compare = checkItem(&item);

    // Add to correct item
    if(compare == 1) {      INVENTORY_BOTTLES += new_stock; }
    else if(compare == 2) { INVENTORY_RATTLES += new_stock; }
    else if(compare == 3) { INVENTORY_DIAPERS += new_stock; }
    else {             printf("Not a valid item to stock"); }

    // free string
    StringDestroy(&item);
}
