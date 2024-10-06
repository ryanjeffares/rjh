# rjh

A library of associative containers. Not fit for use yet.

## Why?

`rjh` provides an alternative set of drop-in replacement associative containers for usage in C++20 and above.

The implementations of `std::unordered_map` and `std::unordered_set` usually provided by the standard library have disappointing performance and memory usage due to their implementation with linked lists in open addressing. The unordered collections provided by `rjh` use Robin Hood hashing for faster, more consistent and more cache friendly performance in lookups and modifications.

## TODO

* Ordered `set` and `map`
* Iterators for all collections
* Make APIs match the standard containers
* Benchmark against other libraries
* CMake setup for usage as a library
* CI