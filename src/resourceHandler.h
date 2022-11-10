#pragma once
#include "gdkmm/pixbuf.h"
#include "glibmm/refptr.h"
#include <boost/filesystem/path.hpp>

class ResourceHandler {
public:
    ResourceHandler();
    Glib::RefPtr<Gdk::Pixbuf> folderIcon;
    boost::filesystem::path mainWindow;
    boost::filesystem::path bar;
private:
    boost::filesystem::path currentPath;
    boost::filesystem::path assetPath;
    boost::filesystem::path designPath;
};