#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include <map>
#include <functional>


// Convert int to str
string inttostr(int i){
    ostringstream os;
    os << i;
    return os.str();
}
struct Pos{
    int x, y;
};




class Widget{
protected:
    int x, y, w, h;
    bool visible = true;
    bool hovered = false;
    function<void()> callback = []()->void{};
    string name;


public:
    Widget(string _name, int _x, int _y, int _w, int _h) : name(_name), x(_x), y(_y), w(_w), h(_h){
        widgets.push_back(this);
        $[name] = this;
    }

    static vector<Widget*> widgets;
    static map<string, Widget*> $; // Widget names
    static void handleWidgets(event ev);


    bool isVisible(){return visible;}
    void setVisible(bool b){visible = b;}

    bool isHovered(){return hovered;}
    void setHovered(bool b){hovered = b;}

    virtual unsigned char getState(){}
    virtual void setState(unsigned char c){}

    virtual void setCaption(string s){}

    virtual Pos getPos(){}

    int getX(){return x;}
    int getY(){return y;}
    int getW(){return w;}
    int getH(){return h;}


    void (*onClick)(Widget *_this);

    virtual void draw() = 0;
    virtual bool handle(event ev) = 0;
};


// Storage
vector<Widget*> Widget::widgets;
map<string, Widget*> Widget::$;




// Event loop
void Widget::handleWidgets(event ev){
    for( Widget *w : Widget::widgets ){
        if( !w->isVisible() )
            continue;


        // Hover
        if( ev.type == ev_mouse ){
            w->setHovered(false);
            if( ev.pos_x >= w->getX() && ev.pos_x < w->getX()+w->getW() &&
                ev.pos_y >= w->getY() && ev.pos_y < w->getY()+w->getH() ){
                w->setHovered(true);
            }
        }

        // onClick event
        if( ev.button == btn_left ){
            if( ev.pos_x >= w->getX() && ev.pos_x < w->getX()+w->getW() &&
                ev.pos_y >= w->getY() && ev.pos_y < w->getY()+w->getH() ){
                w->onClick(w);
            }
        }


        // Prevent double interaction
        // If the given widget gives back true, we stop the loop
        if( w->handle(ev) )
            break;
    }
}


#endif // WIDGET_H_INCLUDED
