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
#include <algorithm>
#include <cstdlib>
#include <ctime>     // Time
#include <cstring>   // strcpy
#include <iostream>
#include <string>

using namespace std;

// Utility functions
void getCardGraphic(int card, char cardLines[6][7]);
void getHiddenCardGraphic(char cardLines[6][7]);

// AVL Tree for player's hand
class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root; // Root of the tree

    //Node inserting
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

    // Height update
    void updateHeight(Node* node) {
        int hl = (node->left ? node->left->height : 0);
        int hr = (node->right ? node->right->height : 0);
        node->height = (hl > hr ? hl : hr) + 1;
    }

    // Balancing factor
    int getBalanceFactor(Node* node) {
        if (!node) return 0;
        int hl = (node->left ? node->left->height : 0);
        int hr = (node->right ? node->right->height : 0);
        return hl - hr;
    }

    // Rotation right
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }
    
    // Rotation left
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

    // Clear node
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
            
    // More member functions
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

// Class for deck of cards
class CardDeck {
private:
    std::map<int, int> cardCounts; // Map
    std::set<int> usedCards;       // Set
    std::stack<int> returnedCards; // Stack
    int deckArray[364];            // Array representing the complete deck

    void initializeDeck() {
        cardCounts.clear();
        for (int i = 1; i <= 13; i++) {
            cardCounts[i] = 28; // 28 cards of each rank in a 7-deck set
        }
        while (!returnedCards.empty()) {
            returnedCards.pop();
        }
        usedCards.clear();

        int index = 0;
        for (int i = 1; i <= 13; ++i) {
            for (int j = 0; j < 28; ++j) {
                deckArray[index++] = i;
            }
        }
        // Recursive shuffle function call
        recursiveShuffleDeck(deckArray, 364);
    }

    // Recursive shuffle function
    void recursiveShuffleDeck(int arr[], int n) {
        if (n <= 1) return;
        int r = rand() % n;
        int temp = arr[r];
        arr[r] = arr[n - 1];
        arr[n - 1] = temp;
        recursiveShuffleDeck(arr, n - 1);
    }

    void reshuffleDeck() {
        while (!returnedCards.empty()) {
            int card = returnedCards.top();
            cardCounts[card]++;
            returnedCards.pop();
        }
        initializeDeck();
    }

public:
    CardDeck() {
        srand(static_cast<unsigned int>(time(0)));
        initializeDeck();
    }

    void shuffleDeck() {
        recursiveShuffleDeck(deckArray, 364);
        cout << "Shuffling the deck with recursion..." << endl;
    }

    // Card drawing
    int drawCard() {
        if (needsReshuffling()) {
            cout << "Reshuffling the deck..." << endl;
            reshuffleDeck();
        }
        int card;
        do {
            card = deckArray[rand() % 364]; 
        } while (cardCounts[card] == 0); 
        cardCounts[card]--;
        usedCards.insert(card);
        return card;
    }

    // Function which checks the total number of cards used to determine if the deck needs reshuffling
    bool needsReshuffling() const {
        int totalCardsUsed = 0;
        for (std::map<int,int>::const_iterator it = cardCounts.begin(); it != cardCounts.end(); ++it) {
            totalCardsUsed += (28 - it->second);
        }
        return totalCardsUsed >= (52 * 7 * 3 / 4);
    }

    void returnCard(int card) {
        returnedCards.push(card);
    }

    void printCardCounts() const {
        std::map<int,int>::const_iterator it = cardCounts.begin();
        cout << "Current card counts:" << endl;
        for (; it != cardCounts.end(); ++it) {
            cout << "Card " << it->first << ": " << it->second << endl;
        }
    }

    void displayDeckStatus() const {
        cout << "Deck status:" << endl;
        cout << "Total unique cards used: " << (int)usedCards.size() << endl;
    }
};

// Player class
class Player {
private:
    AVLTree hand;
    int score;

public:
    Player();
    ~Player() = default;
    void addCard(int card);
    int calculateScore();
    bool hasBlackjack();
    void showHand(bool hideFirstCard = false);
    int getScore() const;
    void clearHand();
    void sortHand();
    void showSortedHand();
    std::string handToString() const;
};

// Game statistics class
class GameStatistics {
private:
    int totalGames;
    int playerWins;
    int houseWins;
    int ties;

public:
    GameStatistics();
    void recordResult(int result);
    void displayStatistics() const;
};

// Class managing game
class BlackjackGame {
private:
    float balance;
    float initialBalance;
    int* gameHistory;
    int historyCount;
    CardDeck deck;
    std::ofstream log;
    std::queue<Player> players;
    GameStatistics stats;

    void logDetailedState();

public:
    BlackjackGame();
    ~BlackjackGame();
    void playGame();
    void displayHistory() const;
    void logResult(const std::string& result);
    void placeBet(float& bet);
    void handleResult(Player& player, Player& house, float& bet);
    void initializePlayers(int numPlayers);
    void printRules() const;
    void displayBalanceReport() const;
};

#endif // BLACKJACK_H
