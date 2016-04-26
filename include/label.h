#ifndef BB_LABEL_H_INCLUDED
#define BB_LABEL_H_INCLUDED

class Label : public Widget{
protected:
    string caption;
    unsigned char colour[3] = {255, 255, 255};

public:
    Label();
    Label(string _name, unsigned _x, unsigned _y, string _caption = "") : Widget(_name, _x, _y, gout.twidth(_caption), 30){
        caption = _caption;
        callback = [](){}; // Do nothing by default :)
    }


    void setCaption(string s){
        caption = s;
        w = gout.twidth(caption);
    }


    void draw(){
        gout << move_to(x, y) << color(colour[0], colour[1], colour[2]) << text(caption);
    }

    bool handle(event ev){

        draw();
        return false;
    }
};

#endif // BB_LABEL_H_INCLUDED
