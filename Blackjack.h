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
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
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

// Class for deck of cards
class CardDeck {
private:
    std::map<int, int> cardCounts;
    std::set<int> usedCards;
    std::stack<int> returnedCards;
    int deckArray[364];

    void initializeDeck() {
        cardCounts.clear();
        for (int i = 1; i <= 13; i++) {
            cardCounts[i] = 28;
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
        recursiveShuffleDeck(deckArray, 364);
    }

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
    AVLTree hand[2];
    int score[2];
    bool activeHands[2];
    bool doubledDown[2]; 
    int numberOfHands;

public:
    int* getHandArray(int handIndex, int &sz) const {
        return hand[handIndex].toArray(sz);
    }

    Player();
    ~Player() = default;
    void addCard(int card, int handIndex=0);
    int calculateScore(int handIndex=0);
    bool hasBlackjack(int handIndex=0);
    void showHand(bool hideFirstCard = false, int handIndex=0);
    int getScore(int handIndex=0) const;
    void clearHand();
    void sortHand(int handIndex=0);
    void showSortedHand(int handIndex=0);
    std::string handToString(int handIndex=0) const;
    int getNumberOfHands() const;
    void setNumberOfHands(int n);
    bool canSplit(int handIndex=0);
    void splitHand();
    void setDoubledDown(int handIndex, bool value);
    bool isDoubledDown(int handIndex) const;
};

// Game statistics
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

// Decision Tree
enum ActionType {
    ACTION_STAND,
    ACTION_HIT,
    ACTION_DOUBLE,
    ACTION_SPLIT
};

struct DecisionNode {
    ActionType action;
    DecisionNode* next;
    DecisionNode(ActionType a) : action(a), next(nullptr) {}
};

class DecisionTree {
public:
    static DecisionNode* buildPlayerDecisionTree(bool canSplit, bool canDouble) {
        DecisionNode* head = nullptr;
        DecisionNode* tail = nullptr;

        // Hit
        {
            DecisionNode* node = new DecisionNode(ACTION_HIT);
            if (!head) head = node; else tail->next = node;
            tail = node;
        }
        // Stand
        {
            DecisionNode* node = new DecisionNode(ACTION_STAND);
            tail->next = node;
            tail = node;
        }
        if (canDouble) {
            DecisionNode* node = new DecisionNode(ACTION_DOUBLE);
            tail->next = node;
            tail = node;
        }
        if (canSplit) {
            DecisionNode* node = new DecisionNode(ACTION_SPLIT);
            tail->next = node;
            tail = node;
        }

        return head;
    }

    static DecisionNode* buildHouseDecisionTree(int houseScore) {
        DecisionNode* head = nullptr;
        DecisionNode* tail = nullptr;

        if (houseScore < 17) {
            DecisionNode* node = new DecisionNode(ACTION_HIT);
            head = node;
            tail = node;
        } else {
            DecisionNode* node = new DecisionNode(ACTION_STAND);
            if (!head) head = node; else tail->next = node;
            tail = node;
        }
        return head;
    }

    static void destroyTree(DecisionNode* head) {
        while (head) {
            DecisionNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Game class
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
    void handleResult(Player& player, Player& house, float& bet, int handIndex);
    void initializePlayers(int numPlayers);
    void printRules() const;
    void displayBalanceReport() const;
};

#endif // BLACKJACK_H
