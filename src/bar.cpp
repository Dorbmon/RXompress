#include "bar.h"
#include "sigc++/functors/mem_fun.h"
#include <iostream>

Bar::Bar() {
    about.set_label("About");
    about.set_can_focus(true);
    about.signal_show().connect(sigc::mem_fun(*this, &Bar::aboutClick));
    //append(about);
    add_child(about, "1");
}
void Bar::aboutClick() {
    std::cout << "click" << std::endl;
}