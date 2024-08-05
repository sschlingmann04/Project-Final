#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "HashMap.h"
#include "Map.h"
#include "Button.h"
#include <random>

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
    int width = 1500;
    int height = 1000;

    sf::RenderWindow window(sf::VideoMode(width, height), "UFO Sightings!", sf::Style::Close | sf::Style::Titlebar);

    // Load font
    sf::Font font;
    font.loadFromFile("../files/font.ttf");

    // Define button attributes
    sf::Vector2f buttonSize(160, 70);
    std::vector<std::string> labels = { "City", "State", "Shape", "Year" };
    std::vector<Button> buttons;

    // Create buttons for parameters
    for (size_t i = 0; i < labels.size(); i++) {
        buttons.emplace_back(buttonSize, sf::Vector2f(380 + i * 200, 360), labels[i], font);
    }

    // create submit button
    Button submit(buttonSize, {680, 600}, "Submit", font);

    // create Texts for Entry Page
    sf::Text welcomeText;
    welcomeText.setString("USA UFO SIGHTINGS!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(50);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setOutlineColor(sf::Color::Black);
    welcomeText.setOutlineThickness(3);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(width / 2.0f, height / 5.0f );
    welcomeText.setOrigin(welcomeText.getGlobalBounds().width / 2.0f, 0);

    sf::Text parameterPrompt; // display title and parameters
    parameterPrompt.setString(" Hashmap vs Map\nSelect Parameter:");
    parameterPrompt.setFont(font);
    parameterPrompt.setStyle(sf::Text::Bold);
    parameterPrompt.setFillColor(sf::Color::White);
    parameterPrompt.setCharacterSize(34);
    parameterPrompt.setPosition(width / 2.0f, height / 5.0f + 75);
    parameterPrompt.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text specific; // specified type of city, state, shape, and year for the search
    specific.setString("Enter Specific Search:");
    specific.setFont(font);
    specific.setStyle(sf::Text::Bold);
    specific.setFillColor(sf::Color::White);
    specific.setCharacterSize(34);
    specific.setPosition(width / 2.0f - 20, height / 2.0f - 20);
    specific.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text byPass; // bypass all parameters to print out all stored data
    byPass.setString("For all Sightings, Press Submit Only");
    byPass.setFont(font);
    byPass.setStyle(sf::Text::Bold);
    byPass.setFillColor(sf::Color(255,255,255,128));
    byPass.setCharacterSize(34);
    byPass.setPosition(width / 2.0f - 200, height / 2.0f + 340);
    byPass.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text output; // outputs map
    output.setFont(font);
    output.setStyle(sf::Text::Bold);
    output.setFillColor(sf::Color::White);
    output.setCharacterSize(30);
    output.setPosition(width / 2.0f, height / 5.0f + 75);
    output.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text outputHash; // outputs hashMap
    outputHash.setFont(font);
    outputHash.setStyle(sf::Text::Bold);
    outputHash.setFillColor(sf::Color::White);
    outputHash.setCharacterSize(10);
    outputHash.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text userInput;
    userInput.setFont(font);
    userInput.setCharacterSize(34);
    userInput.setStyle(sf::Text::Bold);
    userInput.setFillColor(sf::Color::Yellow);
    userInput.setPosition(width / 2.0f, height / 5.0f + 340);
    userInput.setOrigin(userInput.getLocalBounds().width / 2.0f, 0);

    sf::RectangleShape rectangle(sf::Vector2f(3, 34)); // created a cursor to follow text that the user enters
    rectangle.setFillColor(sf::Color::Yellow);
    rectangle.setPosition(width / 2.0f, height / 5.0f + 340);
    int adjustment = 0;
    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);

    // Create the background using purple and UFO images with AI using Bing Designer
    sf::Color colorPurple(35, 35, 70);
    sf::Texture ufoImage1;
    sf::Texture ufoImage2;
    ufoImage1.loadFromFile("../files/images/ufoImage.png");
    ufoImage2.loadFromFile("../files/images/ufoResults.png");

    sf::Sprite ufo;
    ufo.setTexture(ufoImage1);
    ufo.setPosition(width / 2.0f, height / 2.0f - 75);
    ufo.setOrigin(ufo.getLocalBounds().width / 2 , ufo.getLocalBounds().height / 2.35);
    ufo.setScale(2,2);
    ufo.setColor(sf::Color(255,255,255,128));

    bool windowVal = false; // boolean to check if entry page is open or closed
    bool finishedEntry = true;
    // strings to store user's options
    string parameter = ""; // button data
    string name = ""; // specific entry

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) { // collects characters for the specific entry
                if (event.text.unicode >= 32 ) {
                    name += event.text.unicode;

                    adjustment -= 20; // adjusts the cursor while the user is typing
                    userInput.setOrigin(userInput.getLocalBounds().width / 2.0f, 0);
                    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);

                }
                // readjust string
                userInput.setString(name);

                // ensure that the user provides an entry
                if (name != ""){
                    finishedEntry = true;
                }
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace) { // deletes characters for the specific entry
                    name.pop_back();
                    adjustment += 20;

                    if (adjustment > userInput.getLocalBounds().width / 2.0f) {
                        adjustment = userInput.getLocalBounds().width / 2.0f;
                    }
                    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);
                    userInput.setString(name); // readjust string
                }

                // ensure that the user provides an entry
                if (name == ""){
                    finishedEntry = false;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Check which parameter buttons were clicked and activate it
                for (auto& button : buttons) {
                    if (button.isClicked(mousePos)) {
                        // Deactivate all other buttons for parameters
                        for (auto &otherButton: buttons) {
                            otherButton.setActive(false);
                        }
                        // Activate the clicked button
                        button.setActive(true);
                        parameter = button.label;
                        finishedEntry = false;
                    }
                }
                // check if the submit button is clicked
                if (submit.isClicked(mousePos) && finishedEntry == true){
                    submit.setActive(true);
                    cout << "submitted parameter: " << parameter << "  \nspecific: " << name << "  \n"  <<  submit.label;
                    window.close();
                    windowVal = true; // opens next page
                }
            }
        }

        // Create opening window
        window.clear(colorPurple); // making background color
        window.draw(ufo);          // drawing the elements
        window.draw(welcomeText);
        window.draw(parameterPrompt);
        window.draw(specific);
        window.draw(byPass);
        window.draw(rectangle);
        window.draw(userInput);
        for (const auto& button : buttons) {
            window.draw(button.shape);
            window.draw(button.text);
        }
        window.draw(submit.shape);
        window.draw(submit.text);

        window.display();
    }

    // TODO: LOAD DATA FROM CSV TO THE DATA STRUCTURES
    auto start = steady_clock::now();
    HashMap hashMap(1000);
    auto startMap = steady_clock::now();
    Map customMap;
    auto stopMap = steady_clock::now();
    auto durationMap = duration_cast<milliseconds>(stopMap - startMap);
    cout << "Inserting Map data took " << durationMap.count() << " milliseconds." << endl;
    openFile(hashMap, "../ufo_sightings.csv");
    auto stop = steady_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Inserting total data took " << duration.count() << " milliseconds." << endl;

    string strHashMap = "Hashmap";
    string strMap = "Map";
    welcomeText.setString("Searching for UFOs via " + parameter + ": " + name);

    if (parameter == "City"){
        auto startCityHashMap = steady_clock::now();
        strHashMap = hashMap.searchCity(name);
        auto stopCityHashMap = steady_clock::now();
        auto durationCityHashMap = duration_cast<milliseconds>(stopCityHashMap - startCityHashMap);
        cout << "HashMap search for city took " << durationCityHashMap.count() << " milliseconds." << endl;

    }
    else if (parameter == "State"){
        auto startStateHashMap = steady_clock::now();
        strHashMap = hashMap.searchState(name);
        auto stopStateHashMap = steady_clock::now();
        auto durationStateHashMap = duration_cast<milliseconds>(stopStateHashMap - startStateHashMap);
        cout << "HashMap search for state took " << durationStateHashMap.count() << " milliseconds." << endl;

    }
    else if (parameter == "Shape"){
        auto startShapeHashMap = steady_clock::now();
        strHashMap = hashMap.searchShape(name);
        auto stopShapeHashMap = steady_clock::now();
        auto durationShapeHashMap = duration_cast<milliseconds>(stopShapeHashMap - startShapeHashMap);
        cout << "HashMap search for shape took " << durationShapeHashMap.count() << " milliseconds." << endl;

    }
    else if (parameter == "Year"){

        auto startYearHashMap = steady_clock::now();
        strHashMap = hashMap.searchYear(name);
        auto stopYearHashMap = steady_clock::now();
        auto durationYearHashMap = duration_cast<milliseconds>(stopYearHashMap - startYearHashMap);
        cout << "HashMap search for year took " << durationYearHashMap.count() << " milliseconds." << endl;

    }
    else{
        welcomeText.setString("All UFO Sightings in the USA");
        auto startPrintHashMap = steady_clock::now();
        strHashMap = hashMap.print();
        auto stopPrintHashMap = steady_clock::now();
        auto durationPrintHashMap = duration_cast<milliseconds>(stopPrintHashMap - startPrintHashMap);
        cout << "HashMap print took " << durationPrintHashMap.count() << " milliseconds." << endl;
    }


    // Creating the second window to display the data
    sf::RenderWindow Results(sf::VideoMode(width, height), "Hashmap vs Hashtable", sf::Style::Close | sf::Style::Titlebar);

    // creating a static window so that the user can scroll down
    sf::View view(sf::FloatRect(0, 0, width, height));
    view.setCenter(width / 2 , height / 2);
    sf::RectangleShape largeContent(sf::Vector2f(800, 2000));
    largeContent.setFillColor(sf::Color::Green);


    while (Results.isOpen() && windowVal == true) {
        sf::Event event;

        // center text
        welcomeText.setOrigin(welcomeText.getGlobalBounds().width / 2.0f, 0);

        while (Results.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                Results.close();
            if (event.type == sf::Event::MouseWheelScrolled){
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
                    // Adjust the view's center to scroll up or down
                    float scrollSpeed = 30.0f;
                    view.move(0, -event.mouseWheelScroll.delta * scrollSpeed);
                }
            }

            // adjust view of the screen while the user scrolls
            float viewHeight = window.getSize().y;
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

        output.setPosition(width/ 4.0f, height / 5.0f + 75);
        outputHash.setPosition(width / 2, height / 5.0f + 75);

        // create elements for the results screen
        ufo.setTexture(ufoImage2);
        Results.clear(colorPurple);
        Results.setView(view);
        Results.draw(output);
        Results.draw(outputHash);
        Results.draw(ufo);
        Results.draw(welcomeText);

        Results.display();
    }

}
