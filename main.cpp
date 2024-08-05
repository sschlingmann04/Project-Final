#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "HashMap.h"
#include "Map.h"
#include "entryWindow.h"
#include <tuple>

using namespace std::chrono;
using namespace std;

void openFile(HashMap& hashMap, const string& filename) {
    //int lineCount = 0;
    ifstream file(filename);
    string line, state, city, shape, duration, year;
    auto startHashMap = steady_clock::now();
    if (file.is_open()) {
        getline(file, line);
        while (getline(file, line)) {
            istringstream ss(line);
            if (getline(ss, city, ',') && getline(ss, state, ',') &&
                getline(ss, shape, ',') && getline(ss, duration, ',') &&
                getline(ss, year, ',')) {
                string combined = "State: " + state + ", City: " + city + ", Shape: " + shape + ", Duration: " + duration + ", Year: " + year;
                string uniqueKey = city + state + shape + duration + year;
                hashMap.insert(uniqueKey, combined);
                //customMap.Insert(city, state, shape, stof(duration), stoi(year));
                //lineCount++;
                //cout << lineCount << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "failed to open " << filename << endl;
        return;
    }
    auto stopHashMap = steady_clock::now();
    auto durationHashMap = duration_cast<milliseconds>(stopHashMap - startHashMap);
    cout << "Inserting data into HashMap took " << durationHashMap.count() << " milliseconds." << endl;
}

int main() {
    sf::Font font;
    font.loadFromFile("../files/font.ttf");

    int width = 1500;
    int height = 1000;

    sf::Text topText;
    topText.setFont(font);
    topText.setCharacterSize(50);
    topText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    topText.setOutlineColor(sf::Color::Black);
    topText.setOutlineThickness(3);
    topText.setFillColor(sf::Color::White);
    topText.setPosition(width / 2.0f, height / 8.0f);
    topText.setOrigin(topText.getGlobalBounds().width / 2.0f, 0);

    sf::Text output; // outputs map
    output.setFont(font);
    output.setStyle(sf::Text::Bold);
    output.setFillColor(sf::Color::White);
    output.setCharacterSize(30);
    output.setPosition(width/ 4.0f, height / 5.0f + 14);
    output.setOrigin(output.getLocalBounds().width / 2.0f, 0);

    sf::Text outputHash; // outputs hashMap
    outputHash.setFont(font);
    outputHash.setStyle(sf::Text::Bold);
    outputHash.setFillColor(sf::Color::White);
    outputHash.setCharacterSize(12);
    outputHash.setPosition(width / 1.8f, height / 5.0f + 14);
    outputHash.setOrigin(outputHash.getLocalBounds().width / 2.0f, 0);

    sf::Texture ufoImage2; // sets the background image
    ufoImage2.loadFromFile("../files/images/ufoResults.png");

    sf::Sprite ufo;
    ufo.setTexture(ufoImage2);
    ufo.setPosition(width / 2.0f, height / 2.0f - 75);
    ufo.setOrigin(ufo.getLocalBounds().width / 2, ufo.getLocalBounds().height / 2.35);
    ufo.setScale(2, 2);
    ufo.setColor(sf::Color(255, 255, 255, 128));

    tuple<string, string, bool> entry = createEntryWindow(); // makes the entry window

    // TODO: LOAD DATA FROM CSV TO THE DATA STRUCTURES
    // TODO: OUTPUT THIS INTO THE SECOND PAGE SO THAT THE USER CAN READ IT
    auto start = steady_clock::now();
    HashMap hashMap(1000);
    auto startMap = steady_clock::now();
    Map customMap;
    auto stopMap = steady_clock::now();
    auto durationMap = duration_cast<milliseconds>(stopMap - startMap);
    cout << "Inserting Map data took " << durationMap.count() << " milliseconds." << endl;
    openFile(hashMap, "../testingUFO.csv");
    auto stop = steady_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Inserting total data took " << duration.count() << " milliseconds." << endl;

    string strHashMap = "Hashmap";
    string strMap = "Map";
    topText.setString("Searching for UFOs via " + get<0>(entry) + ": " + get<1>(entry));

    // TODO: OUTPUT THE TIME CALCULATIONS INTO THE SECOND PAGE SO THAT THE USER CAN READ IT
    if (get<0>(entry) == "City"){
        auto startCityHashMap = steady_clock::now();
        strHashMap = hashMap.searchCity(get<1>(entry));
        auto stopCityHashMap = steady_clock::now();
        auto durationCityHashMap = duration_cast<milliseconds>(stopCityHashMap - startCityHashMap);
        cout << "HashMap search for city took " << durationCityHashMap.count() << " milliseconds." << endl;

    }
    else if (get<0>(entry) == "State"){
        auto startStateHashMap = steady_clock::now();
        strHashMap = hashMap.searchState(get<1>(entry));
        auto stopStateHashMap = steady_clock::now();
        auto durationStateHashMap = duration_cast<milliseconds>(stopStateHashMap - startStateHashMap);
        cout << "HashMap search for state took " << durationStateHashMap.count() << " milliseconds." << endl;
    }
    else if (get<0>(entry) == "Shape"){
        auto startShapeHashMap = steady_clock::now();
        strHashMap = hashMap.searchShape(get<1>(entry));
        auto stopShapeHashMap = steady_clock::now();
        auto durationShapeHashMap = duration_cast<milliseconds>(stopShapeHashMap - startShapeHashMap);
        cout << "HashMap search for shape took " << durationShapeHashMap.count() << " milliseconds." << endl;
    }
    else if (get<0>(entry) == "Year"){
        auto startYearHashMap = steady_clock::now();
        strHashMap = hashMap.searchYear(get<1>(entry));
        auto stopYearHashMap = steady_clock::now();
        auto durationYearHashMap = duration_cast<milliseconds>(stopYearHashMap - startYearHashMap);
        cout << "HashMap search for year took " << durationYearHashMap.count() << " milliseconds." << endl;
    }
    else{
        topText.setString("All UFO Sightings in the USA");
        auto startPrintHashMap = steady_clock::now();
        strHashMap = hashMap.print();
        auto stopPrintHashMap = steady_clock::now();
        auto durationPrintHashMap = duration_cast<milliseconds>(stopPrintHashMap - startPrintHashMap);
        cout << "HashMap print took " << durationPrintHashMap.count() << " milliseconds." << endl;
    }


    // Creating the second window to display the data
    sf::RenderWindow Results(sf::VideoMode(1500, 1000), "Hashmap vs Hashtable", sf::Style::Close | sf::Style::Titlebar);

    // creating a static window so that the user can scroll down
    sf::View view(sf::FloatRect(0, 0, width, height));
    view.setCenter(width / 2 , height / 2);
    sf::RectangleShape largeContent(sf::Vector2f(800, 2000));
    largeContent.setFillColor(sf::Color::Green);


    while (Results.isOpen()) {
        sf::Event event;

        // center text
        topText.setOrigin(topText.getGlobalBounds().width / 2.0f, 0);

        while (Results.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                Results.close();
            if (event.type == sf::Event::MouseWheelScrolled){
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
                    // Adjust the view's center to scroll up or down
                    float scrollSpeed = 60.0f;
                    view.move(0, -event.mouseWheelScroll.delta * scrollSpeed);
                }
            }

            // adjust view of the screen while the user scrolls
            float viewHeight = height;
            float contentHeight = largeContent.getSize().y;
            sf::Vector2f viewCenter = view.getCenter();
            if (viewCenter.y - viewHeight / 2 < 0) {
                view.setCenter(viewCenter.x, viewHeight / 2);
            }
            else if (viewCenter.y + viewHeight / 2 > contentHeight){
                view.setCenter(viewCenter.x, contentHeight - viewHeight / 2);
            }

            output.setString(strMap);
            outputHash.setString(strHashMap);
        }

        // create elements for the results screen
        ufo.setTexture(ufoImage2);
        Results.clear(sf::Color((35, 35, 70)));
        Results.draw(ufo);
        Results.setView(view);
        Results.draw(output);
        Results.draw(outputHash);
        Results.draw(topText);
        Results.display();

    }

}
