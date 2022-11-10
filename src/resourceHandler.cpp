#include "resourceHandler.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include "gdkmm/pixbuf.h"

static boost::filesystem::path add(boost::filesystem::path a, std::string next) {
    return a.append(next);
}
ResourceHandler::ResourceHandler() {
    this->currentPath = boost::dll::program_location().remove_filename();
    this->assetPath = add(this->currentPath, "asset");
    this->folderIcon = Gdk::Pixbuf::create_from_file(add(this->assetPath, "folder.png").string());
    this->designPath = add(this->currentPath, "design");
    this->mainWindow = add(this->designPath, "mainWin.glade");
}