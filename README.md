In this README file, I want to talk about the task that needed to be completed, my solution is in the vector.h files. In the file main.cpp there are tests for this task

Implementation Details

You need to implement a template Vector class with a single template parameter - the type of stored elements.
When implementing it, you can (and even need to) use generalized algorithms from STL (std::copy, std::fill, etc.), but you cannot use standard containers.
The correctness of the multiplicative array expansion scheme with a factor of 2 will be checked. In the basic version, manual control of the lifetime of objects is not required (see the additional task).
The class must support the following functionality:

The default constructor creates an empty array;
Explicit constructor from a number - creates an array of a given size filled with objects constructed by default;
A constructor that accepts size and value (in this order) - creates an array of length size filled with elements with value;
A template constructor that accepts a pair of iterators creates a copy of the passed range;

Important:
the declaration of this constructor should look like
template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
Vector(Iterator first, Iterator last)

This makes the constructor available only when two Forward iterators come to the input.

Constructor from std::initializer_list;
The rule of "five";
Methods Size, Capacity, Empty;
Constant and non-constant index access operator []. The nonconstant one should allow you to change the received element;
Constant and non-constant index access method At. When going beyond the bounds of the array, it should throw an exception std::out_of_range;
Methods Front() and Back()
access to the first and last elements (also in two versions).
The Data() method
returns a pointer to the beginning of the array.
Method Swap(other)
exchanges the contents with another array other;
The Resize(new_size) method
changes the size to new_size. If the capacity does not allow storing so many items, then a new buffer with a capacity of new_size is allocated. Missing elements are constructed by default.
The Resize(new_size, value) method
the same as Resize(new_size), but in the case of new_size > size, fills in the missing elements with the value value.
The Reserve(new_cap) method
changes the capacity to max(new_cap, current capacity). The size does not change.
Method ShrinkToFit()
reduces capacity to size.
The Clear() method
sets the size to 0, and the allocated memory is NOT cleared.
PushBack Methods(const T&) and PushBack(T&&)
adds a new element to the end of the array.
The PopBack() method
deletes the last element.
Comparison operations (<, >, <=, >=, ==, !=), setting the lexicographic order.

Also implement support for iterators and methods for working with them: begin(), end(), cbegin(), cend(), rbegin(), rend(), crbegin(), crend(). begin()-end(), rbegin()-rend() must have two versions that return constant and non-constant iterators.
In order for codestyle not to swear at writing methods with a small letter, write a comment in this line // NOLINT
Inside the classroom Vector define the member types ValueType, Pointer, ConstPointer, Reference, ConstReference, SizeType, Iterator, ConstIterator.
Security guarantees Design the class so that when exceptions occur (lack of memory, error copying objects, etc.), the methods satisfy a strict security guarantee.
That is, all methods must work atomically: either work without failures, or do not change the initial state of the vector.
Exception: copying assignment can only provide a basic security guarantee.
When solving, it can be assumed that move constructors, move assignments, and object destructors never throw exceptions!
