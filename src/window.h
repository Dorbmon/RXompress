#include "Type/compress.h"
#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include "gtkmm/menubutton.h"
#include "gtkmm/treestore.h"
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
class DirListModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
    DirListModelColumns() {
        add(dirName);
        add(node);
    }
    Gtk::TreeModelColumn<Glib::ustring> dirName;
    Gtk::TreeModelColumn<std::shared_ptr<FileTreeNode>> node;
};
class RxMainWindow : public Gtk::Window {
private:
    std::unique_ptr<Compress> compress;
    FileListModelColumns fileListModel;
    Glib::RefPtr<Gtk::TreeStore> refFileListModel;
    DirListModelColumns dirListModel;
    Glib::RefPtr<Gtk::TreeStore> refDirListModel;
    std::shared_ptr<ResourceHandler> resourceHandler;
    Gtk::Box* barContentBox;
    void refreshDirList(std::shared_ptr<FileTreeNode> currentNode, Gtk::TreeRow* father);
public:
    // std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler
    RxMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    void Init(std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler);
    void refresh();
    void refreshFileList();
    Gtk::ScrolledWindow* fileListWin;
    Gtk::TreeView* fileList;
    Gtk::ScrolledWindow* dirWin;
    Gtk::TreeView* dirList;
    Gtk::Box* contentBox;
    Gtk::Button* aboutButton;
    void activateDirItem(const Gtk::TreePath &, Gtk::TreeViewColumn *);
};

