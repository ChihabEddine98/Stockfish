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
#include "alpha_beta.h"


using namespace Stockfish;


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
    std::string StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position pos;
    StateInfo st;

    StateListPtr states(new std::deque<StateInfo>(1));

    pos.set(StartFEN, false,&states->back(),Threads.main());


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


    return;


}

int main(int argc, char* argv[])
{

    engine_init(argc,argv);
    debug();


    return 0;
}


