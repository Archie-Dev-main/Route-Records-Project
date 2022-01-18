#include <stdio.h>
#include <stdlib.h>
#include "route-records.h"

int main(int argc, char *argv[])
{
    FILE* inFile = NULL;
    RouteRecord* records = NULL;
    char origin[4];
    char destination[4];
    char airline[3];
    int recordCount;
    char uInput[3];
    
    // Checks command line arguments for missing args
    if (argc < 2) {
        printf("ERROR: Missing file name");
        return -1;
    }
    
    // Attempts to open the file in the command line argv[1] and ends the program if it cannot find it
    printf("Opening %s...\n", argv[1]);
    inFile = fopen(argv[1], "r");
    if (inFile == NULL) {
        printf("ERROR: Could not open file");
        return -1;
    }
    
    // Creates and loads records into the records array
    records = createRecords(inFile);
    recordCount = fillRecords(records, inFile);
    printf("Unique routes operated by airlines: %d", recordCount);
    fclose(inFile);

    // Infinite loop for user interaction
    while (1)
    {
        // Prints the selection menu and takes in user input, taking extra care to flush the input buffer
        printMenu();
        fflush(stdin);
        fgets(uInput, 3, stdin);
        fflush(stdin);
        if (uInput[1] != '\n') {
            uInput[0] = '6';
        }
        // Determines what type of search to do based on user input
        switch (uInput[0])
        {
            // Prompts the user and carries out the search the user selected, handling invalid input along the way, and having an option for the user to quit the program
            // 1 does a search based on the route(origin and destination)
            // 2 does a search based on the origin
            // 3 does a search based on the destination
            // 4 does a search based on the airline
            // 5 quits the program
            // default handles invalid inputs
            case '1':
                printf("Enter origin: ");
                scanf("%s", origin);
                printf("Enter destination: ");
                scanf("%s", destination);
                searchRecords(records, recordCount, origin, destination, ROUTE);
                break;
            case '2':
                printf("Enter origin: ");
                scanf("%s", origin);
                searchRecords(records, recordCount, origin, "", ORIGIN);
                break;
            case '3':
                printf("Enter destination: ");
                scanf("%s", destination);
                searchRecords(records, recordCount, "", destination, DESTINATION);
                break;
            case '4':
                printf("Enter airline: ");
                scanf("%s", airline);
                searchRecords(records, recordCount, airline, "", AIRLINE);
                break;
            case '5':
                printf("Good-bye!\n");
                free(records);
                return 0;
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    
    return 0;
}