#include "window.h"
#include "Type/filetype.h"
#include "gtkmm/enums.h"
RxMainWindow::RxMainWindow(std::string inputFile){
  set_title("RX Compress");
  //set_default_size(200, 200);
  this->set_child(this->contentBox);
  this->contentBox.set_orientation(Gtk::Orientation::HORIZONTAL);
  this->contentBox.append(this->dirWin);
  this->contentBox.append(this->fileListWin);
  this->dirWin.set_child(this->dirList);
  this->fileListWin.set_child(this->fileList);
  FileType::GetFileType(inputFile);
}