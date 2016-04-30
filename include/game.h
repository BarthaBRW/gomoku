#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


class Game{
private:
    static event ev;
    static vector<Box*> boxes;

    static void clearScreen();


public:
    static bool turn;                     // true=p1, false=p2||ai
    static unsigned char state;           // Game state, eg. 0menu, 1game, 2game over, 3drawn
    static bool pvp;                      // false AI, true pvp
    static int round;
    static bool finished;


    static void moveAI();

    static void updateStateText(bool b);
    static Box* getPos(int x, int y);
    static bool isWin(Widget *w);
    static bool isDrawn();
    static void onBoxClick(Widget *w);
    static void newGame();
    static void init();
};

vector<Box*> Game::boxes;
bool Game::turn                 = true;
unsigned char Game::state       = 1;
bool Game::pvp                  = false;
int Game::round                 = 0;
bool Game::finished             = false;




/*
 * AI
 */



void Game::moveAI(){
    Pos p = AI::move();
    AI::step(p.x, p.y, 1); // AI 1+1=2
    Game::onBoxClick( Game::getPos(p.x, p.y) );
}





/*
 * Gameplay
 */

void Game::updateStateText(bool turn){
    if( Game::pvp )
        Widget::$["l_state"]->setCaption(turn?"Player 1's turn":"Player 2's turn");
    else
        Widget::$["l_state"]->setCaption(turn?"Your turn":"Opponent is thinking..");
    gout << refresh;
}



Box* Game::getPos(int x, int y){
    return boxes[mapW*y + x];
}


bool Game::isWin(Widget *w){
    unsigned char player = Game::turn?1:2;   // Looking for player 1 or player 2
    Pos p = w->getPos();
    int score;

    // X
    score = 0;
    for( int x=max(p.x-4, 0); x<=min(p.x+4, mapH-1); x++ ){
        if( Game::getPos(x, p.y)->getState() == player ){
            score++;
            if( score >= 5 ) return true;
        }
        else
            score = 0;
    }
    if( score >= 5 )
        return true;

    // Y
    score = 0;
    for( int y=max(p.y-4, 0); y<=min(p.y+4, mapH-1); y++ ){
        if( Game::getPos(p.x, y)->getState() == player ){
            score++;
            if( score >= 5 ) return true;
        }
        else
            score = 0;
    }
    if( score >= 5 )
        return true;

    // diag1
    score = 0;
    int x = max(p.x-4, 0);
    for( int y=min(p.y+4, mapH-1); y>=max(p.y-4, 0); y-- ){
        if( x > min(p.x+4, mapW-1) )
            break;

        if( Game::getPos(x, y)->getState() == player ){
            score++;
            if( score >= 5 ) return true;
        }
        else
            score = 0;

        x++;
    }

    // diag2
    score = 0;
    x = max(p.x-4, 0);
    for( int y=max(p.y-4, 0); y<=min(p.y+4, mapH-1); y++ ){
        if( x > min(p.x+4, mapW-1) )
            break;

        if( Game::getPos(x, y)->getState() == player ){
            score++;
            if( score >= 5 ) return true;
        }
        else
            score = 0;

        x++;
    }
    return false;
}


bool Game::isDrawn(){
    for( Widget *w: Game::boxes ){
        if( w->getState() == 0 )
            return false;
    }
    return true;
}

void Game::onBoxClick(Widget *w){
    if( w->getState() > 0 || Game::finished )
        return;

    w->setState(Game::turn ? 1 : 2);
    Game::updateStateText(!Game::turn);


    if( Game::isWin(w) ){
        if( Game::pvp )
            Widget::$["l_state"]->setCaption( Game::turn?"Player 1 win":"Player 2 win" );
        else
            Widget::$["l_state"]->setCaption( Game::turn?"You win:)":"You lost :/ You should try again :)" );

        Game::finished = true;
        return;
    }
    if( Game::isDrawn() ){
        Widget::$["l_state"]->setCaption("Drawn. It was close :)");
        Game::finished = true;
        return;
    }

    Game::turn = Game::turn ? false : true;
    Game::round++;

    // AI turn after player click
    if( !Game::pvp && !Game::turn ){//Player click
        Pos p = w->getPos();
        AI::step(p.x, p.y, 0); // Player 0+1=
        Game::moveAI();
    }

}


void Game::newGame(){
    Game::turn = true; // Turn player 1
    Game::state = 1;
    Game::round = 0;
    Game::finished = false;

    if( !Game::pvp )
        AI::init();

    // Reset boxes
    for( Box *b : Game::boxes ){
        b->setState(0);
    }

    Game::updateStateText(Game::turn);
}




/*
 * Main
 */

void Game::clearScreen(){
    gout << move_to(0, 0) << color(COLOR_BG[0], COLOR_BG[1], COLOR_BG[2]) << box(WWIDTH, WHEIGHT);
}

void Game::init(){
    gout.open(WWIDTH, WHEIGHT);
    event ev;

    // Menu buttons
    new Button("b_newgame", 25, 20, "New Game", [](Widget *w)->void{
       Game::newGame();
    });

    // You lose/win text
    new Label("l_state", 25, 60, "");



    // Create the map
    for( int y=0; y<mapH; y++ ){
        for( int x=0; x<mapW; x++ ){
            Game::boxes.push_back(new Box(25 + x*boxSize, 100+ y*boxSize, boxSize, boxSize, Game::boxes.size(), Game::onBoxClick));
        }
    }

    Game::newGame();



    // Event loop
    gin.timer(30);
    while(gin >> ev && ev.keycode != key_escape) {
        if( ev.type == ev_timer )
            Game::clearScreen();

        Widget::handleWidgets(ev);

        if( ev.type == ev_timer ){
            gout << refresh;
        }
    }
}


#endif // GAME_H_INCLUDED
