#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

class OrderBook {
private:
	std::unordered_map<int, std::pair<int, std::map<double, int>::iterator>> _buy; 
	std::unordered_map<int, std::pair<int, std::map<double, int>::iterator>> _sell; 
	std::map<double, int> _buy_price;
	std::map<double, int> _sell_price;
public:
	void add(int order_id, char side, double price, int size);
	void modify(int order_id, int new_size);
	void remove(int order_id);
	double get_price(char side, int level);
	int get_size(char side, int level);
	/**
	 * default constructor
	 */
	OrderBook() : _buy(), _sell(), _buy_price(), _sell_price() { }
	/**
	 * get total number of orders in the book
	 */	
	int get_book_size() const {
		return _buy.size() + _sell.size();
	}
};

void OrderBook::add(int order_id, char side, double price, int size) {
	if (size < 0) {
		std::cout << "negative order size!" << std::endl;
		return;
	}
	// decide side first
	if (side == 'B') {
		auto search = _buy_price.find(price);
		// if price already exist, means other orders with same price happend earlier,
		// then just insert into order_id table with price iterator 
		// and update size in that price
		// otherwise insert new price
		if (search == _buy_price.end()) {
			search = _buy_price.insert(std::pair<double, int>(price, size)).first;
		} else {
			_buy_price[price] += size;
		}
		_buy[order_id] = std::make_pair(size, search);
	} else if (side == 'S') {
		auto search = _sell_price.find(price);
		if (search == _sell_price.end()) {
			search = _sell_price.insert(std::pair<double, int>(price, size)).first;
		} else {
			_sell_price[price] += size;
		}
		_sell[order_id] = std::make_pair(size, search);
	}
	return;
}

void OrderBook::modify(int order_id, int new_size) {
	// decide which side
	if (_buy.count(order_id) != 0) {
		int old_size = _buy[order_id].first;
		// update order size:
		_buy[order_id].first = new_size;
		// update corresponding size at that price
		_buy[order_id].second->second += new_size - old_size;
		// if after update the size at that price becomse zero, remove the price entry
		if (_buy[order_id].second->second == 0)
			_buy_price.erase(_buy[order_id].second);
	} else if (_sell.count(order_id) != 0) {
		int old_size = _sell[order_id].first;
		_sell[order_id].first = new_size;
		_sell[order_id].second->second += new_size - old_size;
		if (_sell[order_id].second->second == 0)
			_sell_price.erase(_sell[order_id].second);
	} else {
		std::cout << "order id not found!" << std::endl;
	}
}

void OrderBook::remove(int order_id) {
	// decide side
	if (_buy.count(order_id) != 0) {
		// get price of order and update size at this price
		auto it_price = _buy[order_id].second;
		int price_size = _buy[order_id].first;
		it_price->second -= price_size;
		// if size goes into zero, delete price record
		if (it_price->second == 0)
			_buy_price.erase(it_price);
		// delete order_id
		_buy.erase(order_id);
	} else if (_sell.count(order_id) != 0) {
		auto it_price = _sell[order_id].second;
		int price_size = _sell[order_id].first;
		it_price->second -= price_size;
		if (it_price->second == 0)
			_buy_price.erase(it_price);
		_sell.erase(order_id);
	} else {
		std::cout << "order id not found!";
	}

}

double OrderBook::get_price(char side, int level) {
	// if level is deeper than number of price, return -1
	if (side == 'B') {
		if (level > _buy_price.size())
			return -1.0;
		auto it = _buy_price.begin();
		for (int i = 0; i < level; ++i) ++it;
		return it->first;
	} else if (side == 'S') {
		if (level > _sell_price.size())
			return -1.0;
		auto it = _sell_price.begin();
		for (int i = 0; i < level; ++i) ++it;
		return it->first;
	}
}

int OrderBook::get_size(char side, int level) {
	// if level is deeper than number of price, return -1
	if (side == 'B') {
		if (level > _buy_price.size())
			return -1.0;
		auto it = _buy_price.begin();
		for (int i = 0; i < level; ++i) ++it;
		return it->second;
	} else if (side == 'S') {
		if (level > _sell_price.size())
			return -1.0;
		auto it = _sell_price.begin();
		for (int i = 0; i < level; ++i) ++it;
		return it->second;
	}

}
