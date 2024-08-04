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


Map::Map() {
    ifstream inFile("ufo_sightings.csv");

    if (inFile.is_open())
    {
        // Get header line and immediately throw it away
        string currLine;
        getline(inFile, currLine);

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

Map::~Map() {
    delete[] sightings;
}

void Map::Rehash()
{
    UFO* oldSightings = new UFO[buckets];
    copy(sightings, sightings + buckets, oldSightings);
    delete[] sightings;
    sightings = new UFO[buckets * 2];
    occupied = 0;
    for (int i = 0; i < buckets; i++) {
        if (oldSightings[i].getState() != "") {
            Insert(oldSightings[i]);
        }
    }
    delete[] oldSightings;
    buckets *= 2;
}

void Map::Insert(UFO& obj) {
    if (LoadFactor() > 0.8)
        Rehash();
    int i = obj.Hash(buckets);
    while (sightings[i].getState() != "")
        i = (++i) % buckets;
    sightings[i] = obj;
    occupied++;
}

void Map::Insert(string& city, string& state, string& shape, float& duration, int& year) {
    UFO newUFO(city, state, shape, duration, year);
    Insert(newUFO);
}

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

void Map::Print() {
    for (int i = 0; i < buckets; i++) {
        if (sightings[i].getState() != "")
            cout << "Bucket #" << i << " --> " << sightings[i].Print() << endl;
    }
    cout << "Total number of occupied buckets: " << occupied << endl;
}

double Map::LoadFactor() { return occupied / buckets; }