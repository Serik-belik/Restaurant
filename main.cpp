// Workshop N25
// RESTAURANT
// Rev. A

#include <iostream>
#include <string.h>

class Dish {
	std::string name = "";
	double _price = 0.;

public:
	// Default constructor
	Dish() {}

	// Parameterized constructor
	Dish(std::string dishName, double dishPrice) : name(dishName), _price(dishPrice) {
		std::cout << "Dish " << dishName << " created at " << this << std::endl;
	}

	// Price getter
	double price() { return _price; }
};


class Order {
	Dish* currentDish = nullptr;
	unsigned int size = 0;

public:
	Dish* arrayOfDishes = nullptr;

	// Parameterized constructor
	Order(const unsigned int sizeOfOrder): size(sizeOfOrder) {
		arrayOfDishes = new Dish[sizeOfOrder];
		currentDish = arrayOfDishes;
		std::cout << "Order created at " << this << std::endl;
	}

	// Destructor
	~Order() {
		delete[] arrayOfDishes;
		arrayOfDishes = nullptr;
		currentDish = nullptr;
	}

	void addDish(Dish& newDish) {
		if (currentDish < &arrayOfDishes[size]) {
			*currentDish = newDish;
			++currentDish;
		}
		else
			std::cout << "Order size exceed" << std::endl;
	}

	double getTotalPrice() {
		if (!arrayOfDishes) return 0;

		double totalPrice = 0.;
		for (Dish* thisDish = arrayOfDishes; thisDish < &arrayOfDishes[size]; ++thisDish)
			totalPrice += thisDish->price();

		return totalPrice;
	}
};


class Table {
	Order* orderPtr = nullptr;
	int numberID = 0;
	bool reserved = false;

public:

	// Parameterized constructor
	Table(unsigned int setId): numberID(setId) {

	}

	// Reserve table method
	bool reserve(Order* newOrder) {
		if (newOrder == nullptr || reserved != false) return false; // Якщо замовлення не зробене OR столик вже зайнятий то не опрацьовужмо столик

		if (orderPtr != nullptr) {
			if (orderPtr->arrayOfDishes) {
				delete[] orderPtr->arrayOfDishes;
			}
			// якщо столик вже використовувався і було замовлення то видаляємо попереднє
			delete orderPtr;
			orderPtr = nullptr;
		}

		orderPtr = newOrder;
		reserved = true;
		return reserved;
	};

	// Close table method
	double close() {
		reserved = false;
		double res = 0.;

		if (orderPtr != nullptr) {
			res = (*orderPtr).getTotalPrice();

			if (orderPtr->arrayOfDishes) {
				delete[] orderPtr->arrayOfDishes;
				orderPtr = nullptr;
			}
		}

		return res;
	};
};

int main() {
	// Create a few dishes
	Dish fish = Dish("fish", 15.50);
	Dish salad = Dish("salad", 7.10);
	Dish pizza = Dish("pizza", 12.90);
	Dish soup = Dish("soup", 10.45);
	Dish chicken = Dish("chicken", 9.99);
	std::cout << std::endl;

	// Create an order
	unsigned int counter = 0;
	std::cout << "How many dishes to order? ";
	std::cin >> counter;

	Order* ord1_ptr = nullptr;
	ord1_ptr = new Order(counter);
	Order& ord1 = *ord1_ptr;
	ord1.addDish(fish);
	ord1.addDish(fish);
	ord1.addDish(soup);
	ord1.addDish(soup);
	std::cout << "Total Order Price " << ord1.getTotalPrice() << std::endl;

	// Create tables
	Table* tables = new Table[3]{1, 2, 3};
	tables[0].reserve(ord1_ptr);
	double totalPrice = tables[0].close();
	std::cout << "Total Price on table[3] = " << totalPrice << std::endl;

	delete[] tables;
	tables = nullptr;

	return 0;
};

