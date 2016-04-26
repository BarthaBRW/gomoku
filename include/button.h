#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED


class Button : public Widget{
    string caption;
public:
    Button(string _name, int _x, int _y, string _caption, void (*callback)(Widget*)) : Widget(_name, _x, _y, gout.twidth(_caption), gout.cascent()+gout.cdescent()){
        onClick = callback;
        caption = _caption;
    }

    void draw(){
        gout.set_color(255, 255, 255);
        gout << move_to(x, y+gout.cascent()-gout.cdescent()/2) << text(caption);
    }

    bool handle(event ev){


        draw();
        return false;
    }
};

#endif // BUTTON_H_INCLUDED
