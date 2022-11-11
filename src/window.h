#include "Type/compress.h"
#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include "gtkmm/menubutton.h"
#include "gtkmm/treeview.h"
#include "resourceHandler.h"
#include <gtkmm.h>
#include <memory>
class FileListModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
    FileListModelColumns() {
        add(fileName);
        add(fileSize);
        add(icon);
    }

    Gtk::TreeModelColumn<Glib::ustring> fileName;
    Gtk::TreeModelColumn<size_t> fileSize;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> icon;
};

class RxMainWindow : public Gtk::Window {
private:
    std::unique_ptr<Compress> compress;
    void refreshFileList();
    FileListModelColumns fileListModel;
    Glib::RefPtr<Gtk::ListStore> refFileListModel;
    std::shared_ptr<ResourceHandler> resourceHandler;
    Gtk::Box* barContentBox;
public:
    // std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler
    RxMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    void Init(std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler);
    void refresh();
    Gtk::ScrolledWindow* fileListWin;
    Gtk::TreeView* fileList;
    Gtk::ScrolledWindow* dirWin;
    Gtk::ListBox dirList;
    Gtk::Box* contentBox;
    Gtk::Button* aboutButton;
};

