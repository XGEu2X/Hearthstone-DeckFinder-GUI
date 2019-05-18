#ifndef DEFAULTPARAMETERS_H
#define DEFAULTPARAMETERS_H

#endif // DEFAULTPARAMETERS_H

#include <string>
#include <vector>

using namespace std;

const vector<string> CARD_CLASSES = {"DRUID", "HUNTER", "MAGE", "PALADIN", "PRIEST", "ROGUE", "SHAMAN", "WARLOCK", "WARRIOR"};
const vector<string> SYNERGY_LEVELS = {"Low", "Medium", "High", "Very high"};
const vector<string> DECK_TYPES = {"Super Agro", "Very Agro", "Agro", "Midrange", "Control", "Very Control", "Super Control"};
const int MAX_TUPLE_NUMBER = 4;

const string DECK_FINDER_PATH = "";

const int MAX_NUMBER_OF_CARDS = 60;
const string STATS_RESOURCE = "HSReplay";
const double MIN_MATCH_VALID_RATIO = 0.2;

const int NUM_OF_GENERATIONS = 10000;
const int LIMIT_OF_GENERATIONS_WITHOUT_IMPROVMENT = 200;
const int AMOUNT_OF_POPULATION = 40;
const double PROBABILITY_OF_MUTATION = 0.2;
const int RANDOM_ELEMENTS_IN_POPULATION = 1;
