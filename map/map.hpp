#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "UFO.hpp"
using namespace std;

class Map
{
    private:
        UFO* sightings = new UFO[34];  // Our group number! :D
        int buckets = 34;
        int occupied = 0;
    public:
        Map();
        ~Map();
        void Rehash();
        void Insert(UFO& obj);
        void Insert(string& city, string& state, string& shape, float& duration, int& year);
        int Remove(string& city, string& state);
        int SearchCity(string& city, string& state);
        int SearchState(string& state);
        int SearchShape(string& shape);
        int SearchYear(int& year);
        void Print();
        double LoadFactor();
};


// Constructor
Map::Map() {
    ifstream inFile("../ufo_sightings.csv");

    if (inFile.is_open())
    {
        // Get header line and immediately throw it away
        string currLine;
        getline(inFile, currLine);

        // Continue reading lines and inserting into the map
        while (getline(inFile, currLine))
        {
            istringstream stream(currLine);
            UFO newUFO;
            newUFO.ReadLine(stream);
            Insert(newUFO);
        }
    }
    else
        cout << "An error has occurred!";
}

// Destructor
Map::~Map() {
    delete[] sightings;
}

// Handles rehashing when the load factor exceeds 0.8
void Map::Rehash()
{
    // copy array into temp array then delete array
    UFO* oldSightings = new UFO[buckets];
    copy(sightings, sightings + buckets, oldSightings);
    delete[] sightings;

    // create new array that is double the size and rehash all elements into the new array
    sightings = new UFO[buckets * 2];
    occupied = 0;
    for (int i = 0; i < buckets; i++) {
        if (oldSightings[i].getState() != "") {
            Insert(oldSightings[i]);
        }
    }

    // delete temp array and update bucket size
    delete[] oldSightings;
    buckets *= 2;
}

// Insert an object into the array-based map
void Map::Insert(UFO& obj) {
    if (LoadFactor() > 0.8)
        Rehash();
    int i = obj.Hash(buckets);

    // Open addressing with linear probing to insert into an empty bucket
    while (sightings[i].getState() != "")
        i = (++i) % buckets;
    sightings[i] = obj;
    occupied++;
}

// Parameritized insert function for the user to insert their own data
void Map::Insert(string& city, string& state, string& shape, float& duration, int& year) {
    UFO newUFO(city, state, shape, duration, year);
    Insert(newUFO);
}

// Removes all elements that match a city and state
int Map::Remove(string& city, string& state) {
    int count = 0;
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getCity() == city && sightings[i].getState() == state) {
            sightings[i].Reset();
            occupied--;
            count++;
        }
    }
    return count;
}

// Searches all elements that match a city and state
int Map::SearchCity(string& city, string& state) {
    int count = 0;
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getCity() == city && sightings[i].getState() == state) {
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
            count++;
        }
    }
    return count;
}

// Searches all ements that match an entire state
int Map::SearchState(string& state) {
    int count = 0;
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getState() == state) {
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
            count++;
        }
    }
    return count;
}

// Searches all elements that match a specific shape
int Map::SearchShape(string& shape) {
    int count = 0;
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getShape() == shape) {
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
            count++;
        }
    }
    return count;
}

// Searches all elements that match a specific year
int Map::SearchYear(int& year) {
    int count = 0;
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getYear() == year) {
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
            count++;
        }
    }
    return count;
}

// Prints out all occupied buckets (not any defined to be "empty")
void Map::Print() {
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getState() != "")
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
    }
    cout << "Total number of occupied buckets: " << occupied << endl;
}

// Accessor for load factor
double Map::LoadFactor() { return occupied / buckets; }