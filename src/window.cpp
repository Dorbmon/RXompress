#include "window.h"
#include "Type/filetype.h"
#include "gdkmm/pixbuf.h"
#include "gtkmm/cellrendererpixbuf.h"
#include "gtkmm/enums.h"
#include "resourceHandler.h"
#include <cassert>
#include <fmt/core.h>
#include <memory>

RxMainWindow::RxMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) 
    :Gtk::Window(cobject){
    
    this->set_child(this->contentBox);
    this->set_child(this->barContentBox);
    this->barContentBox.set_orientation(Gtk::Orientation::VERTICAL);
    this->barContentBox.append(this->bar);
    this->barContentBox.append(this->contentBox);
    this->contentBox.set_orientation(Gtk::Orientation::HORIZONTAL);
    this->contentBox.append(this->dirWin);
    this->contentBox.append(this->fileListWin);
    //this->dirWin.set_child(this->dirList);
    this->fileListWin.set_child(this->fileList);
    
    assert(this->compress != nullptr);
    this->refFileListModel = Gtk::ListStore::create(this->fileListModel);
    this->fileList.set_model(refFileListModel);
    Gtk::CellRendererPixbuf* cell = Gtk::manage(new Gtk::CellRendererPixbuf);
    int cols_count = this->fileList.append_column("Type", *cell);
    Gtk::TreeViewColumn* pColumn = this->fileList.get_column(cols_count - 1);
    if(pColumn) {
        pColumn->add_attribute(cell->property_gicon(), this->fileListModel.icon);
    }
    this->fileList.append_column("Name", this->fileListModel.fileName);
    this->fileList.append_column("Size", this->fileListModel.fileSize);
    this->fileList.set_expand(true);
    
}
void RxMainWindow::Init(std::string inputFile, std::shared_ptr<ResourceHandler> resourceHandler) {
    this->resourceHandler = resourceHandler;
    this->set_title(fmt::format("{}-RX Compress", inputFile));
    this->compress = FileType::MakeCompress(inputFile);
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
        Gtk::TreeView::Column* pColumn = this->fileList.get_column(i);
        assert(pColumn != nullptr);
        pColumn->set_reorderable();
    }
}