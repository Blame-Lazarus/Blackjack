/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 11/10/2024
 * Purpose: Blackjack Game Project
 */

// blackjack_functions.cpp
#include "blackjack.h"
#include <iostream>
#include <algorithm> // for sort
#include <ctime>
#include <cstring>   // strcpy and sprintf

using namespace std;

// Utility function
void displayCard(int card) {
    cout << card << " ";
}

void displayCardGraphic(int card) {
    char cardLines[6][7];
    getCardGraphic(card, cardLines); // Generate the card graphic
    for (int line = 0; line < 6; ++line) {
        cout << cardLines[line] << endl; // Print each line of the card
    }
}

// Function to generate the ASCII art for a card
void getCardGraphic(int card, char cardLines[6][7]) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            cardLines[i][j] = ' ';
        }
        cardLines[i][6] = '\0';
    }

    strcpy(cardLines[0], "+-----+");
    strcpy(cardLines[5], "+-----+");

    char rankStr[3];
    if (card == 1) {
        strcpy(rankStr, "A");
    } else if (card == 11) {
        strcpy(rankStr, "J");
    } else if (card == 12) {
        strcpy(rankStr, "Q");
    } else if (card == 13) {
        strcpy(rankStr, "K");
    } else {
        sprintf(rankStr, "%d", card);
    }

    if (strlen(rankStr) == 1) {
        cardLines[1][0] = '|';
        cardLines[1][1] = rankStr[0];
        cardLines[1][5] = '|';

        cardLines[4][0] = '|';
        cardLines[4][4] = rankStr[0];
        cardLines[4][5] = '|';
    } else if (strlen(rankStr) == 2) {
        cardLines[1][0] = '|';
        cardLines[1][1] = rankStr[0];
        cardLines[1][2] = rankStr[1];
        cardLines[1][5] = '|';

        cardLines[4][0] = '|';
        cardLines[4][3] = rankStr[0];
        cardLines[4][4] = rankStr[1];
        cardLines[4][5] = '|';
    }

    cardLines[2][0] = '|';
    cardLines[2][5] = '|';
    cardLines[3][0] = '|';
    cardLines[3][5] = '|';

    for (int i = 0; i < 6; ++i) {
        cardLines[i][6] = '\0';
    }
}

// Function to generate the ASCII art for a hidden card
void getHiddenCardGraphic(char cardLines[6][7]) {
    strcpy(cardLines[0], "+-----+");
    strcpy(cardLines[1], "|#####|");
    strcpy(cardLines[2], "|#####|");
    strcpy(cardLines[3], "|#####|");
    strcpy(cardLines[4], "|#####|");
    strcpy(cardLines[5], "+-----+");
}

// Player implementation

Player::Player() : score(0) {}

void Player::addCard(int card) {
    hand.insert(card);
    score = calculateScore();
}

int recursiveScore(int* arr, int size, int &aces) {
    if (size == 0) return 0;
    int card = arr[0];
    int cardValue;
    if (card > 10) {
        cardValue = 10;
    } else if (card == 1) {
        cardValue = 11;
        aces++;
    } else {
        cardValue = card;
    }
    return cardValue + recursiveScore(arr+1, size-1, aces);
}

int Player::calculateScore() {
    int sz = 0;
    int* arr = hand.toArray(sz);
    int aces = 0;
    int total = recursiveScore(arr, sz, aces);
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    delete[] arr;
    return total;
}

bool Player::hasBlackjack() {
    return score == 21;
}

void Player::showHand(bool hideFirstCard) {
    int sz = 0;
    int* arr = hand.toArray(sz);

    char allCardLines[50][6][7];
    int numCards = sz;
    for (int i = 0; i < sz; i++) {
        if (hideFirstCard && i == 0) {
            getHiddenCardGraphic(allCardLines[i]);
        } else {
            getCardGraphic(arr[i], allCardLines[i]);
        }
    }

    for (int line = 0; line < 6; ++line) {
        for (int c = 0; c < numCards; ++c) {
            cout << allCardLines[c][line];
            if (c < numCards - 1) cout << " ";
        }
        cout << endl;
    }

    if (hideFirstCard) {
        cout << "Total: ??" << endl;
    } else {
        cout << "Total: " << getScore() << endl;
    }

    delete[] arr;
}

int Player::getScore() const {
    return score;
}

void Player::clearHand() {
    hand.clear();
    score = 0;
}

void Player::showSortedHand() {
    int sz = 0;
    int* arr = hand.toArray(sz);

    char allCardLines[50][6][7];
    int numCards = sz;
    for (int i = 0; i < sz; i++) {
        getCardGraphic(arr[i], allCardLines[i]);
    }

    for (int line = 0; line < 6; ++line) {
        for (int c = 0; c < numCards; ++c) {
            cout << allCardLines[c][line];
            if (c < numCards - 1) cout << " ";
        }
        cout << endl;
    }

    cout << "Total: " << getScore() << endl;

    delete[] arr;
}

std::string Player::handToString() const {
    int sz = 0;
    int* arr = (const_cast<AVLTree*>(&hand))->toArray(sz);
    string result;
    for (int i = 0; i < sz; i++) {
        result += std::to_string(arr[i]) + " ";
    }
    delete[] arr;
    return result;
}
void Player::sortHand() {
    // Deleted
}


// Statistics

GameStatistics::GameStatistics() : totalGames(0), playerWins(0), houseWins(0), ties(0) {}

void GameStatistics::recordResult(int result) {
    totalGames++;
    if (result == 1) {
        playerWins++;
    } else if (result == -1) {
        houseWins++;
    } else {
        ties++;
    }
}

void GameStatistics::displayStatistics() const {
    cout << "Game Statistics:" << endl;
    cout << "Total games played: " << totalGames << endl;
    cout << "Player wins: " << playerWins << " (" << fixed << setprecision(2)
         << (totalGames > 0 ? (static_cast<float>(playerWins) / totalGames * 100) : 0) << "%)" << endl;
    cout << "House wins: " << houseWins << " (" << fixed << setprecision(2)
         << (totalGames > 0 ? (static_cast<float>(houseWins) / totalGames * 100) : 0) << "%)" << endl;
    cout << "Ties: " << ties << " (" << fixed << setprecision(2)
         << (totalGames > 0 ? (static_cast<float>(ties) / totalGames * 100) : 0) << "%)" << endl;
}

// BlackjackGame class

BlackjackGame::BlackjackGame() : balance(100.0), initialBalance(100.0), historyCount(0) {
    gameHistory = new int[100];
    log.open("game_log.txt", ios::app);
}

BlackjackGame::~BlackjackGame() {
    delete[] gameHistory;
    log.close();
}

void BlackjackGame::initializePlayers(int numPlayers) {
    for (int i = 0; i < numPlayers; ++i) {
        Player newPlayer;
        players.push(newPlayer);
    }
}

void BlackjackGame::playGame() {
    bool playing = true;
    int numPlayers;

    cout << "Enter the number of players (1-3): ";
    cin >> numPlayers;
    if (numPlayers < 1 || numPlayers > 3) {
        cout << "Invalid number of players. Starting with 1 player." << endl;
        numPlayers = 1;
    }

    initializePlayers(numPlayers);

    while (playing) {
        float bet;
        placeBet(bet);

        for (int i = 0; i < numPlayers; ++i) {
            Player& player = players.front();
            player.clearHand();
            player.addCard(deck.drawCard());
            player.addCard(deck.drawCard());
            cout << "Player " << i + 1 << "'s initial hand:" << endl;
            player.showHand();
            player.sortHand();
            cout << "Player " << i + 1 << "'s sorted hand:" << endl;
            player.showSortedHand();
            players.push(player);
            players.pop();
        }

        Player house;
        house.addCard(deck.drawCard());
        house.addCard(deck.drawCard());

        cout << "House's ";
        house.showHand(true);

        for (int i = 0; i < numPlayers; ++i) {
            Player& player = players.front();
            bool playerBust = false;
            char choice;

            char allCardLines[1][6][7];

            while (!playerBust) {
                cout << "Player " << i + 1 << ", hit or stand? (h/s): ";
                cin >> choice;

                if (choice == 'h') {
                    int card = deck.drawCard();
                    player.addCard(card);
                    cout << "Player " << i + 1 << " dealt card: " << endl;
                    getCardGraphic(card, allCardLines[0]);
                    for (int line = 0; line < 6; ++line) {
                        cout << allCardLines[0][line] << endl;
                    }

                    player.showHand();
                    player.sortHand();
                    cout << "Player " << i + 1 << "'s sorted hand:" << endl;
                    player.showSortedHand();

                    if (player.getScore() > 21) {
                        cout << "Player " << i + 1 << " busts! Total: " << player.getScore() << endl;
                        playerBust = true;
                        deck.returnCard(card);
                    }
                } else if (choice == 's') {
                    break;
                } else {
                    cout << "Invalid choice. Please enter 'h' or 's'." << endl;
                }
            }
            players.push(player);
            players.pop();
        }

        cout << "House reveals second card." << endl;
        house.showHand();
        while (house.getScore() < 17) {
            int card = deck.drawCard();
            house.addCard(card);
            cout << "House dealt card: " << endl;
            char cardLines[6][7];
            getCardGraphic(card, cardLines);
            for (int line = 0; line < 6; ++line) {
                cout << cardLines[line] << endl;
            }
            house.showHand();
        }

        queue<Player> finalPlayers = players;
        while (!finalPlayers.empty()) {
            Player& player = finalPlayers.front();
            handleResult(player, house, bet);
            finalPlayers.pop();
        }

        stats.displayStatistics();

        cout << "Play again? (y/n): ";
        char playAgain;
        cin >> playAgain;
        if (playAgain == 'n' || playAgain == 'N') {
            playing = false;
        }
    }

    displayHistory();
}

void BlackjackGame::placeBet(float& bet) {
    cout << "Current balance: $" << fixed << setprecision(2) << balance << endl;
    cout << "Place your bet: ";
    cin >> bet;
    while (bet < 5 || bet > balance) {
        cout << "Invalid bet. Enter a valid amount: ";
        cin >> bet;
    }
    balance -= bet;
}

void BlackjackGame::handleResult(Player& player, Player& house, float& bet) {
    if (player.getScore() > 21) {
        cout << "Player busts! House wins." << endl;
        logResult("Player busts");
        gameHistory[historyCount++] = -1;
        stats.recordResult(-1);
    } else if (house.getScore() > 21) {
        cout << "House busts! Player wins!" << endl;
        balance += bet * 2;
        logResult("House busts, player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    } else if (player.getScore() > house.getScore()) {
        cout << "Player wins!" << endl;
        balance += bet * 2;
        logResult("Player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    } else if (player.getScore() == house.getScore()) {
        cout << "It's a tie!" << endl;
        balance += bet;
        logResult("Tie");
        gameHistory[historyCount++] = 0;
        stats.recordResult(0);
    } else {
        cout << "House wins." << endl;
        logResult("House wins");
        gameHistory[historyCount++] = -1;
        stats.recordResult(-1);
    }
}

void BlackjackGame::displayHistory() const {
    cout << "Game History:" << endl;
    for (int i = 0; i < historyCount; ++i) {
        cout << "Game " << i + 1 << ": ";
        if (gameHistory[i] == 1) {
            cout << "Player wins" << endl;
        } else if (gameHistory[i] == -1) {
            cout << "House wins" << endl;
        } else {
            cout << "Tie" << endl;
        }
    }
}

void BlackjackGame::logResult(const std::string& result) {
    if (log.is_open()) {
        log << "Result: " << result << ", Balance: $" << fixed << setprecision(2) << balance << endl;
    } else {
        cerr << "Error: Log file is not open." << endl;
    }
}

void BlackjackGame::logDetailedState() {
    if (log.is_open()) {
        log << "Detailed game state: " << endl;
        queue<Player> tempPlayers = players;

        while (!tempPlayers.empty()) {
            Player player = tempPlayers.front();
            log << "Player hand: " << player.handToString() << " Score: " << player.getScore() << endl;
            tempPlayers.pop();
        }

        log << "=====================================" << endl;
    }
}

void BlackjackGame::printRules() const {
    cout << "Game Rules:" << endl;
    cout << "- Try to beat the house by getting as close to 21 as possible without going over." << endl;
    cout << "- Aces count as 1 or 11, face cards as 10, and cards 2-10 are face value." << endl;
    cout << "- Place your bets before the round begins." << endl;
    cout << "- Up to 3 players can join each game." << endl;
}

void BlackjackGame::displayBalanceReport() const {
    cout << "Current balance report: $" << balance << endl;
    cout << "Initial balance: $" << initialBalance << endl;
    cout << "Net earnings: $" << balance - initialBalance << endl;
}
