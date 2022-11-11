#pragma once

#include "gtkmm/builder.h"
#include "gtkmm/label.h"
#include "gtkmm/window.h"
#include "resourceHandler.h"
#include <memory>
#include <fmt/format.h>
namespace AboutWin {
    static void show() {
        auto resourceHandler = std::make_shared<ResourceHandler>();
        auto builder = Gtk::Builder::create_from_file(resourceHandler->aboutWindow.string());
        auto aboutWin = builder->get_widget<Gtk::Window>("AboutWin");
        auto buildTimeLabel = builder->get_widget<Gtk::Label>("BuildTimeLabel");
        buildTimeLabel->set_label(fmt::format("Build at {} {}", __DATE__, __TIME__));
        aboutWin->set_default_size(500, 300);
        aboutWin->show();
    }
}