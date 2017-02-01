/* compile 
g++ program_options.cpp -lboost_program_options
*/
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char *argv[]) {

  opt::options_description desc("All options");
  desc.add_options()
    ("apples,a",  opt::value<int>()->default_value(10), "apples that you have")
    ("oranges,o", opt::value<int>()->default_value(5) , "oranges that you have")
    ("name",      opt::value<std::string>()           , "your name")
    ("help"                                           , "produce help message");

  opt::variables_map vm;
  opt::store(opt::parse_command_line(argc,argv,desc),vm);

  opt::notify(vm);
  
  if(vm.count("help"))  {
    std::cout << desc << std::endl;
    return 1;
  }

  if(vm.count("name"))  {
    std::cout << "Hi, " << vm["name"].as<std::string>() << std::endl;
  }

  std::cout << "Fruits count: " << vm["apples"].as<int>() + vm["oranges"].as<int>()
            << std::endl;

  return 0;
}
  
