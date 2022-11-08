#include <gtkmm.h>
#include <iostream>
#include "argparse/argparse.hpp"
#include "window.h"

int main(int argc, char* argv[])
{
  argparse::ArgumentParser program("RXompress");
  program.add_argument("file").help("input file to compress").default_value(std::string("test.zip"));
  std::string inputFile;
  try {
    program.parse_args(argc, argv);
    inputFile = program.get<std::string>("file");
  }
  catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }
 
  auto app = Gtk::Application::create("rx.compress");

  return app->make_window_and_run<RxMainWindow>(argc, argv, inputFile);
}