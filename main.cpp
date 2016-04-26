#include "graphics.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>

using namespace genv;
using namespace std;



// Convert int to str
string inttostr(int i){
    ostringstream os;
    os << i;
    return os.str();
}



const float PI      = 3.14159265358979323846;

const int WWIDTH    = 800;
const int WHEIGHT   = 600;

const int mapW      = 20;
const int mapH      = 20;
const int boxSize   = 20;

const unsigned char COLOR_BG[3]             = {52, 73, 94};
const unsigned char COLOR_BG2[3]            = {22, 43, 64}; // Hovered
const unsigned char COLOR_BOX_BORDER[3]     = {34, 52, 70};
const unsigned char COLOR_BOX_MARK[3]       = {189, 195, 199};

struct Pos{
    int x, y;
};



#include "widget.h"
#include "label.h"
#include "box.h"
#include "button.h"
#include "game.h"













int main(){
    srand(time(0));

    Game::init();

    return 0;
}
