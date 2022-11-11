#include "window.h"
#include "Type/filetype.h"
#include "aboutWin.h"
#include "gdkmm/pixbuf.h"
#include "gtkmm/box.h"
#include "gtkmm/cellrendererpixbuf.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
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
    this->refFileListModel = Gtk::ListStore::create(this->fileListModel);
    this->fileList->set_model(refFileListModel);
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
    // read files of current dir
    auto files = this->compress->GetFiles();
    for (auto file: files) {
        //std::cout << file->self->fileName << std::endl;
        auto item = *(this->refFileListModel->append());
        item[this->fileListModel.fileName] = file->self->fileName;
        item[this->fileListModel.fileSize] = file->self->size;
        item[this->fileListModel.icon] = this->resourceHandler->folderIcon;
    }
    for(guint i = 0; i < 2; i++) {
        Gtk::TreeView::Column* pColumn = this->fileList->get_column(i);
        assert(pColumn != nullptr);
        pColumn->set_reorderable();
    }
}