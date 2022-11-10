#include "bar.h"
#include "gtkmm/menubutton.h"
#include "sigc++/functors/mem_fun.h"
#include <iostream>
namespace Bar {
Bar::Bar(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) : Gtk::Box(cobject) {
    about = builder->get_widget<Gtk::MenuButton>("AboutButton");
    about->signal_show().connect(sigc::mem_fun(*this, &Bar::aboutClick));
    std::cout << "build bar" << std::endl;
}
void Bar::aboutClick() {
    std::cout << "click" << std::endl;
}
}