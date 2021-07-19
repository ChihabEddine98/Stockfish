//
// Created by Abdelkader Chihab Benamara on 19/07/2021.
//

#ifndef STOCKFISH_ALPHABETA_H
#define STOCKFISH_ALPHABETA_H

#include "types.h";
#include "position.h";


namespace Stockfish {

    extern Position &root;
    extern Depth depth;

    namespace AlphaBeta {
        void init(Position& root,int depth);
        Move search(Position& pos);
        Move best_move(Position& pos,Depth depth);
        Value negamax(Position& pos, Depth depth, int alpha, int beta, Color color);

    }




}

#endif //STOCKFISH_ALPHABETA_H
