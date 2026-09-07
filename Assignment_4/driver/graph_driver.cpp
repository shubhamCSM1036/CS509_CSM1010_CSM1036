#include <iostream>
#include "../include/kmeans.h"
#include "../include/graph_driver.h"

using namespace std;

void assignment_4_func()
{
    cout << "\n 1. K means clustering"
         << "\n 2. FastMap"
         << "\n\n Enter your choice: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        kmeans_fn();
        break;

    case 2:
        // mm_fn();
        break;

    default:
        cout << "Invalid choice." << endl;
        break;
    }
}