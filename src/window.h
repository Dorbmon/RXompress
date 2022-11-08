#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include <gtkmm.h>
class RxMainWindow : public Gtk::Window
{
public:
  RxMainWindow(std::string inputFile);

  Gtk::ScrolledWindow fileListWin;
  Gtk::ListBox fileList;
  Gtk::ScrolledWindow dirWin;
  Gtk::ListBox dirList;
  Gtk::Box contentBox;
};

