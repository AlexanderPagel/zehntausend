// ui_subobject.h
//
// TODO Brief explanation.

#ifndef UI_SUBOBJECT_H_INCLUDED
#define UI_SUBOBJECT_H_INCLUDED 1


namespace ui
{

class Ui;

class UiSubobject
{
    Ui* ui;

  public:
    Ui* getUi() { return ui; }
    Ui const* getUi() const { return ui; }

    explicit UiSubobject(Ui* ui) : ui{ui} {} // Does this work with public inheritance?
};


} // namespace ui


#endif // UI_SUBOBJECT_H_INCLUDED
