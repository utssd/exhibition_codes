# System information
OS: Linux vm-VirtualBox 4.15.0-34-generic #37~16.04.1-Ubuntu  
gcc: gcc version 5.4.0 20160609

# Design Ideas
1. Use hash table (std::unordered_map in C++) (`_buy` and `_sell` in the implementation) to store order information, which gives O(1) time complexity for adding, removing, looking-up and also O(1) for size modifying. 
2. Since prize is not frequentyly modified (unless order is deleted or size is changed to zero) but need to be in sorted form when query comes, prize can be
 	- put into a binary search tree/ red-black Tree (std::map or std::set in C++), which gives O(lg(N)) time for inserting and O(l), l for level, time for looking up. std::map<T>::iterator is a bidirectional iterator, so everytime when we want prize at level l, we need to move forward iterators l times; this additional indexing takes O(l) time.
 	- put into an array (std::vector in C++) and sort the array. This gives us O(1) for looking up since vector supports random access (indexing), but gives O(N) inserting time by using vector::insert().
	 	(Binary search takes O(lg(N)) to locate desired position then O(N-position) for modifying the vector. Overall O(N) ).  
If the order book is heavily used for storage other than looking-up, then BST is better than array; if the order book is heavily used for looking-up than storage, then array is better than BST, since once it is sorted, we can access later for constant time. Here I implemented it using a BST.
3. Use std::map (`_buy_price` and `_sell_price`) as BST for storing price; key is a price and value is aggregated size at that price; everytime new order is added at that price, size will be updated in the map; if order is modified, this map is also updated to reflect current size
4. The unordered_map (`_buy`) and map (`_buy_price`) is related by storing map::iterator in unordered_map, so in case the order is deleted, it takes constant time to delete corresponding prize in the searching tree; also convenient for updating size in `_buy_price`
5. sell orders and buy orders will be stored in two different instances; so do their price
6. Some utility functions, including reading, printing, size checking should be added.

