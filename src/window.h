#include "Type/compress.h"
#include "gtkmm/box.h"
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
        add(meta);
        add(node);
    }

    Gtk::TreeModelColumn<Glib::ustring> fileName;
    Gtk::TreeModelColumn<size_t> fileSize;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> icon;
    Gtk::TreeModelColumn<void*> meta;
    Gtk::TreeModelColumn<std::shared_ptr<FileTreeNode>> node;
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
    Gtk::Box* wholeContentBox;
    void refreshDirList(std::shared_ptr<FileTreeNode> currentNode, Gtk::TreeRow* father);
public:
    // std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler
    RxMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    void Init(std::string inputFile,  std::shared_ptr<ResourceHandler> resourceHandler, Glib::RefPtr<Gtk::Application> app);
    void refresh();
    void refreshFileList();
    void initFileList();
    Gtk::ScrolledWindow* fileListWin;
    Gtk::TreeView* fileList;
    Gtk::ScrolledWindow* dirWin;
    Gtk::TreeView* dirList;
    Gtk::Box* contentBox;
    void activateDirItem(const Gtk::TreePath &, Gtk::TreeViewColumn *);
    void onEditFileName(const Glib::ustring &, const Glib::ustring &);
    void saveFile();
private:
    Gtk::PopoverMenu fileListMenuPopup;
    void fileListRename();
    void fileListRemove();
    bool fileListOnDropFile(const Glib::ValueBase &, double, double);
    std::pair<bool, std::string>  fileListInsertFileOrDir(const std::string& path);
};

