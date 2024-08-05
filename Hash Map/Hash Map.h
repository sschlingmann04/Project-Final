#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class HashMap {
private:
	struct HashNode {
		string key;
		string value;
		bool isDeleted;

		HashNode(const string& k, const string& v)
			: key(k), value(v), isDeleted(false) {}
	};
	vector<HashNode*> hashTable;

public:
	int size;
	int currentSize;
	int hashFunction(const string& key);
	int nextSpace(int index, int position);
	void resize();
	HashMap(int size = 100);
	~HashMap();
	bool insert(const string& key, const string& value);
	bool remove(const string& key);
	void print();
	int getSize();
	void searchCity(string& city);
	void searchShape(string& shape);
	void searchYear(string& year);
	void searchState(string& state);
};


