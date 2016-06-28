#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace std;

void my_print(const boost::system::error_code&)
{
    cout << "I will be a time consuming function which can be handled asynchronosly" << endl ;
}

void my_timer_print( const boost::system::error_code&,
                    boost::asio::deadline_timer* t, int* count )
{

    if( *count < 5)
    {
        cout << *count + 1 << endl;
        ++(*count);

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));

        t->async_wait(boost::bind(my_timer_print,boost::asio::placeholders::error, t, count));
    }
}

class Printer
{
public:
    Printer(boost::asio::io_service &io):timer_(io,boost::posix_time::seconds(1)), count(0)
    {
        timer_.async_wait(boost::bind(&Printer::print, this));
    }
    ~Printer() { cout << "Final count " << count << endl; }
    void print()
    {
        if( count < 5)
        {
            count++;
            cout << count << endl;

            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));

            timer_.async_wait(boost::bind(&Printer::print,this));
        }
    }
private:
    boost::asio::deadline_timer timer_;
    int count ;
};


int main()
{
    boost::asio::io_service io;

    boost::asio::deadline_timer t(io,boost::posix_time::seconds(1));

    /* blocking wait */
    /*
    t.wait();
    std::cout << "Hello World!" << endl;
    */

    /* Unblocking or asynchrnous wait */
    /* async wait only happens when run method is used on same thread*/
    /*
    t.async_wait(&my_print);
    io.run();
    */

    /* Async wait and  binding arguments to handlers */
    /*
    int count = 0;
    t.async_wait(boost::bind(my_timer_print,boost::asio::placeholders::error, &t, &count));
    io.run();
    */

    /* Using member function in timer */
    /* Understand the usage of this  in Printer class */
    /*
    Printer printer(io);
    io.run();
    */

    /*
    The previous four codes avoided the issue of handler synchronisation by calling
    the io_service::run() function from one thread only. As you already know, the asio
    library provides a guarantee that callback handlers will only be called from threads
    that are currently calling io_service::run(). Consequently, calling io_service::run()
    from only one thread ensures that callback handlers cannot run concurrently.
    The single threaded approach is usually the best place to start when developing applications using asio.
    The downside is the limitations it places on programs, particularly servers, including:
    Poor responsiveness when handlers can take a long time to complete.
    An inability to scale on multiprocessor systems.
    */


    return 0;
}
