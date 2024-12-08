/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 11/10/2024
 * Purpose: Blackjack Game Project
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

// System Libraries
#include <fstream>
#include <iomanip>
#include <ext/slist> // slist for player hand
#include <map>       // Map
#include <set>       // Set
#include <stack>     // Stack
#include <queue>     // Queue for players
#include <algorithm> // For random_shuffle, sort
#include <cstdlib>
#include <ctime>     // Time
#include <cstring>   // For strcpy

using __gnu_cxx::slist;
using namespace std;

// Utility functions for displaying cards
void getCardGraphic(int card, char cardLines[6][7]);
void getHiddenCardGraphic(char cardLines[6][7]);

// Class representing a deck of cards for the game
class CardDeck {
private:
    map<int, int> cardCounts; // Map
    set<int> usedCards;       // Set
    stack<int> returnedCards; // Stack
    int deckArray[364];       // Array representing the complete deck

    // Initializes the deck array and card counts
    void initializeDeck();
    // Reshuffles the deck when needed
    void reshuffleDeck();

public:
    CardDeck();
    // Shuffles the deck using random_shuffle
    void shuffleDeck();
    // Draws a card and returns its value (1-11)
    int drawCard();
    // Checks if the deck needs reshuffling (after 3/4 usage)
    bool needsReshuffling() const;
    // Returns a card to the discard stack
    void returnCard(int card);
    // Prints the current card counts
    void printCardCounts() const;
    void displayDeckStatus() const;
};

// Class representing a player
class Player {
private:
    slist<int> hand;
    int score;

public:
    Player();
    ~Player() = default;
    // Adds a card
    void addCard(int card);
    int calculateScore();
    // Checks if the player has a blackjack
    bool hasBlackjack();
    // Shows the player's hand
    void showHand(bool hideFirstCard = false);
    // Returns the player's current score
    int getScore() const;
    // Clears the player's hand
    void clearHand();
    // Sorts the player's hand in ascending order
    void sortHand();
    // Displays the sorted hand
    void showSortedHand();
    string handToString() const;
};

// Class for game statistics
class GameStatistics {
private:
    int totalGames;
    int playerWins;
    int houseWins;
    int ties;

public:
    GameStatistics();
    void recordResult(int result); // 1 for player win, -1 for house win, 0 for tie
    void displayStatistics() const;
};

// Class managing game
class BlackjackGame {
private:
    float balance;             // Player's current balance
    float initialBalance;      // Initial balance
    int* gameHistory;
    int historyCount;
    CardDeck deck;
    ofstream log;              // Log file for game results
    queue<Player> players;     // Queue
    GameStatistics stats;

    // Logs a detailed report
    void logDetailedState();

public:
    // constructor
    BlackjackGame();
    // destructor
    ~BlackjackGame();
    void playGame();
    void displayHistory() const;
    //log results in a file
    void logResult(const string& result);
    void placeBet(float& bet);
    void handleResult(Player& player, Player& house, float& bet);
    void initializePlayers(int numPlayers);
    void printRules() const;
    void displayBalanceReport() const;
};

#endif // BLACKJACK_H
