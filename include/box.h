#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

class Box : public Widget{
    unsigned char state;
    int id;
public:
    Box(int _x, int _y, int _w, int _h, int _id, void (*callback)(Widget*)) : Widget("box"+inttostr(_id), _x, _y, _w, _h){
        onClick = callback;
        id = _id;
    }

    void draw(){
        canvas canv;
        canv.open(w+1, h+1);

        // Background
        if( hovered ) canv.set_color(COLOR_BG2[0], COLOR_BG2[1], COLOR_BG2[2]);
        else canv.set_color(COLOR_BG[0], COLOR_BG[1], COLOR_BG[2]);

        canv << move_to(0, 0) << box(w, h);

        // Border
        canv.set_color(COLOR_BOX_BORDER[0], COLOR_BOX_BORDER[1], COLOR_BOX_BORDER[2]);
        canv << move_to(0, 0) << line_to(w, 0)
             << move_to(0, 0) << line_to(0, h)
             << move_to(w, 0) << line_to(w, h)
             << move_to(0, h) << line_to(w, h);

        // State
        canv.set_color(COLOR_BOX_MARK[0], COLOR_BOX_MARK[1], COLOR_BOX_MARK[2]);
        if( state == 1 )
            canv << move_to(2, 2) << line_to(w-2, h-2)
                 << move_to(2, h-2) << line_to(w-2, 2);
        if( state == 2 ){
            canv << move_to(3, 3)
                 << line_to(w-3, 3)
                 << line_to(w-3, h-3)
                 << line_to(3, h-3)
                 << line_to(3, 3);
        }

        gout << stamp(canv, x, y);
    }

    bool handle(event ev){


        draw();
        return false;
    }


    unsigned char getState(){return state;}
    void setState(unsigned char c){state = c;}



    Pos getPos(){
        Pos res;
        res.x = id%mapW;
        res.y = id/mapW;
        return res;
    }
};


#endif // BOX_H_INCLUDED
