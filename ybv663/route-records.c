#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route-records.h"

/*
This function
    reads through the file passed in through it's parameter,
    counts the number of lines/records in the file,
    dynamically allocates an array of RouteRecord structs the size of the number of lines/records,
    initializes the elements of passengers array of each struct in the array to 0,
    rewinds the file pointer to the beginning,
    and returns a RouteRecord pointer to the records array
*/
RouteRecord* createRecords(FILE* fileIn) {
    RouteRecord* records = NULL; // Dynamically allocated array
    int c; // holds an int for reading through the file
    int count = 0; // holds the count for the number of lines/records

    // Reads through the file and adds to the count variable for every line
    while (1) {
        c = fgetc(fileIn);
        if(feof(fileIn)) {
            break;
        }
        if (c == '\n') {
            count++;
        }
    }
    // Dynamic allocation of records array
    records = (RouteRecord*)malloc(count * sizeof(RouteRecord));
    // Goes through the records array and initializes the elements of the passengers arrays to 0 in the RouteRecords structs
    for (int i = 0; i < count; ++i) {
        for (int k = 0; k < 6; ++k) {
            records[i].passengers[k] = 0;
        }
    }
    // Rewinds and returns
    rewind(fileIn);
    return records;
}

/*
This function
    skips the header of the file that was passed in,
    uses a while loop to...
    scan data from the file that was passed in,
    find out whether the read data is a unique record or if the record is one month of data for a different record that was determined to be unique using a recursive function that is called,
    if the record is not unique(result > -1) then the passenger data is added to the prexisting record's passenger array according to the month(with regards to array indices),
    if the record is unique then the record is added to the records array by changing the values of the members of the RouteRecord structs and the length variable is incremented to represent the number of unique records in the array,
    finally the program returns an int that is the number of unique records added
*/
int fillRecords(RouteRecord* r, FILE* fileIn) {
    char r_origin[4];
    char r_destination[4];
    char r_airline[3];
    char passengers[11];
    int length = 0;
    int month;
    int passengerCount;
    int result;
    int c;

    // header skipping code
    while (1) 
    {
        c = fgetc(fileIn);
        if (c == '\n') {
            break;
        }
    }

    while (1) 
    {
        // Data scanned in and assigned to temp variables
        fscanf(fileIn, "%d,%[^,],%[^,],%[^,],%[^,],%d", &month, r_origin, r_destination, r_airline, passengers, &passengerCount);
        // Call to a recursive search function, the result of which determines whether a record is unique
        result = findAirlineRoute(r, length, r_origin, r_destination, r_airline, length);
        // Determining logic for what happens when a record is unique or not(first branch runs if a record is not unique, the second branch runs if a record is unique)
        if (result > -1) {
            r[result].passengers[month - 1] = passengerCount;
        }
        else {
            strcpy(r[length].origin, r_origin);
            strcpy(r[length].destination, r_destination);
            strcpy(r[length].airline, r_airline);
            r[length].passengers[month-1] = passengerCount;
            length++;
        }
        if (feof(fileIn)) {
            break;
        }
    }
    // Return value, number of unique records
    return length;
}

/*
This function,
    takes in a record passed in by the parameters origin, destination, and airline
    checks the current index parameter,
    if it's less than zero it returns a -1 which means that the record it's checking is unique,
    if it finds the record in the records array then it returns the index of the records array that contains the record compared,
    if it does not find the record but the current index is not less than 0 then it recursively calls itself with a new current index less than the one started with
*/
int findAirlineRoute(RouteRecord* r, int length, const char* origin, const char* destination, const char* airline, int curIdx) {
    if (curIdx < 0) {
        return -1;
    }
    else if (strcmp(r[curIdx].origin, origin) == 0 && strcmp(r[curIdx].destination, destination) == 0 && strcmp(r[curIdx].airline, airline) == 0) {
        return curIdx;
    }
    else {
        findAirlineRoute(r, length, origin, destination, airline, curIdx - 1);
    }
}

/*
This function
    uses the st parameter to choose a branch,
    every branch will count up the total numbner of passengers and total number of passengers for every month based on what search type they are and also prints out the matches it finds,
    the first branch is for if st is ROUTE meaning that both key1 and key2 are compared to all of the records from the records array too see if both key1 and key2 match both the origin and destination members of any of the RouteRecord structs in the records array,
    the second branch is for if st is ORIGIN meaning that only key1 is compared to all of the records from the records array too see if key1 matches the origin member of any of the RouteRecord structs in the records array,
    the third branch is for if st is DESTINATION meaning that only key2 is compared to all of the records from the records array too see if key2 matches the destination member of any of the RouteRecord structs in the records array,
    the fourth branch is for if st is AIRLINE meaning that only key1 is compared to all of the records from the records array too see if key1 matches the airline member of any of the RouteRecord structs in the records array,
    finally the general statistics of the search performed by this function are printed, the number of matches found, the total passengers, the total passengers for each month, and average number of passengers per month
*/
void searchRecords(RouteRecord* r, int length, const char* key1, const char* key2, SearchType st) {
    int count;
    int i;
    int totalPassengers = 0;
    int totalPassengersPerMonth[6];
    int monthSum = 0;
    int k;
    for (i = 0; i < 6; ++i) {
        totalPassengersPerMonth[i] = 0;
    }
    // ROUTE based search
    if (st == ROUTE) {
        count = 0;
        printf("\nSearching by route...\n");
        for (i = 0; i < length; ++i) {
            if (strcmp(key1, r[i].origin) == 0 && strcmp(key2, r[i].destination) == 0) {
                printf("%s (%s - %s) ", r[i].airline, r[i].origin, r[i].destination);
                for (k = 0; k < 6; ++k) {
                    totalPassengers += r[i].passengers[k];
                }
                totalPassengersPerMonth[0] += r[i].passengers[0];
                totalPassengersPerMonth[1] += r[i].passengers[1];
                totalPassengersPerMonth[2] += r[i].passengers[2];
                totalPassengersPerMonth[3] += r[i].passengers[3];
                totalPassengersPerMonth[4] += r[i].passengers[4];
                totalPassengersPerMonth[5] += r[i].passengers[5];
                count++;
            }
        }
    }
    // ORIGIN based search
    else if (st == ORIGIN) {
        count = 0;
        printf("\nSearch by origin...\n");
        for (i = 0; i < length; ++i) {
            if (strcmp(key1, r[i].origin) == 0) {
                printf("%s (%s - %s) ", r[i].airline, r[i].origin, r[i].destination);
                for (k = 0; k < 6; ++k) {
                    totalPassengers += r[i].passengers[k];
                }
                totalPassengersPerMonth[0] += r[i].passengers[0];
                totalPassengersPerMonth[1] += r[i].passengers[1];
                totalPassengersPerMonth[2] += r[i].passengers[2];
                totalPassengersPerMonth[3] += r[i].passengers[3];
                totalPassengersPerMonth[4] += r[i].passengers[4];
                totalPassengersPerMonth[5] += r[i].passengers[5];
                count++;
            }
            
        }
    }
    // DESTINATION based search
    else if (st == DESTINATION) {
        count = 0;
        printf("\nSearching by destination...\n");
        for (i = 0; i < length; ++i) {
            if (strcmp(key2, r[i].destination) == 0) {
                printf("%s (%s - %s) ", r[i].airline, r[i].origin, r[i].destination);
                for (k = 0; k < 6; ++k) {
                    totalPassengers += r[i].passengers[k];
                }
                totalPassengersPerMonth[0] += r[i].passengers[0];
                totalPassengersPerMonth[1] += r[i].passengers[1];
                totalPassengersPerMonth[2] += r[i].passengers[2];
                totalPassengersPerMonth[3] += r[i].passengers[3];
                totalPassengersPerMonth[4] += r[i].passengers[4];
                totalPassengersPerMonth[5] += r[i].passengers[5];
                count++;
            }
        }
    }
    // AIRLINE based search
    else {
        count = 0;
        printf("\nSearch by airline...\n");
        for (i = 0; i < length; ++i) {
            if (strcmp(key1, r[i].airline) == 0) {
                printf("%s (%s - %s) ", r[i].airline, r[i].origin, r[i].destination);
                for (k = 0; k < 6; ++k) {
                    totalPassengers += r[i].passengers[k];
                }
                totalPassengersPerMonth[0] += r[i].passengers[0];
                totalPassengersPerMonth[1] += r[i].passengers[1];
                totalPassengersPerMonth[2] += r[i].passengers[2];
                totalPassengersPerMonth[3] += r[i].passengers[3];
                totalPassengersPerMonth[4] += r[i].passengers[4];
                totalPassengersPerMonth[5] += r[i].passengers[5];
                count++;
            }
        }
    }

    // General search statistics of the matches found, total passengers, total number of passengers for each month, and average number of passengers per month
    printf("\n%d matches were found.\n", count);
    printf("\nStatistics\n");
    printf("Total Passengers:\t\t\t%d\n", totalPassengers);
    printf("Total Passengers in Month 1:\t\t%d\n", totalPassengersPerMonth[0]);
    printf("Total Passengers in Month 2:\t\t%d\n", totalPassengersPerMonth[1]);
    printf("Total Passengers in Month 3:\t\t%d\n", totalPassengersPerMonth[2]);
    printf("Total Passengers in Month 4:\t\t%d\n", totalPassengersPerMonth[3]);
    printf("Total Passengers in Month 5:\t\t%d\n", totalPassengersPerMonth[4]);
    printf("Total Passengers in Month 6:\t\t%d\n", totalPassengersPerMonth[5]);
    for (i = 0; i < 6; ++i) {
        monthSum += totalPassengersPerMonth[i];
    }
    printf("\nAverage Passengers per Month:\t\t%d\n", monthSum/6);
}

/*
This function
    goes through the records array and prints the records in a viewable format
*/
void printRecords(RouteRecord* r, int length) {
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        for (int k = 0; k < 6; ++k) {
            sum += r[i].passengers[k];
        }
        printf("%s,%s,%s,%d", r[i].origin, r[i].destination, r[i].airline, sum);
    }
}

/*
This function
    prints a selection menu promting the user what to enter
*/
void printMenu()
{
    printf("\n\n######### Airline Route Records Database MENU #########\n");
    printf("1. Search by Route\n");
    printf("2. Search by Origin Airport\n");
    printf("3. Search by Destination Airport\n");
    printf("4. Search by Airline\n");
    printf("5. Quit\n");
    printf("Enter your selection: ");
}