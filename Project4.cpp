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

int checkItem(String* item_name) { // output 1 for bottles, 2 for rattles, 3 for diapers, and 0 for none
    createStrings();
    int out = 0;
    if(StringIsEqualTo(item_name, &bottles)) {       out = 1;}
    else if(StringIsEqualTo(item_name, &rattles)) {  out = 2;}
    else if(StringIsEqualTo(item_name, &diapers)) {  out = 3;}
    clearStrings();
    return out;
}

int findCustomer(String *input_name) { // return index of customer in array, -1 if not there
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

void processSummarize() {

    printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n",
            INVENTORY_BOTTLES, INVENTORY_DIAPERS, INVENTORY_RATTLES);
    printf("we have had a total of %d different customers\n", num_customers);

    summarizeBottles();
    summarizeDiapers();
    summarizeRattles();

}

void purchaseError(String* name) {
    printf("Sorry ");
    StringPrint(name);
    printf(" we only have ");
}

void processPurchase() {

    String cname;
    String item;
    int purchase;
    int compare;

    readString(&cname);
    readString(&item);
    readNum(&purchase);

    compare = checkItem(&item);
    StringDestroy(&item);

    int customer_index = findCustomer(&cname);
    if(customer_index != -1) {
        StringDestroy(&cname);
        cname = customers[customer_index].name;
    }

    if(purchase < 0) return;

    // check for stock

    if(compare == 1 && purchase > INVENTORY_BOTTLES) {
        purchaseError(&cname);
        printf("%d Bottles\n", INVENTORY_BOTTLES);
        return;
    }
    else if(compare == 2 && purchase > INVENTORY_RATTLES) {
        purchaseError(&cname);
        printf("%d Rattles\n", INVENTORY_RATTLES);
        return;
    }
    else if(compare == 3 && purchase > INVENTORY_DIAPERS) {
        purchaseError(&cname);
        printf("%d Diapers\n", INVENTORY_DIAPERS);
        return;
    }

    // create new customer if new customer

    if(customer_index == -1) {
        // create new customer
        num_customers++;
        customer_index = num_customers - 1;
        customers[customer_index].name = cname; // TODO: change this if the name doesnt update
    }

    //-----------------------------------------------------------------------------------\\
    // Actually purchase stuff
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
    uint8_t compare;

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
