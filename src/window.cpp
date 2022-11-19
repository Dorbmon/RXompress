#include "window.h"
#include "Type/compress.h"
#include "Type/filetype.h"
#include "aboutWin.h"
#include "gdkmm/pixbuf.h"
#include "gtkmm/box.h"
#include "gtkmm/cellrendererpixbuf.h"
#include "gtkmm/treeiter.h"
#include "gtkmm/treeview.h"
#include "resourceHandler.h"
#include <cassert>
#include <fmt/core.h>
#include <iostream>
#include <memory>

RxMainWindow::RxMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) 
    :Gtk::Window(cobject){
    this->wholeContentBox = builder->get_widget<Gtk::Box>("WholeContent");
    this->contentBox = builder->get_widget<Gtk::Box>("ContentBox");
    this->set_default_size(800, 500);
    //auto tmp = Gtk::Button();
    //tmp.set_label("Hello");
    //this->barContentBox->append(tmp);
    this->fileList = builder->get_widget<Gtk::TreeView>("FileList");
    this->refFileListModel = Gtk::TreeStore::create(this->fileListModel);
    this->fileList->set_model(this->refFileListModel);
    Gtk::CellRendererPixbuf* cell = Gtk::manage(new Gtk::CellRendererPixbuf);
    int cols_count = this->fileList->append_column("Type", *cell);
    Gtk::TreeViewColumn* pColumn = this->fileList->get_column(cols_count - 1);
    if(pColumn) {
        pColumn->add_attribute(cell->property_gicon(), this->fileListModel.icon);
    }
    this->fileList->append_column_editable("Name", this->fileListModel.fileName);
    this->fileList->append_column("Size", this->fileListModel.fileSize);
    Gtk::CellRenderer *renderer = this->fileList->get_column_cell_renderer(1);
    Gtk::CellRendererText *text_renderer = dynamic_cast<Gtk::CellRendererText *>(renderer);
    if (text_renderer) {
      text_renderer->signal_edited().connect(sigc::mem_fun(*this, &RxMainWindow::onEditFileName) );
    }

    this->dirList = builder->get_widget<Gtk::TreeView>("DirList");
    this->refDirListModel = Gtk::TreeStore::create(this->dirListModel);
    this->dirList->set_model(this->refDirListModel);
    this->dirList->append_column("Name", this->dirListModel.dirName);
    this->dirList->signal_row_activated().connect(sigc::mem_fun(*this, &RxMainWindow::activateDirItem));

    for(guint i = 0; i < this->fileList->get_n_columns(); i++) {
        Gtk::TreeView::Column* pColumn = this->fileList->get_column(i);
        assert(pColumn != nullptr);
        pColumn->set_resizable(true);
    }
    for(guint i = 0; i < this->dirList->get_n_columns(); i++) {
        Gtk::TreeView::Column* pColumn = this->dirList->get_column(i);
        assert(pColumn != nullptr);
        pColumn->set_resizable(true);
    }
}
void RxMainWindow::activateDirItem(const Gtk::TreePath & path, Gtk::TreeViewColumn * column) {
    auto iter = this->refDirListModel->get_iter(path);
    if (iter) {
        this->compress->fileTree->currentNode = (*iter)[this->dirListModel.node];
        this->refreshFileList();
    }
}
void RxMainWindow::refreshDirList(std::shared_ptr<FileTreeNode> currentNode, Gtk::TreeRow* father) {
    if (currentNode == nullptr) {
        currentNode = this->compress->fileTree->root;
        this->refDirListModel->clear();
    }
    if (currentNode->self->isDir) {
        Gtk::TreeRow row;
        Gtk::TreeModel::iterator iter;
        if (father != nullptr) {
            iter = (this->refDirListModel->append(father->children()));
        } else {
            iter = (this->refDirListModel->append());
        }
        currentNode->dirListColumn = iter;
        row = *iter;
        row [this->dirListModel.dirName] = currentNode->self->fileName;
        row [this->dirListModel.node] = currentNode;
        for (auto child: currentNode->map) {
            if (child.second->self->isDir) {
                refreshDirList(child.second, &row);
            }
        }
    }
}

void RxMainWindow::Init(std::string inputFile, std::shared_ptr<ResourceHandler> resourceHandler, Glib::RefPtr<Gtk::Application> app) {
    static Glib::ustring ui_info =
  "<interface>"
  "  <menu id='menubar'>"
  "    <submenu>"
  "      <attribute name='label' translatable='yes'>_File</attribute>"
  "      <section>"
  "        <item>"
  "          <attribute name='label' translatable='yes'>_New</attribute>"
  "          <attribute name='action'>app.new</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
  "        </item>"
  "        <item>"
  "          <attribute name='label' translatable='yes'>_Save</attribute>"
  "          <attribute name='action'>app.save</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;s</attribute>"
  "        </item>"
  "      </section>"
  "      <section>"
  "        <item>"
  "          <attribute name='label' translatable='yes'>_Quit</attribute>"
  "          <attribute name='action'>example.quit</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
  "        </item>"
  "      </section>"
  "    </submenu>"
  "    <submenu>"
  "      <attribute name='label' translatable='yes'>_About</attribute>"
  "      <item>"
  "        <attribute name='label' translatable='yes'>_About RXompress</attribute>"
  "        <attribute name='action'>app.about</attribute>"
  "      </item>"
  "    </submenu>"
  "  </menu>"
  "</interface>";
    this->resourceHandler = resourceHandler;
    this->set_title(fmt::format("{}-RX Compress", inputFile));
    this->compress = FileType::MakeCompress(inputFile);
    assert(this->compress != nullptr);
    this->refresh();
    auto m_refBuilder = Gtk::Builder::create();
    
    m_refBuilder->add_from_string(ui_info);
    app->set_accel_for_action("app.save", "<Primary>s");
    auto gmenu = m_refBuilder->get_object<Gio::Menu>("menubar");
    auto pMenuBar = Gtk::make_managed<Gtk::PopoverMenuBar>(gmenu);
    auto actionGroup = Gio::SimpleActionGroup::create();

    actionGroup->add_action("about", [] {
        AboutWin::show();
    });
    actionGroup->add_action("save", sigc::mem_fun(*this, &RxMainWindow::saveFile));
    insert_action_group("app", actionGroup);
    //m_Box.append(*pMenuBar);
    this->wholeContentBox->insert_child_at_start(*pMenuBar);
}
void RxMainWindow::refresh() {
    this->refreshFileList();
    this->refreshDirList(nullptr, nullptr);
    this->refreshFileList();
    // read files of current dir
}
void RxMainWindow::refreshFileList() {
    this->refFileListModel->clear();
    auto files = this->compress->GetFiles();
    for (auto file: files) {
        //std::cout << file->self->fileName << std::endl;
        auto iter = this->refFileListModel->append();
        auto item = *(iter);
        item[this->fileListModel.fileName] = file->self->fileName;
        item[this->fileListModel.fileSize] = file->self->size;
        if (file->self->isDir) {
            item[this->fileListModel.icon] = this->resourceHandler->folderIcon;
        }
        file->fileListColumn = iter;
        item[this->fileListModel.meta] = file->self->metaData;
        item[this->fileListModel.node] = file;
    }
}
void RxMainWindow::onEditFileName(const Glib::ustring & path, const Glib::ustring & value) {
    auto iter = this->refFileListModel->get_iter(path);
    if (iter) {
        auto column = *iter;
        auto meta = column[this->fileListModel.meta];
        std::string stdName = value;
        std::shared_ptr<FileTreeNode> node = column[this->fileListModel.node];
        if (this->compress->ChangeName(meta, stdName)) {
            // If success then change the name in the file tree
           node->self->fileName = value;
        }
        if (node->dirListColumn) {
            (*node->dirListColumn)[this->dirListModel.dirName] = node->self->fileName;
        }
        if (node->fileListColumn) {
            (*node->fileListColumn)[this->fileListModel.fileName] = node->self->fileName;
        }
    }
}

void RxMainWindow::saveFile() {
    if (this->compress != nullptr) {
        auto res = this->compress->Save();
        if (!res.first) {
            std::cerr << res.second << std::endl;
        }
    }
}