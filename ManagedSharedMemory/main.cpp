#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>


using namespace std;
using namespace boost::interprocess;

int main()
{
    try
    {
        shared_memory_object::remove("Boost");
        managed_shared_memory shm { open_or_create, "Boost", 1024 };
        int  *i = shm.construct<int>("Number")(10);
        cout << std::dec << *i << endl;

        /* address and number of element */
        std::pair<int *,std::size_t> p = shm.find<int>("Number");

        if ( p.first )
        {
            cout << *p.first << endl;
            cout << p.second << endl;
        }

        /* destroy element from memory */
        shm.destroy<int>("Number");
        p = shm.find<int>("Number");
        if ( p.first )
        {
            cout << *p.first << endl;
            cout << p.second << endl;
        }

        /* Use of container class and string class in managed shared memory */

        typedef boost::interprocess::allocator<char,managed_shared_memory::segment_manager> CharAllocator;

        typedef boost::interprocess::basic_string<char,std::char_traits<char>, CharAllocator> string;

        string *s = shm.find_or_construct<string>("mystring")("Hello managed shared memory!",shm.get_segment_manager());
        cout << *s << endl ;
        s->insert(5, " World");
        cout << *s << endl ;

    }
    catch(boost::interprocess::bad_alloc &ex)
    {
        std::cerr << ex.what() << endl;
    }

    return 0;
}
