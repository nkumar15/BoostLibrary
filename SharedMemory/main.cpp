#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

using namespace boost::interprocess;
using namespace std;

int main()
{
    shared_memory_object shdmem{open_or_create, "Boost", read_write};
    shdmem.truncate(1024);
    std::cout << shdmem.get_name() << endl;
    offset_t size;

    if (shdmem.get_size(size))
        std::cout << size << endl;

    mapped_region region { shdmem, read_write };
    cout << std::hex << region.get_address() << endl;
    cout << std::dec << region.get_size() << endl;

    mapped_region region2 { shdmem, read_only};
    cout << std::hex << region2.get_address() << endl;
    cout << std::dec << region2.get_size() << endl;

    int *val = static_cast<int *> ( region.get_address());
    *val = 20;

    int *val2 = static_cast<int *> ( region2.get_address());
    cout << "value " << *val2 << endl;

    bool removed = shared_memory_object::remove("Boost");
    cout <<"Shared memory removed " << std::boolalpha << removed << endl;

}
