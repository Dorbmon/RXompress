#pragma once
#include "gtkmm/box.h"
#include "gtkmm/builder.h"
#include "gtkmm/menubutton.h"
#include "resourceHandler.h"
namespace Bar {

class Bar: public Gtk::Box {
private:
    Gtk::MenuButton* about;
public:
    Bar(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    void aboutClick();
};

static Bar* createBar(ResourceHandler* resourceHandler) {
    auto builder = Gtk::Builder::create_from_file(resourceHandler->bar.string());
    auto bar = builder->get_widget_derived<Bar>(builder, "Bar");
    return bar;
}
};