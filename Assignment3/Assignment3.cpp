#include <iostream>
#include <list>
#include <algorithm>
#include <chrono>
#include <random>

/*
	The overall results to my timing experiemtns show that Insertion sort was slowest when it had a large reverse sorted list and was fastest when the list was either already sorted or they were duplicate elements.
	For Merge Sort it was fastest when the list was in a random order and then it almost performed the same for any other type of list with a difference of +/- 2ms.
	For Quick Sort it was fastest on duplicate elements and it was slowest on a random orderer list.
*/

template<typename T>
void insertionSort(std::list<T>& A) 
{
	// Iterate from second element to end of list
	for (auto j = std::next(A.begin()); j != A.end(); ++j)
	{
		T key = *j; // Store current element to be inserted into list
		auto i = std::prev(j); // Initialize i to the previous position of j

		// Move elements greater then our key one position ahead
		while (i != A.begin() && *i > key) {
			std::iter_swap(std::next(i), i);
			--i;
		}

		// Put key in correct position
		if (*i > key) {
			std::iter_swap(std::next(i), i);
		}
		else {
			// Assign key to next position
			*std::next(i) = key;
		}
	}
}

template<typename T>
typename std::list<T>::iterator partition(std::list<T>& A, typename std::list<T>::iterator p, typename std::list<T>::iterator r)
{
	// Choose last element as pivot
	T pivot = *r;
	auto i = p;

	// Iterate through the sublist from p to r
	for (auto j = p; j != r; ++j) {
		// Checks if our current element is less then pivot
		if (*j < pivot) {
			// If true, swap with element at position i
			std::iter_swap(i, j);
			++i;
		}
	}

	// Swap pivot with the element at position i
	std::iter_swap(i, r);

	return i;
}

template<typename T>
void quickSort(std::list<T>& A, typename std::list<T>::iterator p, typename std::list<T>::iterator r) 
{
	// Checks that there is more then 1 element in list
	if (std::distance(p,r) > 1) {
		// Get pivot
		auto pivot = partition(A, p, r);
		// Checks to make sure p (or begining on list) is not the same as the pivot
		if (p != pivot)
			// Recursively call quickSort
			quickSort(A, p, std::prev(pivot));
		// Checks to make sure pivot is the same as r (or end of the list)
		if (pivot != r)
			// Recursively call quickSort
			quickSort(A, std::next(pivot), r);
	}
}

template<typename T>
std::list<T> merge(std::list<T>& leftList, std::list<T>& rightList) 
{
	std::list<T> sortedList;

	// Create 2 iterators for the 2 lists that need to be merged
	auto leftIt = leftList.begin();
	auto rightIt = rightList.begin();

	// Checks that iterators aren't at the end of the lists
	while (leftIt != leftList.end() && rightIt != rightList.end()) {
		// Checks which iterator is lower value and pushes it to the sorted list
		if (*leftIt < *rightIt) {
			sortedList.push_back(*leftIt);
			++leftIt;
		}
		else {
			sortedList.push_back(*rightIt);
			++rightIt;
		}
	}

	// Add any missing values
	sortedList.insert(sortedList.end(), leftIt, leftList.end());
	sortedList.insert(sortedList.end(), rightIt, rightList.end());


	return sortedList;
}

template<typename T>
std::list<T> mergeSort(std::list<T>& A)
{
	// Checks if list is already sorted
	if (A.size() <= 1) {
		return A;
	}

	// Get the middle of the list
	auto middle = A.begin();
	std::advance(middle, A.size() / 2);

	// Fill left list with everythign from the begining to the middle value
	std::list<T> leftList(A.begin(), middle);

	// Fill right list with everything from the middle value to the end of list
	std::list<T> rightList(middle, A.end());

	// Sort both lists
	leftList = mergeSort(leftList);
	rightList = mergeSort(rightList);

	// Merge both sorted lists together
	return merge(leftList, rightList);
}

int main() 
{
	std::list<int> reverseSortedList;
	std::list<int> reverseSortedList2;
	std::list<int> randomOrderList;
	std::list<int> randomOrderList2;
	std::list<int> duplicateElems;
	
	std::random_device rd; // Seed for random number generator
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<int> dist(1, 1000); // Distribution from 1 to 1000

	for (int i = 500; i > 0; --i) {
		reverseSortedList.push_back(i);
		reverseSortedList2.push_back(i);
	}

	for (int i = 0; i < 500; ++i) 
		randomOrderList.push_back(dist(gen));

	for (int i = 0; i < 50; ++i)
		randomOrderList2.push_back(dist(gen));

	for (int i = 0; i < 500; i++)
		duplicateElems.push_back(5);

	//------INSERTION SORT----------
	// timing for reverse sorted list
	auto insertionSort_reverseSortedList_start_time = std::chrono::high_resolution_clock::now();
	insertionSort(reverseSortedList);
	auto insertionSort_reverseSortedList_end_time = std::chrono::high_resolution_clock::now();
	auto reverseSorted_insertionSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insertionSort_reverseSortedList_end_time - insertionSort_reverseSortedList_start_time);
	
	// Timing for already sorted list
	auto insertionSort_sortedList_start_time = std::chrono::high_resolution_clock::now();
	insertionSort(reverseSortedList);
	auto insertionSort_sortedList_end_time = std::chrono::high_resolution_clock::now();
	auto sorted_insertionSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(insertionSort_sortedList_end_time - insertionSort_sortedList_start_time);

	// Timing for random order list
	auto insertionSort_randomList_start_time = std::chrono::high_resolution_clock::now();
	insertionSort(randomOrderList);
	auto insertionSort_randomList_end_time = std::chrono::high_resolution_clock::now();
	auto random_insertionSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insertionSort_randomList_end_time - insertionSort_randomList_start_time);

	// Timing for duplicate elements
	auto insertionSort_duplicateElems_start_time = std::chrono::high_resolution_clock::now();
	insertionSort(duplicateElems);
	auto insertionSort_duplicateElems_end_time = std::chrono::high_resolution_clock::now();
	auto duplicateElems_insertionSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(insertionSort_duplicateElems_end_time - insertionSort_duplicateElems_start_time);

	//------MERGE SORT---------
	// Timing for reverse order
	auto mergeSort_reverseSortedList_start_time = std::chrono::high_resolution_clock::now();
	std::list<int> sortedReverseList = mergeSort(reverseSortedList2);
	auto mergeSort_reverseSortedList_end_time = std::chrono::high_resolution_clock::now();
	auto reverseSorted_mergeSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(mergeSort_reverseSortedList_end_time - mergeSort_reverseSortedList_start_time);

	// Timing for sorted order
	auto mergeSort_sortedList_start_time = std::chrono::high_resolution_clock::now();
	std::list<int> sortedList = mergeSort(reverseSortedList);
	auto mergeSort_sortedList_end_time = std::chrono::high_resolution_clock::now();
	auto sorted_mergeSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(mergeSort_sortedList_end_time - mergeSort_sortedList_start_time);

	// Timing for random order
	auto mergeSort_randomList_start_time = std::chrono::high_resolution_clock::now();
	std::list<int> sortedRandomList = mergeSort(randomOrderList2);
	auto mergeSort_randomList_end_time = std::chrono::high_resolution_clock::now();
	auto random_mergeSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(mergeSort_randomList_end_time - mergeSort_randomList_start_time);

	// Timing for duplicate elements
	auto mergeSort_duplicateElems_start_time = std::chrono::high_resolution_clock::now();
	std::list<int> duplicateList = mergeSort(duplicateElems);
	auto mergeSort_duplicateElems_end_time = std::chrono::high_resolution_clock::now();
	auto duplicateElems_mergeSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(mergeSort_duplicateElems_end_time - mergeSort_duplicateElems_start_time);

	//--------QUICK SORT----------
	// Timing for reverse sorted order
	auto quickSort_reverseSortedList_start_time = std::chrono::high_resolution_clock::now();
	quickSort(reverseSortedList2, reverseSortedList2.begin(), std::prev(reverseSortedList2.end()));
	auto quickSort_reverseSortedList_end_time = std::chrono::high_resolution_clock::now();
	auto reverseSorted_quickSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(quickSort_reverseSortedList_end_time - quickSort_reverseSortedList_start_time);

	// Timing for sorted order
	auto quickSort_sortedList_start_time = std::chrono::high_resolution_clock::now();
	quickSort(reverseSortedList, reverseSortedList.begin(), std::prev(reverseSortedList.end()));
	auto quickSort_sortedList_end_time = std::chrono::high_resolution_clock::now();
	auto sorted_quickSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(quickSort_sortedList_end_time - quickSort_sortedList_start_time);

	// Timing for random order
	auto quickSort_randomList_start_time = std::chrono::high_resolution_clock::now();
	quickSort(randomOrderList2, randomOrderList2.begin(), std::prev(randomOrderList2.end()));
	auto quickSort_randomList_end_time = std::chrono::high_resolution_clock::now();
	auto random_quickSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(quickSort_randomList_end_time - quickSort_randomList_start_time);

	// Timing for duplicate order
	auto quickSort_duplicateElemsList_start_time = std::chrono::high_resolution_clock::now();
	quickSort(duplicateElems, duplicateElems.begin(), std::prev(duplicateElems.end()));
	auto quickSort_duplicateElemsList_end_time = std::chrono::high_resolution_clock::now();
	auto duplicateElems_quickSort_duration = std::chrono::duration_cast<std::chrono::milliseconds>(quickSort_duplicateElemsList_end_time - quickSort_duplicateElemsList_start_time);
	


	//--------TIMING PRINTS------------
	std::cout << "-Reverse Sorted Order-" << std::endl;
	std::cout << "Insertion Sort: " << reverseSorted_insertionSort_duration.count() << " ms" << std::endl;
	std::cout << "Merge Sort: " << reverseSorted_mergeSort_duration.count() << " ms" << std::endl;
	std::cout << "Quick Sort: " << reverseSorted_quickSort_duration.count() << " ms" << std::endl;
	std::cout << std::endl;
	std::cout << "-Sorted Order-" << std::endl;
	std::cout << "Insertion Sort: " << sorted_insertionSort_duration.count() << " microseconds" << std::endl;
	std::cout << "Merge Sort: " << sorted_mergeSort_duration.count() << " ms" << std::endl;
	std::cout << "Quick Sort: " << sorted_quickSort_duration.count() << " ms" << std::endl;
	std::cout << std::endl;
	std::cout << "-Random Order-" << std::endl;
	std::cout << "Insertion Sort: " << random_insertionSort_duration.count() << " ms" << std::endl;
	std::cout << "Merge Sort: " << random_mergeSort_duration.count() << " microseconds" << std::endl;
	std::cout << "Quick Sort: " << random_quickSort_duration.count() << " ms" << std::endl;
	std::cout << std::endl;
	std::cout << "-Duplicate Elements-" << std::endl;
	std::cout << "Insertion Sort: " << duplicateElems_insertionSort_duration.count() << " microseconds" << std::endl;
	std::cout << "Merge Sort: " << duplicateElems_mergeSort_duration.count() << " ms" << std::endl;
	std::cout << "Quick Sort: " << duplicateElems_quickSort_duration.count() << " ms" << std::endl;
	
	return 0;
}
