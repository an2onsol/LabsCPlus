#include <iostream>
#include <forward_list>

template <typename kty, typename vty, typename Hasher = std::hash<kty>>
class HashTable {
private:

	Hasher hasher;
	size_t batchSize;
	size_t size;
	std::forward_list<std::pair<const kty, vty>>* batches;
public:

	HashTable(size_t size = 1, size_t batchSize = 2) {
		this->batchSize = batchSize;
		this->size = size;

		batches = new std::forward_list<std::pair<const kty, vty>>[size];
		for (int i = 0; i < size; i++) {
			batches[i] = std::forward_list<std::pair<const kty, vty>>();
		}
	}

	HashTable& operator=(const HashTable& other) {
		size = other.size;
		batchSize = other.batchSize;
		batches = new std::forward_list<std::pair<const kty, vty>>[size];
		for (int i = 0; i < size; i++) {
			batches[i] = std::forward_list<std::pair<const kty, vty>>();
		}
		for (int i = 0; i < size; i++) {
			for (auto it : other.batches[i]) {
				insert(it.first, it.second);
			}
		}
		return *this;
	}

	~HashTable() {
		delete[] batches;
	}

	void insert(const kty key, vty value) {
		int count = 0;
		int pos = hasher(key) % size;

		for (auto it : batches[pos]) {
			count++;
		}
		batches[pos].push_front(std::pair<const kty, vty>(key, value));

		if (++count >= batchSize) {
			rehash(size * 2);
		}
	}

	void erase(const kty key) {
		int pos = hasher(key) % size;
		auto prevIt = batches[pos].end();

		for (auto it = batches[pos].begin(); it != batches[pos].end(); it++) {
			if ((*it).first == key) {
				if (prevIt != batches[pos].end()) {
					batches[pos].erase_after(prevIt);
				}
				else {
					batches[pos].pop_front();
				}
				break;
			}
			prevIt = it;
		}
	}

	void rehash(size_t value) {
		if (value == 0) {
			throw std::exception(std::invalid_argument("Cant be zero.\n"));
		}
		int tempSize = size;
		size = value;
		std::forward_list<std::pair<const kty, vty>>* tempBatches = batches;
		batches = new std::forward_list<std::pair<const kty, vty>>[size];
		for (int i = 0; i < value; i++) {
			batches[i] = std::forward_list<std::pair<const kty, vty>>();
		}
		for (int i = 0; i < tempSize; i++) {
			for (auto it : tempBatches[i]) {
				insert(it.first, it.second);
			}
		}

		delete[] tempBatches;
	}

	bool contains(kty key) {
		int pos = hasher(key) % size;
		for (auto it : batches[pos]) {
			if (it.first == key) {
				return true;
			}
		}
		return false;
	}

	void clear() {
		size = 1;
		delete[] batches;
		batches = new std::forward_list<std::pair<const kty, vty>>[size];
		for (int i = 0; i < size; i++) {
			batches[i] = std::forward_list<std::pair<const kty, vty>>();
		}
	}

	vty& operator[](const kty key) {
		int pos = hasher(key) % size;
		for (auto it = batches[pos].begin(); it != batches[pos].end(); it++) {
			if ((*it).first == key) {
				return (*it).second;
			}
		}
		insert(key, vty());
		for (auto it = batches[pos].begin(); it != batches[pos].end(); it++) {
			if ((*it).first == key) {
				return (*it).second;
			}
		}
	}
	vty operator[](const kty key) const {
		int pos = hasher(key) % size;
		for (auto it = batches[pos].begin(); it != batches[pos].end(); it++) {
			if ((*it).first == key) {
				return (*it).second;
			}
		}
		throw std::exception("No such element.\n");
	}
};

int main() {
	HashTable<int, int> hashT;
	hashT.insert(0, 24);
	hashT.insert(1, 72);
	hashT.insert(2, 200);
	if (hashT.contains(1)) {
		for (int i = 0; i < 3; i++) {
			std::cout << hashT[i] << std::endl;
		}
	}
}
