#include "alphabeta.hpp"
#include "utils.hpp"
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace draughts;

constexpr int INF = 1000000000;

MiniMaxAlphaBeta::MiniMaxAlphaBeta(std::shared_ptr<EvaluationStrategy> evaluator, int depth, bool log):
    _searchDepth(depth), _boardEvaluated(0), _cutoffsProduced(0), _evaluationStrategy(evaluator), _log(log)
{}

Move MiniMaxAlphaBeta::GetBestMove(std::shared_ptr<Position> position, const Player& player)
{
    Move move;
    Alliance player_alliance = player.GetAlliance();
    //Alliance opponent_alliance = player.GetOpponentAlliance();
    std::vector<Move> lolm;
    position->LegalMoves(player_alliance, lolm);

    /*auto redPieces = position->GetPieces(Alliance::RED);

    std::cout << "Red pieces: \n";
    for(const auto& [i, p] : redPieces)
    {
        Piece piece = *p;
        std::cout << "px = " << piece.GetCol()  << " py = " << piece.GetRow() << "\n";
    }
    std::cout << std::endl;*/

    if(lolm.size() == 1)
    {
        if(_log)
            std::cout << "Only one move available!" << std::endl;
        return lolm.at(0);
    }

    Move bestMove;

    if(_log)
    {
        std::cout << "AI starts thinking with depth = " << this->_searchDepth << std::endl;
        std::cout << "Legal moves count = " << lolm.size() << std::endl;
    }

    auto totalMoveTime = 0u;
    int alpha = -INF;
    int beta = +INF;
    int moveNumber = position->GetMoveLog().size();

    std::vector<std::pair<int, Move>> vBestMoves;
    if(player_alliance == Alliance::DARK)
    {
        std::cout << position->ToString() << std::endl;
        int minVal = INF;
        for(auto it = lolm.begin(); it != lolm.end(); ++it)
        {
            int number = moveNumber;
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            position->MakeMove(*it); //CPU make the move

            std::string curr_line = position->MoveToNotation(*it);
            std::shared_ptr<Position> positionCopy = position->MakeCopy();
            Evaluation currVal = max(positionCopy, this->_searchDepth, alpha, beta, curr_line);
            //beta = std::min(beta, minVal);
            if(currVal.score < minVal)
            {
                bestMove = *it;
                vBestMoves.push_back({currVal.score, bestMove});
                minVal = std::min(currVal.score, minVal);
                if(position->IsEndgameScenario())
                    break;
            }

            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            position->UndoLastMove();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            totalMoveTime += elapsed;

            if(_log)
            {
                std::cout << "Analyzed move " + position->MoveToNotation(*it)
                          << " Score: " << currVal.score << " Time taken: "
                          << elapsed << "ms" << std::endl;
                std::cout << "Best line:\n";
                std::vector<std::string> v_best_line = Utils::split(currVal.best_line, " ");
                const size_t N = v_best_line.size();
                for(size_t i = 0; i < N; ++i)
                {
                    if(i % 2 == 0)
                    {
                        std::cout << (i == 0 ? "... " : " ");
                    }
                    else
                    {
                        ++number;
                        std::cout << " " << number << ". ";
                    }
                    std::cout << v_best_line[i];
                }

                std::cout << "\n";
                std::cout << currVal.eval_board << "\n";
            }
        }
    }
    else if(player_alliance == Alliance::LIGHT)
    {
        int maxVal = -INF;
        for(auto it = lolm.begin(); it != lolm.end(); ++it)
        {
            int number = moveNumber;
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            position->MakeMove(*it); //CPU make the move
            std::string curr_line = position->MoveToNotation(*it);
            std::shared_ptr<Position> positionCopy = position->MakeCopy();
            Evaluation currVal = min(positionCopy, this->_searchDepth, alpha, beta, curr_line);
            //beta = std::min(beta, minVal);
            if(currVal.score > maxVal)
            {
                bestMove = *it;
               vBestMoves.push_back({currVal.score, bestMove});
                maxVal = std::max(currVal.score, maxVal);
                if(position->IsEndgameScenario())
                    break;
            }

            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            position->UndoLastMove();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            totalMoveTime += elapsed;

            if(_log)
            {
                std::cout << "Analyzed move " + position->MoveToNotation(*it)
                          << " Score: " << currVal.score << " Time taken: "
                          << elapsed << "ms" << std::endl;
                std::cout << "Best line:\n";
                std::vector<std::string> v_best_line = Utils::split(currVal.best_line, " ");

                for(size_t i = 0; i < v_best_line.size(); ++i)
                {
                    if(i % 2 == 0)
                    {
                        std::cout << (i == 0 ? "... " : " ");
                    }
                    else
                    {
                        ++number;
                        std::cout << " " << number << ". ";
                    }
                    std::cout << v_best_line[i];
                }

                std::cout << "\n";
                std::cout << currVal.eval_board << "\n";
            }
        }
    }

    if(_log)
    {
        std::cout << "boardEvaluated_ = " << _boardEvaluated << std::endl;
        std::cout << "cutoffsProduced_ = " << _cutoffsProduced << std::endl;
        std::cout << "Best move  = " + position->MoveToNotation(bestMove) << std::endl;
        std::cout << "Total move time = " << totalMoveTime << "ms" << std::endl;
    }

    if(vBestMoves.size() > 1)
    {
        std::sort(vBestMoves.begin(), vBestMoves.end(), [&](const auto& a, const auto& b){
            return player_alliance == Alliance::DARK ? a.first < b.first : a.first > b.first;
        });

        int bestScore = vBestMoves.front().first;

        auto it_ = std::remove_if(vBestMoves.begin(), vBestMoves.end(), [player_alliance, bestScore](const auto& a){
            return player_alliance == Alliance::DARK ? a.first > bestScore : a.first < bestScore;;
        });

        vBestMoves.erase(it_, vBestMoves.end());

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0, vBestMoves.size());
        const int rndIdxFrom = dist(mt);

        return vBestMoves.at(rndIdxFrom).second;
    }

    return vBestMoves.front().second;
}

MiniMaxAlphaBeta::Evaluation MiniMaxAlphaBeta::max(std::shared_ptr<Position> position, int depth, int alpha, int beta, std::string curr_line)
{
    if(depth == 0 || position->IsEndgameScenario())
    {
        ++_boardEvaluated;
        const std::shared_ptr<Rules> rules = position->GetRules();
        int score = _evaluationStrategy->Evaluate(*position, *rules);
        Evaluation eval;
        eval.score = score;
        eval.best_line = curr_line;
        eval.eval_board = position->ToString();
        return eval;
    }

    /*std::string curr_board =  position->ToString();
    std::cout << curr_board << std::endl;*/

    int value = -INF;
    std::vector<Move> lolm; // list of legal moves
    position->LegalMoves(Alliance::LIGHT, lolm);

    Evaluation evalBest;
    for(auto it = lolm.begin(); it != lolm.end(); ++it)
    {
        position->MakeMove(*it);
        std::stringstream ss;
        ss << curr_line << std::string(" ") <<  position->MoveToNotation(*it);
        std::string line = ss.str();
        std::shared_ptr<Position> positionCopy = position->MakeCopy();
        Evaluation evaluation = min(positionCopy, CalculateQuiescenceDepth(*positionCopy, depth), alpha, beta, line);
        value = std::max(value, evaluation.score);
        if(value == evaluation.score)
        {
            //best_line = evaluation.best_line;
            evalBest = evaluation;
        }

        alpha = std::max(alpha, value);
        if(value >= beta)
        {
            ++_cutoffsProduced;
            break;
        }        
        position->UndoLastMove();
    }
    return evalBest;
}

MiniMaxAlphaBeta::Evaluation MiniMaxAlphaBeta::min(std::shared_ptr<Position> position, int depth, int alpha, int beta, std::string curr_line)
{
    if(depth == 0 || position->IsEndgameScenario())
    {
        ++_boardEvaluated;
        const std::shared_ptr<Rules> rules = position->GetRules();
        int score = _evaluationStrategy->Evaluate(*position, *rules);
        Evaluation eval;
        eval.score = score;
        eval.best_line = curr_line;
        eval.eval_board = position->ToString();
        return eval;
    }

    /*std::string curr_board = position->ToString();
    std::cout << curr_board << std::endl;*/

    int value = INF;
    std::vector<Move> lolm; // list of legal moves
    position->LegalMoves(Alliance::DARK, lolm);

    Evaluation evalBest;
    for(auto it = lolm.begin(); it != lolm.end(); ++it)
    {
        position->MakeMove(*it);
        std::stringstream ss;
        ss << curr_line << std::string(" ") << position->MoveToNotation(*it);
        std::string line = ss.str();
        std::shared_ptr<Position> positionCopy = position->MakeCopy();
        Evaluation evaluation = max(positionCopy, CalculateQuiescenceDepth(*positionCopy, depth), alpha, beta, line);
        value = std::min(value, evaluation.score);
        if(value == evaluation.score)
        {
            //best_line = evaluation.best_line;
            evalBest = evaluation;
        }
        beta = std::min(beta, value);
        if(value <= alpha)
        {
            ++_cutoffsProduced;
            break;
        }
        position->UndoLastMove();
    }
    return evalBest;
}

int MiniMaxAlphaBeta::CalculateQuiescenceDepth(const Position &position, int depth)
{
    int total = position.GetTotalPieces();
    if(total <= 8)
        depth = std::min(8, depth);
    if(total <= 6)
        depth = std::min(6, depth);
    if(total <= 5)
        depth = std::min(4, depth);
    if(total <= 3)
        depth = std::min(2, depth);
    return depth - 1;
}
