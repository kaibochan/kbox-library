#include "Memory.h"

#ifdef _MEM
#include <iostream>

unsigned int kbox::Mem::count;
unsigned int kbox::Mem::reserved;
void** kbox::Mem::addresses;
std::size_t* kbox::Mem::sizes;

using namespace kbox;

void kbox::Mem::initialize() {
	count = 0;
	reserved = 1;
	addresses = (void**)std::malloc(reserved * sizeof(void*));
	sizes = (std::size_t*)std::malloc(reserved * sizeof(std::size_t));
}

void kbox::Mem::inspect() {
	std::cout << "MEM {" << std::endl;
	for (unsigned int i = 0; i < count; i++) {
		std::cout << "\t[" << addresses[i] << "] : " << sizes[i] << std::endl;
	}
	std::cout << "} MEM END" << std::endl;
}

void kbox::Mem::insert(void* ptr, std::size_t size) {
	if (count + 1 > reserved) {
		reserved *= 2;
		void** new_addresses = (void**)std::malloc(reserved * sizeof(void*));
		std::size_t* new_sizes = (std::size_t*)std::malloc(reserved * sizeof(std::size_t));
		free(addresses);
		free(sizes);
		addresses = new_addresses;
		sizes = new_sizes;
	}
	addresses[count] = ptr;
	sizes[count] = size;

	std::cout << "Allocation [" << addresses[count] << "] : " << sizes[count] << " bytes" << std::endl;
	count++;
}

void kbox::Mem::remove(void* ptr) {
	for (unsigned int i = 0; i < count; i++) {
		if (addresses[i] == ptr) {
			for (unsigned int j = i + 1; j < count; j++) {
				addresses[j - 1] = addresses[j];
				sizes[j - 1] = sizes[j];
			}

			std::cout << "Deallocation [" << addresses[i] << "] : " << sizes[i] << " bytes" << std::endl;
			count--;

			break;
		}
	}
}

void* operator new(std::size_t size) {
	void* ptr = std::malloc(size);

	if (ptr) {
		Mem::insert(ptr, size);
		return ptr;
	}

	throw std::bad_alloc();
}

void operator delete(void* ptr) noexcept {
	Mem::remove(ptr);
	std::free(ptr);
}
#endif // _MEM