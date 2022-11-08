#include "window.h"
#include "Type/filetype.h"
#include "gtkmm/enums.h"
#include<cassert>
RxMainWindow::RxMainWindow(std::string inputFile){
  set_title("RX Compress");
  //set_default_size(200, 200);
  this->set_child(this->contentBox);
  this->contentBox.set_orientation(Gtk::Orientation::HORIZONTAL);
  this->contentBox.append(this->dirWin);
  this->contentBox.append(this->fileListWin);
  this->dirWin.set_child(this->dirList);
  this->fileListWin.set_child(this->fileList);
  this->compress = FileType::MakeCompress(inputFile);
  assert(this->compress != nullptr);
}
void RxMainWindow::refreshFileList() {
    auto child = this->fileList.get_first_child();
    while (child != nullptr) {
        this->fileList.remove(*child);
        child = this->fileList.get_first_child();
    }
}
void RxMainWindow::refresh() {
    this->refreshFileList();
    // read files of current dir
    auto files = this->compress->GetFiles();
    for (auto file: files) {
        
    }
}