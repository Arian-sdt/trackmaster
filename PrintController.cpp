/*
PrintController.cpp
--------------------------------------------------------------------------------------------------------------------
Revision History:
Rev. 1 - 2024/07/13 Original by Bowen Jin
Rev. 2 - 2024/07/30 
--------------------------------------------------------------------------------------------------------------------
This Cpp file includes all the implemntaions related to the header file 
--------------------------------------------------------------------------------------------------------------------
*/

#include "PrintController.h"
#include "EntityIO.h"
#include "globalEntities.h"
#include "Change.h"
#include <fstream>

// Static file stream for handling print operations
static std::ofstream printFile;
// global variable to keep count of how many items have been printed


namespace PrintController {
    // -------------------------------------------------------------------------------------------------------------------
    // Initialize the print controller
    // Opens the print output file, creating it if necessary, and initializes the print item count.
    // -------------------------------------------------------------------------------------------------------------------
    void initPrintController() {
        // open the print file, clean out what is inside the file 
        printFile.open("print_output.txt", std::ofstream::out | std::ofstream::trunc);
        // if the file doesn't exist make one 
        if (!printFile) {
            std::ofstream { "product.dat" };
            // Open for reading and writing
            printFile.open("product.dat", std::ios::binary);
        } 
        // throw an exception if the file open failed
        if (!printFile) {
        throw FileOpenFailedException("File open failed");
        }
    }

    // -------------------------------------------------------------------------------------------------------------------
    // Print product information
    // Generates a report for a specified product release's change items.
    // -------------------------------------------------------------------------------------------------------------------
    void printProduct(string productName, int releaseID) {
        // check to make sure the file is open 
        static int printItemCount = 0;
        if (!printFile.is_open()) {
            throw FileNotOpenException("File is not open");
        }

        // go to the begining of the change file 
        changeIO.seekToStart();
        // make a new instance of change object 
        Change* outputChangeItem = new Change(); 

        // read the first change item 
        outputChangeItem = changeIO.readRecord();

        // while the getChangeRecord has not returned a nullptr loop through the file 
        while(outputChangeItem != nullptr) {
            // check to see if the instance matches the Product release details 
            if (outputChangeItem->getProductName() == productName && 
                outputChangeItem->getAnticipatedReleaseID() == releaseID) {
                
                // Print header if its the first change item 
                if (printItemCount == 0) {
                    const char * ProductName = outputChangeItem->getProductName();
                    int anticipatedReleaseID = outputChangeItem->getAnticipatedReleaseID();
                    printFile << "\n" << "Report for " << '"' << ProductName << '"' << " release " << '"' << anticipatedReleaseID  << '"' << "\n";
                }

                // pull all the neccesary details from the change item 
                const char * ProductName = outputChangeItem->getProductName();
                int ChangeID = outputChangeItem->getchangeID();
                char * Date = outputChangeItem->getDate();
                Change::Status status = outputChangeItem->getStatus();
                int anticipatedReleaseID = outputChangeItem->getAnticipatedReleaseID();
                const char * Description = outputChangeItem->getDescription();

                // print it into file 
                printFile << "Change item report " << (printItemCount + 1) << ":\n";
                printFile << " Product Name: " << ProductName << "\n"; 
                printFile << " Change ID: " << ChangeID << "\n"; 
                printFile << " First Reported: " << Date << "\n"; 
                printFile << " Status: " << Change::statusToString(status) << "\n";
                printFile << " Anticipated Release: " << anticipatedReleaseID << "\n"; 
                printFile << " Description: " << Description << "\n\n\n"; 
                
                // enumarate count
                printItemCount++;
            }

            // get the next record
            outputChangeItem = changeIO.readRecord();
        }

        // free the Change Item instance 
        free(outputChangeItem);
        return;
    }

    // -------------------------------------------------------------------------------------------------------------------
    // Print completed change items
    // Generates a report for completed change items, assuming similar logic for changes.
    // -------------------------------------------------------------------------------------------------------------------
    void printCompletedChangeItems(const Change &outputChangeItem) {

        //pull all the neccesary details from the change item 
        const char * ProductName = outputChangeItem.getProductName();
        int ChangeID = outputChangeItem.getchangeID();
        char * Date = outputChangeItem.getDate();
        Change::Status status = outputChangeItem.getStatus();
        int anticipatedReleaseID = outputChangeItem.getAnticipatedReleaseID();
        const char * Description = outputChangeItem.getDescription();

        // the header for the file 
        printFile << "\n" << "Report for " << '"' << ProductName << '"' << "\n";
        
        // print all the information into the file 
        printFile << " Product Name: " << ProductName << "\n"; 
        printFile << " Change ID: " << ChangeID << "\n"; 
        printFile << " First Reported: " << Date << "\n"; 
        printFile << " Status: " << Change::statusToString(status) << "\n"; 
        printFile << " Anticipated Release: " << anticipatedReleaseID << "\n"; 
        printFile << " Description: " << Description << "\n\n\n";

        return;
    }

    // -------------------------------------------------------------------------------------------------------------------
    // Exit the print controller
    // Closes the print file if it is open.
    // -------------------------------------------------------------------------------------------------------------------
    void exitPrint() {
        if (printFile.is_open()) {
            printFile.close();
        }
    }
}


