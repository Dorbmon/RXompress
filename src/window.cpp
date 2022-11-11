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
    this->contentBox = builder->get_widget<Gtk::Box>("ContentBox");
    this->set_default_size(800, 500);
    this->barContentBox = builder->get_widget<Gtk::Box>("BarBox");
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
    this->fileList->append_column("Name", this->fileListModel.fileName);
    this->fileList->append_column("Size", this->fileListModel.fileSize);
    aboutButton = builder->get_widget<Gtk::Button>("AboutButton");
    aboutButton->signal_clicked().connect([]() {
        AboutWin::show();
    });

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

}
void RxMainWindow::refreshDirList(std::shared_ptr<FileTreeNode> currentNode, Gtk::TreeRow* father) {
    if (currentNode == nullptr) {
        currentNode = this->compress->fileTree->root;
        //this->refDirListModel->clear();
        std::cout << "clear";
    }
    if (currentNode->self->isDir) {
        std::cout << currentNode->self->fileName << std::endl;
        Gtk::TreeRow row;
        if (father != nullptr) {
            row = *(this->refDirListModel->append(father->children()));
        } else {
            row = *(this->refDirListModel->append());
        }
        row [this->dirListModel.dirName] = currentNode->self->fileName;
        for (auto child: currentNode->map) {
            if (child.second->self->isDir) {
                refreshDirList(child.second, &row);
            }
        }
    }
}
void RxMainWindow::Init(std::string inputFile, std::shared_ptr<ResourceHandler> resourceHandler) {
    this->resourceHandler = resourceHandler;
    std::cout << "num:" << this->barContentBox->get_first_child()->get_name() << std::endl;
    this->set_title(fmt::format("{}-RX Compress", inputFile));
    this->compress = FileType::MakeCompress(inputFile);
    assert(this->compress != nullptr);
    this->refresh();
}
void RxMainWindow::refreshFileList() {
    this->refFileListModel->clear();
}
void RxMainWindow::refresh() {
    this->refreshFileList();
    this->refreshDirList(nullptr, nullptr);
    // read files of current dir
    auto files = this->compress->GetFiles();
    for (auto file: files) {
        //std::cout << file->self->fileName << std::endl;
        auto item = *(this->refFileListModel->append());
        item[this->fileListModel.fileName] = file->self->fileName;
        item[this->fileListModel.fileSize] = file->self->size;
        item[this->fileListModel.icon] = this->resourceHandler->folderIcon;
    }
}