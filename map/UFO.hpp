#pragma once

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class UFO
{
    private:
        string city;
        string state;
        string shape;
        float duration;
        int year;
    public:
        UFO();
        UFO(string c, string st, string sh, float d, int y);
        void ReadLine(istringstream& stream);
        string getCity();
        string getState();
        string getShape();
        float getDuration();
        int getYear();
        string FormatDuration();
        string Print();
        void Reset();
        int Hash(int buckets);
};


// Default constructor
UFO::UFO() : city(""), state(""), shape(""), duration(0.0f), year(0) {}

// Parameritized constructor
UFO::UFO(string c, string st, string sh, float d, int y) : city(c), state(st), shape(sh), duration(d), year(y) {}

// Reads a single line from the ufo_sightings.csv file and assigns all variables
void UFO::ReadLine(istringstream& stream)
{
    // city, state, and shape are all strings, no special consideration necessary
    getline(stream, city, ',');
    getline(stream, state, ',');
    getline(stream, shape, ',');

    // duration and year are integers, use a token to get string then convert using stoi() or stof()
    string token;
    getline(stream, token, ',');
    duration = stof(token);

    getline(stream, token, ',');
    year = stoi(token);
}

// Accessors
string UFO::getCity() { return city; }
string UFO::getState() { return state; }
string UFO::getShape() { return shape; }
float UFO::getDuration() { return duration; }
int UFO::getYear() { return year; }

// Used to trim off trailing zeros for clean printing of the duration
string UFO::FormatDuration()
{
    string d = to_string(duration);
    // Remove trailing zeros
    d.erase(d.find_last_not_of('0') + 1, string::npos);
    // If string ends with a decimal point, pop it off
    if (d.back() == '.')
        d.pop_back();
    return d;
}

// Prints all attributes in order
string UFO::Print()
{
    return "City: " + city + " | State: " + state + " | Shape: " + shape + " | Duration: " + FormatDuration() + " | Year: " + to_string(year);
}

// Resets this object's variables
void UFO::Reset() { city = ""; state = ""; shape = ""; duration = 0.0f; year = 0; }

// Hashing function
int UFO::Hash(int buckets)
{
    return ((year * int(duration)) / city.size()) % buckets;
}
