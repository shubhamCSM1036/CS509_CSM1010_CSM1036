#include <iostream>
#include <cstdlib>
#include "../Assignment_1/include/graph_driver.h"
#include "../Assignment_2/include/graph_driver.h"
#include "../Assignment_3/include/graph_driver.h"
#include "../Assignment_4/include/graph_driver.h"

using namespace std;

int main()
{
    int choice;

    do
    {
        cout << "\n===== CS509 LAB WRAPPER =====\n";
        cout << "1. Assignment 1\n";
        cout << "2. Assignment 2\n";
        cout << "3. Assignment 3\n";
        cout << "4. Assignment 4\n";
        cout << "0. Exit\n\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                driver_func();
                break;

            case 2:
                assignment_2_func();
                break;

            case 3:
                assignment_3_func();
                break;

            case 4:
                assignment_4_func();
                break;

            case 0:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}