#pragma once


#include "gtkmm/menubutton.h"
#include <gtkmm/popovermenubar.h>

class Bar: public Gtk::PopoverMenuBar {
private:
    Gtk::MenuButton about;
public:
    Bar();
    void aboutClick();
};