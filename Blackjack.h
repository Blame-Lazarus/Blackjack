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
#include <map>       // Map
#include <set>       // Set
#include <stack>     // Stack
#include <queue>     // Queue for players
#include <algorithm> // For random_shuffle, sort
#include <cstdlib>
#include <ctime>     // Time
#include <cstring>   // For strcpy
#include <iostream>
#include <string>

// Utility functions for displaying cards
void getCardGraphic(int card, char cardLines[6][7]);
void getHiddenCardGraphic(char cardLines[6][7]);

// AVL Tree Implementation for player's hand
class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    Node* insertNode(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->key) {
            node->left = insertNode(node->left, key);
        } else {
            node->right = insertNode(node->right, key);
        }

        updateHeight(node);
        return balance(node);
    }

    void updateHeight(Node* node) {
        int hl = (node->left ? node->left->height : 0);
        int hr = (node->right ? node->right->height : 0);
        node->height = (hl > hr ? hl : hr) + 1;
    }

    int getBalanceFactor(Node* node) {
        if (!node) return 0;
        int hl = (node->left ? node->left->height : 0);
        int hr = (node->right ? node->right->height : 0);
        return hl - hr;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        int bf = getBalanceFactor(node);
        if (bf > 1) {
            if (getBalanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        } else if (bf < -1) {
            if (getBalanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    void clearNode(Node* node) {
        if (!node) return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

    void inorder(Node* node, int*& arr, int& index) const {
        if (!node) return;
        inorder(node->left, arr, index);
        arr[index++] = node->key;
        inorder(node->right, arr, index);
    }

    int getSize(Node* node) const {
        if (!node) return 0;
        return 1 + getSize(node->left) + getSize(node->right);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertNode(root, key);
    }

    void clear() {
        clearNode(root);
        root = nullptr;
    }

    int getSize() const {
        return getSize(root);
    }

    int* toArray(int &sz) const {
        sz = getSize(root);
        int* arr = new int[sz];
        int idx = 0;
        inorder(root, arr, idx);
        return arr;
    }
};

// Class representing a deck of cards for the game
class CardDeck {
private:
    std::map<int, int> cardCounts; // Map
    std::set<int> usedCards;       // Set
    std::stack<int> returnedCards; // Stack
    int deckArray[364];            // Array representing the complete deck

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
    AVLTree hand;
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
    // Sorts the player's hand
    void sortHand();
    // Displays the sorted hand
    void showSortedHand();
    std::string handToString() const;
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
    std::ofstream log;         // Log file for game results
    std::queue<Player> players;     // Queue
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
    void logResult(const std::string& result);
    void placeBet(float& bet);
    void handleResult(Player& player, Player& house, float& bet);
    void initializePlayers(int numPlayers);
    void printRules() const;
    void displayBalanceReport() const;
};

#endif // BLACKJACK_H
