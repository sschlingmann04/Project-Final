#include "Hash Map.h"
#include <iostream>


using namespace std;

HashMap::HashMap(int size) : size(size), currentSize(0) {
	hashTable.resize(size, nullptr);
}

HashMap::~HashMap() {
	for (auto node : hashTable) {
		delete node;
	}
}

int HashMap::hashFunction(const string& key) {
	int hashValue = 0;
	for (char c : key) {
		hashValue = (hashValue * 31 + c) % size;
	}
	return hashValue;
}

int HashMap::nextSpace(int index, int position) {
	return (index + position) % size;
}

void HashMap::resize() {
	int newSize = size * 3;
	vector<HashNode*> newHashTable(newSize, nullptr);

	for (auto node : hashTable) {
		if (node != nullptr && !node->isDeleted) {
			int index = 0;
			int position = 0;
			int hashValue = 0;
			for (char c : node->key) {
				hashValue = (hashValue * 31 + c) % newSize;
			}
			index = hashValue;

			while (newHashTable[nextSpace(index, position)] != nullptr) {
				position++;
			}
			newHashTable[nextSpace(index, position)] = node;
		}
	}
	hashTable = newHashTable;
	size = newSize;
}

bool HashMap::insert(const string& key, const string& value) {
	if (currentSize >= size * 0.75) {
		resize();
	}

	int index = hashFunction(key);
	int position = 0;
	while (hashTable[nextSpace(index, position)] != nullptr && !hashTable[nextSpace(index, position)]->isDeleted) {
		position++;
	}

	if (hashTable[nextSpace(index, position)] == nullptr || hashTable[nextSpace(index, position)]->isDeleted) {
		delete hashTable[nextSpace(index, position)];
		hashTable[nextSpace(index, position)] = new HashNode(key, value);
		currentSize++;
		return true;
	}
	return false;
}


bool HashMap::remove(const string& city) {
	int linesRemoved = 0;
	bool found = false;
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			string value = hashTable[i]->value;
			size_t position = value.find("City: " + city);
			//https://stackoverflow.com/questions/66575296/can-anyone-explain-how-stdstringnpos-works
			//cout << "Checking value: " << value << " with position: " << position << endl;
			if (position != string::npos) {
				//cout << value << endl;
				hashTable[i]->isDeleted = true;
				currentSize--;
				found = true;
				linesRemoved++;
			}
		}
	}
	if (!found) {
		cout << "Couldnt find: " << city << endl;
	}
	cout << "Lines removed: " << linesRemoved << endl;
}

void HashMap::print() {
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			cout << hashTable[i]->value << endl;
		}
	}
}

int HashMap::getSize() {
	return currentSize;
}

void HashMap::searchCity(string& city) {
	bool found = false;
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			string value = hashTable[i]->value;
			size_t position = value.find("City: " + city);
			//https://stackoverflow.com/questions/66575296/can-anyone-explain-how-stdstringnpos-works
			//cout << "Checking value: " << value << " with position: " << position << endl;
			if (position != string::npos) {
				cout << value << endl;
				found = true;
			}
		}
	}
	if (!found) {
		cout << "Couldnt find: " << city << endl;
	}
}

void HashMap::searchShape(string& shape) {
	bool found = false;
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			string value = hashTable[i]->value;
			size_t position = value.find("Shape: " + shape);
			//https://stackoverflow.com/questions/66575296/can-anyone-explain-how-stdstringnpos-works
			if (position != string::npos) {
				cout << value << endl;
				found = true;
			}
		}
	}
	if (!found) {
		cout << "Couldnt find: " << shape << endl;
	}
}

void HashMap::searchYear(string& year) {
	bool found = false;
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			string value = hashTable[i]->value;
			size_t position = value.find("Year: " + year);
			//https://stackoverflow.com/questions/66575296/can-anyone-explain-how-stdstringnpos-works
			if (position != string::npos) {
				cout << value << endl;
				found = true;
			}
		}
	}
	if (!found) {
		cout << "Couldnt find: " << year << endl;
	}
}

void HashMap::searchState(string& state) {
	bool found = false;
	for (int i = 0; i < size; i++) {
		if (hashTable[i] != nullptr && !hashTable[i]->isDeleted) {
			string value = hashTable[i]->value;
			size_t position = value.find("State: " + state);
			//https://stackoverflow.com/questions/66575296/can-anyone-explain-how-stdstringnpos-works
			if (position != string::npos) {
				cout << value << endl;
				found = true;
			}
		}
	}
	if (!found) {
		cout << "Couldnt find: " << state << endl;
	}
}