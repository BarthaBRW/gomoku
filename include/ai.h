#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED


#include <limits>
#include <algorithm>
#include <functional>


const int/*l*/ INFTY = INFINITY;


struct mapPoint{
    int r,
        c;
    int set = 0;
    int score = 0;
    bool valid = false;
    vector<vector<int>> info = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
};

void copyMapPointVector(vector<mapPoint> &v1, const vector<vector<mapPoint>> &v2){
    v1.clear();
    for( int x=0; x<v2.size(); x++ ){
        for( int y=0; y<v2[x].size(); y++ ){
            v1.push_back(v2[x][y]);
        }
    }
}






int mc = 0;
int sum = 0;
int setNum = 0;
int/*l*/ alpha,
     beta;

//vector<mapPoint> scoreMap;
vector<mapPoint> scoreQueue;
vector<vector<mapPoint>> Map;


//int boardBuff[mapW*mapH];
int boardBuffArr[mapW*mapH] = {0};

string bufstr;
string bufToString(){
    string res;
    for( int i=0; i<mapW*mapH; i++ ){
        res += boardBuffArr[i];
    }
    return res;
}


const int moves[4][2] = {
    {-1, -1},
    {-1, 0},
    {0, -1},
    {-1, 1}
};

const int coe[2] = {-2, 1};
const int/*l*/ scores[6] = {0, 1, 10, 2000, 4000, 100000000000};
const int totry[2] = {10, 10};
const int _depth = 3;


map<string, int> cache;



class AI{
private:



public:
    static void init(){
        Map.clear();
        for( int x=0; x<mapW; x++ ){
            vector<mapPoint> tmp;
            for( int y=0; y<mapH; y++ ){
                mapPoint a;
                a.r = x;
                a.c = y;
                tmp.push_back(a);
                scoreQueue.push_back(a);
            }
            Map.push_back(tmp);
        }
    }


    static void _updateMap(int r, int c, int num, bool remove){
        int i = 4;
        int x, y, step, xx, yy;
        int e, changes = 0;
        int/*l*/ s;
        int tmp;
        if( !remove ){
            boardBuffArr[r*mapW+c] = num + 2;
            Map[r][c].set = num + 1;
            while(i--){
                x = r;
                y = c;

                step = 5;
                while( step-- && x>=0 && y>=0 && y<mapH ){
                    xx = x - moves[i][0] *4;
                    yy = y - moves[i][1] *4;
                    if( xx >= mapW || yy <0 || yy >= mapH ){
                        x += moves[i][0];
                        y += moves[i][1];
                        continue;
                    }
                    if( Map[x][y].info[i][2] > 0 ){
                        tmp = 5;
                        xx = x;
                        yy = y;
                        s = scores[ Map[x][y].info[i][2] ];
                        changes -= s * Map[x][y].info[i][3];
                        while( tmp-- ){
                            Map[xx][yy].score -= s;
                            xx -= moves[i][0];
                            yy -= moves[i][1];
                        }
                    }
                    Map[x][y].info[i][num]++;
                    if( Map[x][y].info[i][1-num] > 0 )
                        Map[x][y].info[i][2] = 0;
                    else{
                        Map[x][y].info[i][2] = Map[x][y].info[i][num];
                        e = coe[num];
                        Map[x][y].info[i][3] = e;
                        s = scores[ Map[x][y].info[i][2] ];
                        tmp = 5;
                        xx = x;
                        yy = y;
                        changes += s * Map[x][y].info[i][3];
                        while( tmp-- ){
                            Map[xx][yy].score += s;
                            xx -= moves[i][0];
                            yy -= moves[i][1];
                        }
                    }
                    x += moves[i][0];
                    y += moves[i][1];
                }
            }
        }
        else{
            boardBuffArr[r * mapW + c] = 0;

            Map[r][c].set = 0;
            while( i-- ){
                x = r;
                y = c;
                step = 5;
                while( step-- && x>=0 && y>=0 && y<mapH ){
                    xx = x - moves[i][0] *4;
                    yy = y - moves[i][1] *4;
                    if( xx>=mapW || yy<0 || yy>=mapH ){
                        x += moves[i][0];
                        y += moves[i][1];
                        continue;
                    }
                    int sc = 0;
                    Map[x][y].info[i][num]--;
                    if( Map[x][y].info[i][2] > 0 ){
                        tmp = 5;
                        xx = x;
                        yy = y;
                        s = scores[ Map[x][y].info[i][2] ];
                        changes -= s * Map[x][y].info[i][3];
                        while( tmp-- ){
                            Map[xx][yy].score -= s;
                            xx -= moves[i][0];
                            yy -= moves[i][1];
                        }
                        Map[x][y].info[i][2]--;
                        if( Map[x][y].info[i][num] > 0 )
                            sc = 1;
                    }
                    else{
                        if( Map[x][y].info[i][1-num] > 0 && !Map[x][y].info[i][num] )
                            sc = -1;
                    }

                    if( sc == 1 ){
                        tmp = 5;
                        s = scores[ Map[x][y].info[i][2] ];
                        xx = x;
                        yy = y;
                        changes += s * Map[x][y].info[i][3];
                        while( tmp-- ){
                            Map[xx][yy].score += s;
                            xx -= moves[i][0];
                            yy -= moves[i][1];
                        }
                    }
                    else{
                        if( sc == -1 ){
                            Map[x][y].info[i][2] = Map[x][y].info[i][1-num];
                            tmp = 5;
                            s = scores[ Map[x][y].info[i][2] ];
                            Map[x][y].info[i][3] = coe[1-num];
                            xx = x;
                            yy = y;
                            changes += s * Map[x][y].info[i][3];
                            while( tmp-- ){
                                Map[xx][yy].score += s;
                                xx -= moves[i][0];
                                yy -= moves[i][1];
                            }
                        }
                    }

                    x += moves[i][0];
                    y += moves[i][1];
                }
            }
        }
        sum += changes;
    }


    static void simulate(int x, int y, int num){
        //if( Map[x][y].valid )
        //    return;
        setNum++;
        AI::_updateMap(x, y, num, false);
    }
    static void desimulate(int x, int y, int num){
        //if( Map[x][y].valid )
        //    return;
        AI::_updateMap(x, y, num, true);
        setNum--;
    }


    static bool _sortMove(const mapPoint &a, const mapPoint &b){
        if( a.set > 0 )
            return true;
        if( b.set > 0 )
            return false;
        if( a.score < b.score )
            return true;
        return false;
    }

    static int/*l*/ nega(int x, int y, int depth, int/*l*/ b, int/*l*/ alpha){
        int i = 4;
        int num = depth%2;

        AI::simulate(x, y, num);
        bufstr = bufToString();
        if( cache[bufstr] )
            return cache[bufstr];
        if( abs(sum) >= INFTY )
            return -INFTY;
        if( setNum == mapW*mapH )
            return 0;
        else if( depth == 0 )
            return sum;

        sort( scoreQueue.begin(), scoreQueue.end(), AI::_sortMove );
        i = totry[num];

        vector<int> tmpQueue;
        b = beta;
        while( i-- ){
            tmpQueue.push_back(scoreQueue[i].c);
            tmpQueue.push_back(scoreQueue[i].r);
        }
        depth -= 1;
        i = tmpQueue.size()-1;
        x = tmpQueue[i];
        y = tmpQueue[--i];
        int/*l*/ score = -AI::nega(x, y, depth, -b, -alpha);
        AI::desimulate(x, y, depth%2);
        if( score > alpha ){
            bufstr = bufToString();
            cache[bufstr] = score;
            alpha = score;
        }
        if( alpha >= beta ){
            bufstr = bufToString();
            cache[bufstr] = beta;
            return alpha;
        }
        b = alpha + 1;
        while( i-- ){
            x = tmpQueue[i];
            y = tmpQueue[--i];
            score = -AI::nega(x, y, depth, -b, -alpha);
            AI::desimulate(x, y, depth%2);
            if( alpha<score && score < beta ){
                score = -AI::nega(x, y, depth, -beta, -alpha);
                AI::desimulate(x, y, depth%2);
            }
            if( score > alpha )
                alpha = score;
            if( alpha >= beta )
                return alpha;
            b = alpha+1;
        }
        return alpha;
    }



    static void watch(int r, int c, int color){
        AI::_updateMap(r, c, color, false); //0=player, 1=ai
        setNum++;
        sort( scoreQueue.begin(), scoreQueue.end(), AI::_sortMove );
        Map[r][c].valid = true;
    }


    static Pos move(){
        Pos bestpoint;
        cache.clear();

        alpha = -INFTY;
        beta = INFTY;
        int i = 20;
        int depth = _depth;


        vector<int> tmpQueue;


        copyMapPointVector(scoreQueue, Map);

        bestpoint.x = scoreQueue[0].r;
        bestpoint.y = scoreQueue[0].c;

        while(i--){
            tmpQueue.push_back(scoreQueue[i].c);
            tmpQueue.push_back(scoreQueue[i].r);
        }
        i = tmpQueue.size()-1;
        int x = tmpQueue[i];
        int y = tmpQueue[--i];
        int/*l*/ b = beta;
        int/*l*/ score = -AI::nega(x, y, depth, -b, -alpha);
        AI::desimulate(x, y, depth%2);

        if( score > alpha ){
            alpha = score;
            bestpoint.x = x;
            bestpoint.y = y;
        }

        /*
        b = alpha+1;
        while( i-- ){
            x = tmpQueue[i];
            y = tmpQueue[--i];
            score = -AI::nega(x, y, depth, -b, -alpha);
            AI::desimulate(x, y, depth%2);
            if( alpha < score && score < beta ){
                score = -AI::nega(x, y, depth, -beta, -alpha);
                AI::desimulate(x, y, depth%2);
            }
            if( score > alpha ){
                alpha = score;
                bestpoint.x = x;
                bestpoint.y = y;
            }
            b = alpha+1;
        }
        */


        int biggest = -INFINITY;
        int biggesti = -1;
        for( int i=0; i<scoreQueue.size(); i++ ){
            if( scoreQueue[i].valid )
                continue;
            if( scoreQueue[i].score > biggest ){
                biggest = scoreQueue[i].score;
                biggesti = i;
            }
        }

        bestpoint.x = scoreQueue[biggesti].r;
        bestpoint.y = scoreQueue[biggesti].c;

        return bestpoint;
    }





};


#endif // AI_H_INCLUDED
