#include "Type/compress.h"
#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include <gtkmm.h>
class RxMainWindow : public Gtk::Window
{
private:
    std::unique_ptr<Compress> compress;
    void refreshFileList();
public:
    RxMainWindow(std::string inputFile);
    void refresh();
    Gtk::ScrolledWindow fileListWin;
    Gtk::ListBox fileList;
    Gtk::ScrolledWindow dirWin;
    Gtk::ListBox dirList;
    Gtk::Box contentBox;
};

