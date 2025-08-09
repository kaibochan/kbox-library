#pragma once

#include <cstdlib>

#ifdef _MEM
namespace kbox {
	class Mem;
}

class kbox::Mem {
public:
	static void initialize();
	static void inspect();
	static void insert(void* ptr, std::size_t size);
	static void remove(void* ptr);

private:
	static unsigned int count;
	static unsigned int reserved;
	static void** addresses;
	static std::size_t* sizes;
};
#endif // _MEM