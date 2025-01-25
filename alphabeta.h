#pragma once

#include "position.h"
#include "movestrategy.h"
#include "evaluation_strategy.h"
#include <vector>
#include <string>

namespace draughts
{
    class MiniMaxAlphaBeta: public MoveStrategy
    {
    public:
        explicit MiniMaxAlphaBeta(std::shared_ptr<EvaluationStrategy> evaluator, int depth = 8, bool log = true);
        virtual Move GetBestMove(std::shared_ptr<Position> position, const Player& player) override;
    private:
        class Evaluation
        {
        public:
            int score = 0;
            std::string best_line = "";
            std::string eval_board = "";
        };
    private:
        Evaluation max(std::shared_ptr<Position> position, int depth, int alpha, int beta, std::string curr_line);
        Evaluation min(std::shared_ptr<Position> position, int depth, int alpha, int beta, std::string curr_line);
        int CalculateQuiescenceDepth(const Position &position, int depth);
    private:
        int _searchDepth;
        int _boardEvaluated;
        int _cutoffsProduced;
        std::shared_ptr<EvaluationStrategy> _evaluationStrategy;
        bool _log;
    };
}
