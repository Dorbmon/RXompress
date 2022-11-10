#include <gtkmm.h>
#include <iostream>
#include <memory>
#include "gtkmm/builder.h"
#include "resourceHandler.h"
#include "thirdparty/argparse/argparse.hpp"
#include "window.h"
Glib::RefPtr<Gtk::Application> app;
std::string inputFile;
void startWin() {
  auto resourceHandler = std::make_shared<ResourceHandler>();
  auto builder = Gtk::Builder::create_from_file(resourceHandler->mainWindow.string());
  auto win = builder->get_widget_derived<RxMainWindow>(builder, "RXompress");
  win->Init(inputFile, resourceHandler);
  app->add_window(*win);
  win->show();
}
int main(int argc, char* argv[])
{
  argparse::ArgumentParser program("RXompress");
  program.add_argument("file").help("input file to compress").default_value(std::string("test.zip"));
  
  try {
    program.parse_args(argc, argv);
    inputFile = program.get<std::string>("file");
  }
  catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }
  app = Gtk::Application::create("rx.compress");
  app->signal_startup().connect(&startWin);
  return app->run(argc, argv);
}