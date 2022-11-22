#include "gtkmm/treeiter.h"
#include "window.h"
#include <boost/filesystem/operations.hpp>
#include <gdk/gdkcontentformats.h>
#include <iostream>
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
void RxMainWindow::initFileList() {
    // right click popup menu
    auto refGesture = Gtk::GestureClick::create();
    refGesture->set_button(GDK_BUTTON_SECONDARY);
    refGesture->signal_pressed().connect([this] (int, double x, double y) {
        const Gdk::Rectangle rect(x, y, 1, 1);
        fileListMenuPopup.set_pointing_to(rect);
        fileListMenuPopup.popup();
    });
    add_controller(refGesture);
    auto gmenu = Gio::Menu::create();
    gmenu->append("_Rename", "popup.rename");
    gmenu->append("_Remove", "popup.remove");

    fileListMenuPopup.set_parent(*this);
    fileListMenuPopup.set_menu_model(gmenu);
    fileListMenuPopup.set_has_arrow(false);

    // Create actions:
    auto refActionGroup = Gio::SimpleActionGroup::create();

    refActionGroup->add_action("rename",
    sigc::mem_fun(*this, &RxMainWindow::fileListRename));
    refActionGroup->add_action("remove",
    sigc::mem_fun(*this, &RxMainWindow::fileListRemove));

    insert_action_group("popup", refActionGroup);

    // Bind File Drop Event
    auto target = Gtk::DropTarget::create(GDK_TYPE_FILE_LIST, Gdk::DragAction::COPY);
    target->signal_drop().connect(sigc::mem_fun(*this, &RxMainWindow::fileListOnDropFile), false);
    this->fileList->add_controller(target);
}
void RxMainWindow::fileListRemove() {
    auto selection = this->fileList->get_selection();
    if (selection != nullptr) {
        auto row = *(selection->get_selected());
        if (this->compress != nullptr) {
            std::shared_ptr<FileTreeNode> node = row[this->fileListModel.node];
            auto res = this->compress->Remove(row[this->fileListModel.meta]);
            if (!res.first) {
                std::cerr << res.second << std::endl;
            } else {
                if (node->dirListColumn) {
                    this->refDirListModel->erase(node->dirListColumn);
                }
                if (node->fileListColumn) {
                    this->refFileListModel->erase(node->fileListColumn);
                }
            }
        }
        //row->get_iter
    }
}
void RxMainWindow::fileListRename() {
    auto selection = this->fileList->get_selection();
    if (selection != nullptr) {
        auto row = *(selection->get_selected());
        //row[this->fileListModel.fileName].column_.index();
        auto rows = selection->get_selected_rows();
        for (auto row: rows) {
            auto column = this->fileList->get_column(1);
            this->fileList->set_cursor(row, *column, true);
            this->fileList->grab_focus();
        }
        
    }
}
bool RxMainWindow::fileListOnDropFile(const Glib::ValueBase& value, double x, double y) {
    if (G_VALUE_HOLDS(value.gobj(), gdk_file_list_get_type())) {
        Glib::Value<GdkFileList*> file_list;
        file_list.init(value.gobj());
        GdkFileList* files = file_list.get();
        auto file = gdk_file_list_get_files(files);
        while (file) {
            auto s = (GFile*)(file->data);
            this->fileListInsertFileOrDir(g_file_get_path(s));
            file = file->next;
        }
        return true;
    }
    return false;
}
std::pair<bool, std::string> RxMainWindow::fileListInsertFileOrDir(const std::string& path) {
    auto p = boost::filesystem::path(path);
    std::cout << "enter f" << path << std::endl;
    if (boost::filesystem::is_regular_file(path)) { // is a regular file
        if (this->compress != nullptr) {
            auto node = this->compress->AddFile(this->compress->fileTree->currentNode->self->metaData, path);
            if (!std::get<0>(node)) {
                std::cerr << std::get<1>(node) << std::endl;
            } else {
                // Add File Successed
                this->refreshFileList();
            }
        }
    }
    return std::make_pair(true, "");
}