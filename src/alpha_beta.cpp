//
// Created by Abdelkader Chihab Benamara on 19/07/2021.
//

#include "position.h"
#include "evaluate.h"
#include "search.h"
#include "types.h"

#include "alpha_beta.h"


namespace Stockfish {

    Position& root;
    Depth depth;

    namespace AlphaBeta {

        // Initialize the Search Object AlphaBeta with root <r> and depth <d>
        void AlphaBeta::init(Position& r,Depth d)
        {
            root = r;
            depth = d;
            return;
        }

        // Do The Alpha Beta Search from a given position
        Move AlphaBeta::search(Position& pos)
        {
            return best_move(pos,depth);
        }

        // Return the best move after an alpha-beta search
        Move best_move(Position& pos,Depth depth)
        {
            MoveList<LEGAL> moves = MoveList<LEGAL>(pos);
            Value best_value = -VALUE_INFINITE;
            Move best_move;

            for(auto& move : moves )
            {
                pos.do_move(move,st);
                v = std::max(best_value,negamax(pos, depth - 1, -10000, 10000,pos.side_to_move()));
                pos.undo_move(move);

                if (v > best_value) {
                    best_move = move;
                    best_move = v;
                }
            }

            return best_move;
        }

        // Do the AlphaBeta Search using Negamax Framework
        Value negamax(Position& pos, Depth depth, int alpha, int beta, Color color)
        {
            //depth end reached ? or we actually hit a win/lose condition?
            if (depth == 0 || pos.is_draw(pos.game_ply()).points != 0)
            {
                return color*Eval::evaluate(pos);
            }

            // Get successors (possible actions from the actual Position)
            MoveList<LEGAL> moves = MoveList<LEGAL>(pos);


            // No possible moves - then it's a terminal state
            if (moves.size() == 0)
            {
                return color*Eval::evaluate(pos);
            }

            Value best_value = -VALUE_INFINITE;
            Value v;
            StateInfo st;

            for (auto& move : moves)
            {
                pos.do_move(move,st)
                v = -negamax(pos, depth - 1, -beta, -alpha, -color);
                pos.undo_move(move);

                best_value = std::max(best_value, v);
                alpha = std::max(alpha, v);
                if (alpha >= beta)
                    break;
            }
            return best_value;
        }

    }




}
