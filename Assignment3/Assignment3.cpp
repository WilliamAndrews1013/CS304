#include <iostream>
#include <list>
#include <algorithm>

template<typename T>
void insertionSort(std::list<T>& A) 
{
	for (auto j = std::next(A.begin()); j != A.end(); ++j)
	{
		T key = *j;
		auto i = std::prev(j);

		while (i != A.begin() && *i > key) {
			std::swap(*std::next(i), *i);
			--i;
		}
		if (*i > key) {
			std::swap(*std::next(i), *i);
		}
		else {
			*std::next(i) = key;
		}
	}
}

template<typename T>
typename std::list<T>::iterator partition(std::list<T>& A, typename std::list<T>::iterator p, typename std::list<T>::iterator r)
{
	auto pivot = *r;
	auto i = p;

	for (auto j = p; j != r; ++j) {
		if (*j < pivot) {
			++i;
			std::swap(*i, *j);
		}
	}

	std::swap(*std::next(i), *r);
	return std::next(i);
}

template<typename T>
void quickSort(std::list<T>& A, typename std::list<T>::iterator p, typename std::list<T>::iterator r)
{
	if (std::distance(p, r) > 0) {
		auto pivot = partition(A, p, r);
		quickSort(A, p, std::prev(pivot));
		quickSort(A, std::next(pivot), r);
	}
}

template<typename T>
std::list<T> merge(std::list<T>& leftList, std::list<T>& rightList) 
{
	std::list<T> sortedList;

	auto leftIt = leftList.begin();
	auto rightIt = rightList.begin();

	while (leftIt != leftList.end() && rightIt != rightList.end()) {
		if (*leftIt < *rightIt) {
			sortedList.push_back(*leftIt);
			++leftIt;
		}
		else {
			sortedList.push_back(*rightIt);
			++rightIt;
		}
	}

	sortedList.insert(sortedList.end(), leftIt, leftList.end());
	sortedList.insert(sortedList.end(), rightIt, rightList.end());


	return sortedList;
}

template<typename T>
std::list<T> mergeSort(std::list<T>& list)
{
	if (list.size() <= 1) {
		return list;
	}
	auto middle = list.begin();
	std::advance(middle, list.size() / 2);
	std::list<T> leftList(list.begin(), middle);
	std::list<T> rightList(middle, list.end());

	leftList = mergeSort(leftList);
	rightList = mergeSort(rightList);

	std::list<T> sortedList = merge(leftList, rightList);

	return sortedList;
}

int main() 
{
	std::list<int> myList = { 9,8,7,6,5,4,3,2,1 };
	auto p = myList.begin();
	auto r = myList.end();
	//insertionSort(myList);
	quickSort(myList, p, r);
	//std::list<int> sortedList = mergeSort(myList);
	for (auto elem : myList)
		std::cout << elem << ", ";
	
	return 0;
}
