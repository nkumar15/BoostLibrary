#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <ctime>

using namespace std;
using boost::asio::ip::tcp;


void make_client()
{
    try
    {

        boost::asio::io_service io_serv;
        tcp::resolver resolver(io_serv);
        tcp::resolver::query query("localhost","50000");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_serv);
        boost::asio::connect(socket,endpoint_iterator);

        for(;;)
        {
            boost::array<char,128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf),error);

            if ( error = boost::asio::error::eof)
                break;
            else if ( error )
                throw boost::system::system_error(error);

            cout.write(buf.data(),len);
        }
    }
    catch(exception &e )
    {
        cerr << e.what() << endl;
    }
}


std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

void make_server()
{
    try
    {
        boost::asio::io_service io_serv;
        tcp::acceptor acceptor ( io_serv, tcp::endpoint(tcp::v4(),50000) );

        for(;;)
        {
            tcp::socket socket(io_serv);
            cout << "Waiting for request " << endl;
            acceptor.accept(socket);
            cout << "Recieved request " << endl;

            string msg = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket,boost::asio::buffer(msg), ignored_error);

        }
    }
    catch(exception &ex)
    {
        cerr << ex.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    if ( argc == 2 )
        make_client();
    else
        make_server();

    return 0;
}
