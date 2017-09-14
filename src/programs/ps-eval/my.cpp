#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <boost/program_options.hpp>
#include <pokerstove/penum/ShowdownEnumerator.h>

using namespace pokerstove;
using namespace std;

vector<string> get_all_hands()
{
    const char* suits = "hdcs";
    const char* ranks = "23456789TJQKA";

    int suit_len = strlen(suits);
    int rank_len = strlen(ranks);

    vector<string> all_cards;
    for(int j = 0; j < rank_len; j++)
    {
        for(int i = 0; i < suit_len; i++)
        {
            string card;
            card.push_back(ranks[j]);
            card.push_back(suits[i]);

            all_cards.emplace_back(card);
        }
    }

    int card_len = all_cards.size();

    vector<string> all_pairs;
    for(int i = 0; i < card_len; i++)
    {
        for(int j = 0; j < i; j++)
        {
            // std::cout << all_cards[i] << all_cards[j] << std::endl;
            all_pairs.emplace_back(all_cards[i] + all_cards[j]);
        }
    }

    return all_pairs;
}


vector<EquityResult> eval_one(vector<string> hands, string board, boost::shared_ptr<PokerHandEvaluator> evaluator)
{
    vector<CardDistribution> handDists;
    for (const string& hand : hands) {
        handDists.emplace_back();
        handDists.back().parse(hand);
    }

    // fill with random if necessary
    if (handDists.size() == 1) {
        handDists.emplace_back();
        handDists.back().fill(evaluator->handSize());
    }

    // calcuate the results and print them
    ShowdownEnumerator showdown;
    vector<EquityResult> results =
        showdown.calculateEquity(handDists, CardSet(board), evaluator);

    return results;
}


int eval_one_board(string board)
{

    // allocate evaluator and create card distributions
    boost::shared_ptr<PokerHandEvaluator> evaluator = PokerHandEvaluator::alloc("h");

    vector<string> all_pairs = get_all_hands();
    int pair_len = all_pairs.size();

    for(int k = 0; k < pair_len; k++)
    {
        vector<string> hands;
        hands.emplace_back(all_pairs[k]);
        vector<EquityResult> results = eval_one(hands, board, evaluator);

        int hand_len = hands.size();

        double total = 0.0;
        for (const EquityResult& result : results) {
            total += result.winShares + result.tieShares;
        }

        for (size_t i = 0; i < results.size() && i < hand_len; ++i) {
            double equity = (results[i].winShares + results[i].tieShares) / total;
            std::cout << hands[i] << "\t" << board << "\t" << equity << "\t" << results[i].winShares << "\t" << total << std::endl;
        }
    }

    return 0;
}


int main(int argc, char** argv)
{
    const char* filename = "";
    if(argc > 1)
        filename = argv[1];

    ifstream is(filename);

    string board;
    while(getline(is, board))
    {
        if(board.size() > 5)
            eval_one_board(board);
    }

    return 0;
}

