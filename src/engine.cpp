//
// Created by Abdelkader Chihab Benamara on 18/07/2021.
//

#include <iostream>

#include "bitboard.h"
#include "endgame.h"
#include "position.h"
#include "psqt.h"
#include "search.h"
#include "syzygy/tbprobe.h"
#include "thread.h"
#include "tt.h"
#include "uci.h"

#include "engine.h"



using namespace Stockfish;




// Do the AlphaBeta Search using Negamax Framework
Value negamax(Position& pos, Depth depth, int alpha, int beta, Color color)
{
    //depth end reached ? or we actually hit a win/lose condition?
    if (depth == 0 || pos.is_draw(pos.game_ply()))
    {
        return  color ? Eval::evaluate(pos) : -Eval::evaluate(pos);
    }

    // Get successors (possible actions from the actual Position)
    MoveList<LEGAL> moves = MoveList<LEGAL>(pos);


    // No possible moves - then it's a terminal state
    if (moves.size() == 0)
    {
        return  color ? Eval::evaluate(pos) : -Eval::evaluate(pos);
    }

    Value best_value = -VALUE_INFINITE;
    Value v;
    StateInfo st;

    for (auto& move : moves)
    {
        pos.do_move(move,st);
        v = -negamax(pos, depth - 1, -beta, -alpha, pos.side_to_move());
        pos.undo_move(move);

        best_value = std::max(best_value, v);
        alpha = std::max(alpha, (int)v);
        if (alpha >= beta)
            break;
    }
    return best_value;
}

// Return the best move after an alpha-beta search
Move best_move(Position& pos,Depth depth)
{
    MoveList<LEGAL> moves = MoveList<LEGAL>(pos);
    Value best_value = -VALUE_INFINITE;
    Move best_move;
    StateInfo st;
    Value v;



    for(auto& move : moves )
    {
        pos.do_move(move,st);
        v = std::max(best_value,negamax(pos, depth - 1, -10000, 10000,pos.side_to_move()));
        pos.undo_move(move);

        if (v > best_value) {
            best_move = move;
            best_value = v;
        }
    }

    return best_move;
}


// Do The Alpha Beta Search from a given position
Move search(Position& pos,Depth depth)
{
    return best_move(pos,depth);
}

void engine_init(int argc, char* argv[])
{
    std::cout << engine_info() << std::endl;

    CommandLine::init(argc, argv);
    UCI::init(Options);
    Tune::init();
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Endgames::init();
    Threads.set(size_t(Options["Threads"]));
    Search::clear(); // After threads are up
    Eval::NNUE::init();
}

void debug()
{
    std::string StartFEN = "rnbqkbnN/pppp2pp/8/8/4P3/4KNP1/PP3P1q/8 b q - 1 6";
    Position pos;
    StateInfo st;

    StateListPtr states(new std::deque<StateInfo>(1));

    pos.set(StartFEN, false,&states->back(),Threads.main());

    //AlphaBeta::init(pos,(Depth) 3);
    Move best_move = search(pos,(Depth) 7);

    std::cout << "Turn : " << pos.side_to_move() << std::endl;
    std::cout << "----------------- \n" << std::endl;
    std::cout << pos << std::endl;
    std::cout << "Best Move : " << UCI::move(best_move,false) << std::endl;


    /**
    MoveList<LEGAL> moves = MoveList<LEGAL>(pos);
    AlphaBeta::
    std::cout << "Evaluation : " <<Eval::evaluate(pos) << std::endl;
    std::cout << "----------------- \n" << std::endl;
    std::cout << pos << std::endl;


    for (auto& m : moves) {
        sync_cout << UCI::move(m,false) << sync_endl;
        pos.do_move(m,st);
        std::cout << "Evaluation : " <<Eval::evaluate(pos) << std::endl;
        std::cout << pos << std::endl;
        pos.undo_move(m);

    }
    **/

    return;


}

int main(int argc, char* argv[])
{

    engine_init(argc,argv);
    debug();


    return 0;
}


