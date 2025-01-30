// Workshop N28
// RESTAURANT
// Rev. B

/*
Questions

1. default constructor
2. malloc

*/

#include <iostream>
#include <string.h>

const bool TriggerCreateDelete{false};

class Dish {
public:
	// Default constructor
	Dish() {
		if (TriggerCreateDelete) std::cout << "Blank Dish created at " << this << std::endl;
	}

	// Parameterized constructor
	Dish(std::string dishName, double dishPrice) : _name(dishName), _price(dishPrice) {
		if (TriggerCreateDelete) std::cout << "Dish " << dishName << " created at " << this << std::endl;
	}
	
	// Copy constructor
	Dish(const Dish& dishToCopy) : _name(dishToCopy._name), _price(dishToCopy._price) {
		if (TriggerCreateDelete) std::cout << "Dish Copy of " << &dishToCopy << " created at " << this << std::endl;
	}

static Dish* addObject(Dish* listOfDishes, Dish* newDish, unsigned int& count) {
	++count;
	Dish* temp = new Dish[count];
	for (unsigned int i{0}; i < count - 1; ++i) {
		temp[i] = Dish(listOfDishes[i]);
	}
	temp[count - 1] = Dish(*newDish);
	delete[] listOfDishes;

	return temp;
	}

	const double price() { return _price; }

	const std::string name() { return _name; }

private:
	std::string _name{ "" };
	double _price{ 0. };
};

class Order {
public:

	Order(): id(0) { if (TriggerCreateDelete) std::cout << "Blank Order created at " << this << std::endl; }

	Order(const unsigned int sizeOfOrder) : size(sizeOfOrder) {
		arrayOfDishes = new Dish[sizeOfOrder];
		currentDish = arrayOfDishes;
		++_count;
		id = _count;
		if (TriggerCreateDelete) std::cout << "Order created at " << this << std::endl;
	}

	Order(const Order& orderToCopy): id(orderToCopy.id), size(orderToCopy.size) {
		arrayOfDishes = new Dish[size];
		currentDish = arrayOfDishes;
		for (int i{ 0 }; i < size; ++i) {
			arrayOfDishes[i] = Dish(orderToCopy.arrayOfDishes[i]);
		}
	}

	// Clear and keep the order
	void recreateDishes(const unsigned int sizeOfOrder) {
		delete[] arrayOfDishes;
		arrayOfDishes = nullptr;
		currentDish = nullptr;

		size = sizeOfOrder;
		arrayOfDishes = new Dish[sizeOfOrder];
		currentDish = arrayOfDishes;
		if (TriggerCreateDelete) std::cout << "Order at " << this << " recreated" << std::endl;
	}

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
		if (!arrayOfDishes) return 0.;

		double totalPrice{ 0. };
		for (Dish* thisDish = arrayOfDishes; thisDish < &arrayOfDishes[size]; ++thisDish)
			totalPrice += thisDish->price();

		return totalPrice;
	}

	const Order& operator= (const Order& other) {
		if (this == &other)
			return other;

		id = other.id;
		size = other.size;

		delete[] arrayOfDishes;
		arrayOfDishes = new Dish[size];
		for (int i{ 0 }; i < size; ++i)
			arrayOfDishes[i] = other.arrayOfDishes[i];

		return other;
	}

	static Order* addObject(Order* listOfOrders, Order* newOrder, unsigned int& count) {
		++count;
		Order* temp = new Order[count];
		for (unsigned int i{ 0 }; i < count - 1; ++i) {
			temp[i] = Order(listOfOrders[i]);
		}
		delete[] listOfOrders;
		temp[count - 1] = Order(*newOrder);
		return temp;
	}

	static void printActiveOrders(Order* listOfOrders, unsigned int totalOrders) {
			std::cout << "=== Active Orders ===" << std::endl;
			for (Order* thisOrder = listOfOrders; thisOrder < &listOfOrders[totalOrders]; ++thisOrder) {
				std::cout << "id " << thisOrder->id << " [ ";
				for (int i{ 0 }; i < thisOrder->size; ++i) {
					std::cout << thisOrder->arrayOfDishes[i].name() << ((i < thisOrder->size - 1) ? ", " : " ");
				}
				std::cout << "]" << std::endl;
			}
	}

	unsigned int id;
	Dish* arrayOfDishes{ nullptr };
	unsigned int size{ 0 };
	Dish* currentDish{ nullptr };

private:
	static unsigned int _count;
};

class Table {
public:

	// Default constructor
	Table() {
		++_count;
		_id = _count;
		if (TriggerCreateDelete) std::cout << "Table id " << _id << " created at " << this << std::endl;
	}

	// Reserve table method
	bool reserve(Order* newOrder) {
		if (newOrder == nullptr || reserved != false) return false;

		if (orderPtr != nullptr) {
			if (orderPtr->arrayOfDishes) {
				delete[] orderPtr->arrayOfDishes;
			}
			
			delete orderPtr;
			orderPtr = nullptr;
		}

		orderPtr = newOrder;
		reserved = true;
		std::cout << "Table id " << _id << " reserved" << std::endl;
		return true;
	};

	// Close table 
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

	static void reportTableStatus(Table* listOfTables) {
		for (Table* thisTable = listOfTables; thisTable < &listOfTables[_count]; ++thisTable) {
			std::cout << "Table id " << thisTable->_id << " reserved: ";
			if (thisTable->reserved)
				std::cout << " order id " << thisTable->orderPtr->id;
			else
				std::cout << (bool)thisTable->reserved;
			std::cout << std::endl;
		}
	}

private:
	Order* orderPtr = nullptr;
	bool reserved = false;

	static unsigned int _count;
	unsigned int _id;
};

void printCommands() {
	std::cout << std::endl << "(1) Create a new dish" << std::endl;
	std::cout << "(2) Print dish menu" << std::endl;
	std::cout << "(3) Create a new order" << std::endl;
	std::cout << "(4) Show active orders" << std::endl;
	//std::cout << "(5) Add table" << std::endl;
	//std::cout << "(6) Delete table" << std::endl;
	std::cout << "(7) Reserve Table" << std::endl;
	std::cout << "(8) Close Table" << std::endl;
	std::cout << std::endl << "(-1) Show commands" << std::endl;
	std::cout << "(0) Exit" << std::endl;
}

Dish* find(std::string &findName, Dish* arrayOfDishes, unsigned int arraySize) {
	for (Dish* dish_i = arrayOfDishes; dish_i < &arrayOfDishes[arraySize]; ++dish_i)
		if ((*dish_i).name() == findName) return dish_i;
	return nullptr;
}

unsigned int Table::_count = 0;
unsigned int Order::_count = 0;

int main() {
	// Array of Dishes in menu
	unsigned int totalDishes{ 5 };
	Dish* listOfDishes = new Dish[5]{
		Dish("fish", 15.50), 
		Dish("salad", 7.10), 
		Dish("pizza", 12.90), 
		Dish("soup", 10.45), 
		Dish("chicken", 9.99)};
	
	// Array of Orders
	unsigned int totalOrders{ 0 };
	Order* listOfOrders{ nullptr };

	// Array of Tables
	Table* listOfTables = new Table[2];

	// Control Interface
	unsigned int mode{ 2 };
	printCommands();
	while (mode) {
		std::cout << std::endl << "Command: ";
		std::cin >> mode;

		switch (mode) {
		case 1: {
			// Create new dish - OK
			std::string newDishName;
			std::cout << std::endl << "Provide new dish name: ";
			std::cin >> newDishName;
			double priceDish;
			std::cout << "Provide new dish price: ";
			std::cin >> priceDish;

			Dish* newDish = new Dish(newDishName, priceDish); 
			listOfDishes = Dish::addObject(listOfDishes, newDish, totalDishes);
			delete newDish;
			break;
		}
		case 2: {
			// Print dish menu - OK
			for (Dish* thisDish = listOfDishes; thisDish < &listOfDishes[totalDishes]; ++thisDish)
				std::cout << thisDish->name() << "\t:\t" << thisDish->price() << std::endl;
			break;
		}
		case 3: {
			// Create an order
			unsigned int dishCount;
			std::cout << "How many dishes to order? ";
			std::cin >> dishCount;
			Order* newOrder = new Order(dishCount);

			std::string dishName;
			for (unsigned int i = 0; i < dishCount; ++i) {
				std::cout << "Dish No" << i + 1 << " Name: ";
				std::cin >> dishName;
				Dish* dish = find(dishName, listOfDishes, totalDishes);
				newOrder->addDish(*dish);
			}

			listOfOrders = Order::addObject(listOfOrders, newOrder, totalOrders);
			break;
		}
		case 4: {
			// Show active orders
			Order::printActiveOrders(listOfOrders, totalOrders);
			break;
		}
		case 5: {
			// Add Table
			//Table* newTable = new Table;
			//addObject(listOfTables, newTable);
			break;
		}
		case 7: {
			// Reserve table
			Table::reportTableStatus(listOfTables);
			int tableId;
			std::cout << "Which table (id) reserve? ";
			std::cin >> tableId;

			Order::printActiveOrders(listOfOrders, totalOrders);
			int orderId;
			std::cout << std::endl << "Which order (id) assign? ";
			std::cin >> orderId;

			Order* reserveOrder = &listOfOrders[orderId];
			listOfTables[tableId - 1].reserve(reserveOrder);

			break;
		}
		case 8: {
			// Close table
			int tableId;
			std::cout << "Which table (id) to close? ";
			std::cin >> tableId;

			listOfTables[tableId - 1].close();

			break;
		}
		case -1: {
			printCommands();
			break;
		}
		default:
			break;
		}
	}

	// Create tables
	//double totalPrice = tables[0].close();
	//std::cout << "Total Price on table[3] = " << totalPrice << std::endl;

	return 0;
};

