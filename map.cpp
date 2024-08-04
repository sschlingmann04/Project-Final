#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include "map.hpp"
using namespace std;
using namespace std::chrono;


float ValidateDuration(string& d) {
    float duration;
    while (true) {
        try {
            duration = stof(d);
            return duration;
        }
        catch (const invalid_argument& e) {
            cerr << "Invalid duration, please reenter: ";
            cin >> d;
        }
        catch (const out_of_range& e) {
            cerr << "Duration is out of range, please reenter: ";
            cin >> d;
        }
    }
}

int ValidateYear(string& y) {
    int year;
    while (true) {
        try {
            year = stoi(y);
            return year;
        }
        catch (const invalid_argument& e) {
            cerr << "Invalid year, please reenter: ";
            cin >> y;
        }
        catch (const out_of_range& e) {
            cerr << "Year is out of range, please reenter: ";
            cin >> y;
        }
    }
}


int main()
{
    auto startInitialization = steady_clock::now();
    Map sample;
    auto stopInitialization = steady_clock::now();
    auto durationInitialization = duration_cast<milliseconds>(stopInitialization - startInitialization);
    cout << "Map initialization complete, took " << durationInitialization.count() << " milliseconds." << endl;

    int option;
    int sub_option;

    do {
        cout << "\nPlease select an option:\n1. Print all UFO sightings\n2. Search all sightings based on a city/state/shape/year\n";
        cout << "3. Insert a new UFO sighting\n4. Remove all sightings from a city & state\nPress any other digit to exit program.\n";
        cin >> option;

        if (option == 1) {  // print all UFO sightings
            auto startPrint = steady_clock::now();
            sample.Print();
            auto stopPrint = steady_clock::now();
            auto durationPrint = duration_cast<milliseconds>(stopPrint - startPrint);
            cout << "Printing complete, took " << durationPrint.count() << " milliseconds." << endl;
        }

        else if (option == 2) {  // searching
            cout << "\nHow would you like to search by?\n1. City & State\n2. State only\n3. Shape\n4. Year\n";
            cin >> sub_option;
            switch (sub_option) {
                case 1: {  // search by city + state
                    string city, state;
                    cout << "Enter city: ";
                    cin.get();
                    getline(cin, city);
                    cout << "Enter state: ";
                    cin >> state;

                    auto startCitySearch = steady_clock::now();
                    int count = sample.SearchCity(city, state);
                    auto stopCitySearch = steady_clock::now();
                    auto durationCitySearch = duration_cast<milliseconds>(stopCitySearch - startCitySearch);
                    cout << "Number of sightings in " << city << ", " << state << ": " << count << " | procedure took " << durationCitySearch.count() << " milliseconds.\n";
                    break;
                }

                case 2: {  // search by state
                    string state;
                    cout << "Enter state to search for (two-letter uppercase postal abbreviation): ";
                    cin >> state;

                    auto startStateSearch = steady_clock::now();
                    int count = sample.SearchState(state);
                    auto stopStateSearch = steady_clock::now();
                    auto durationStateSearch = duration_cast<milliseconds>(stopStateSearch - startStateSearch);
                    cout << "Number of sightings in " << state << ": " << count << " | procedure took " << durationStateSearch.count() << " milliseconds.\n";
                    break;
                }

                case 3: {  // search by shape
                    string shape;
                    cout << "Enter shape to search for (all lowercase): ";
                    cin >> shape;

                    auto startShapeSearch = steady_clock::now();
                    int count = sample.SearchShape(shape);
                    auto stopShapeSearch = steady_clock::now();
                    auto durationShapeSearch = duration_cast<milliseconds>(stopShapeSearch - startShapeSearch);
                    cout << "Number of " << shape << "-shaped sightings: " << count << " | procedure took " << durationShapeSearch.count() << " milliseconds.\n";
                    break;
                }

                case 4: {  // search by year
                    string y;
                    cout << "Enter year to search for: ";
                    cin >> y;
                    int year = ValidateYear(y);

                    auto startYearSearch = steady_clock::now();
                    int count = sample.SearchYear(year);
                    auto stopYearSearch = steady_clock::now();
                    auto durationYearSearch = duration_cast<milliseconds>(stopYearSearch - startYearSearch);
                    cout << "Number of sightings in " << year << ": " << count << " | procedure took " << durationYearSearch.count() << " milliseconds.\n";
                    break;
                }

                default: {
                    cout << "Invalid selection, please try again." << endl;
                    break;
                }
            }
        }

        else if (option == 3) {  // inserting a new UFO sighting
            string city, state, shape, d, y = "";
            while (true) {
                cout << "Enter the following information in this format: [city] [state] [shape] [duration] [year], all separated by a single comma. (e.g. saint petersburg,FL,circle,7.5,2004)\n";
                string line;
                cin.get();
                getline(cin, line);
                istringstream stream(line);
                getline(stream, city, ',');
                getline(stream, state, ',');
                getline(stream, shape, ',');
                getline(stream, d, ',');
                getline(stream, y, ',');
                if (city == "" || state == "" || shape == "" || d == "" || y == "")
                    cerr << "Error! Not all arguments were entered. Please try again.\n";
                else
                    break;
            }
            float duration = ValidateDuration(d);
            int year = ValidateYear(y);

            auto startInsert = steady_clock::now();
            sample.Insert(city, state, shape, duration, year);
            auto stopInsert = steady_clock::now();
            auto durationInsert = duration_cast<milliseconds>(stopInsert - startInsert);
            cout << "Insertion complete, took " << durationInsert.count() << " milliseconds.\n";
        }

        else if (option == 4) {  // removing all sightings from a city + state
            string city, state;
            cout << "Enter city: ";
            cin.get();
            getline(cin, city);
            cout << "Enter state: ";
            cin >> state;

            auto startRemove = steady_clock::now();
            int count = sample.Remove(city, state);
            auto stopRemove = steady_clock::now();
            auto durationRemove = duration_cast<milliseconds>(stopRemove - startRemove);
            if (count != 0)
                cout << count << " records for " << city << ", " << state << " successfully removed!" << endl;
            else
                cout << "There were no records matching " << city << ", " << state << "." << endl;
            cout << "Procedure took " << durationRemove.count() << " milliseconds.\n";
        }

        else
            cout << "Exiting program..." << endl;
    }
    while (option >= 1 && option <= 4);

    return 0;
}