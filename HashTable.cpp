#include <iostream>
#include <string>
using namespace std;

#define TABLE_SIZE 1000003

unsigned int hash_func(const char* key, const unsigned int table_size)
{
	unsigned int h = 0;
	unsigned int o = 31415;
	const unsigned int t = 27183;
	while (*key)
	{
		h = (o * h + *key++) % table_size;
		o = o * t % (table_size - 1);
	}
	return h;
}

class HashtableItem;

class Hashtable
{
private:
	HashtableItem** table;
	HashtableItem* currentTableItem;
	int currentIndex;

public:
	Hashtable();
	~Hashtable();

	bool Add(const string& key);

	int Search(const string& key);

	void Remove(const string& key);

	void Clear();
};

class HashtableItem
{
private:
	HashtableItem* pnext;
	string key, value;
	int repeat = 0;

	HashtableItem(const string& key);
	~HashtableItem();

public:
	const string& Key() const;
	const string& Value() const;

	friend int Hashtable::Search(const string& key);
	friend bool Hashtable::Add(const string& key);
	friend void Hashtable::Remove(const string& key);
	friend void Hashtable::Clear();
	friend Hashtable::~Hashtable();
};

Hashtable::Hashtable() : currentTableItem(nullptr), currentIndex(0)
{
	table = new HashtableItem * [TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; table[i++] = nullptr);
}
Hashtable::~Hashtable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i])
			delete table[i];
	}
	delete[]table;
}
bool Hashtable::Add(const string& key)
{
	unsigned int i = hash_func(key.c_str(), TABLE_SIZE);
	if (table[i])
	{
		HashtableItem* node;
		for (node = table[i]; node->pnext && (node->pnext->Key() != key); node = node->pnext);
		if (node->pnext)
		{
			node->repeat++;
			return false;
		}
		node->pnext = new HashtableItem(key);
		return true;
	}
	table[i] = new HashtableItem(key);
	table[i]->repeat++;
	return true;
}

int Hashtable::Search(const string& key)
{
	unsigned int i = hash_func(key.c_str(), TABLE_SIZE);
	if (table[i])
	{
		HashtableItem* node;
		for (node = table[i]; node->pnext && (node->pnext->Key() != key); node = node->pnext);
		if (node->pnext)
		{
			return node->repeat;
		}
		return node->repeat;
	}
}

void Hashtable::Remove(const string& key)
{
	unsigned int i = hash_func(key.c_str(), TABLE_SIZE);
	if (table[i])
	{
		HashtableItem* node;
		if (table[i]->Key() == key)
		{
			table[i]->repeat = 0;
		}
		else
		{
			for (node = table[i]; node->pnext && (node->pnext->Key() != key); node = node->pnext);
			if (node->pnext) 
			{
				table[i]->repeat = 0;
			}
		}
	}
}
void Hashtable::Clear()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		delete table[i];
		table[i] = nullptr;
	}
}

HashtableItem::~HashtableItem()
{
	if (this->pnext)
		delete this->pnext;
}
const string& HashtableItem::Key() const
{
	return this->key;
}
const string& HashtableItem::Value() const
{
	return this->value;
}

int main(int argc, char* argv[])
{
	string key, value;
	Hashtable ht;
	HashtableItem* item;
	string command, text;
	bool q = true;

	while (q == true)
	{
		cin >> command;
		if (command[0] == 'q' && command[1] == NULL)
		{
			q = false;
		}
		else if (command[0] == '?')
		{
			cout << ht.Search(command.substr(1)) << endl;
		}
		else if (command[0] == '-')
		{
			ht.Remove(command.substr(1));
		}
		else
		{
			text += command + " ";
			ht.Add(command);
		}
	}
	return 0;
}