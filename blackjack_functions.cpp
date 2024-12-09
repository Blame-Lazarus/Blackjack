/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 11/10/2024
 * Purpose: Blackjack Game Project
 */

#include "blackjack.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstring>

using namespace std;

void displayCard(int card) {
    cout << card << " ";
}

void displayCardGraphic(int card) {
    char cardLines[6][7];
    getCardGraphic(card, cardLines);
    for (int line = 0; line < 6; ++line) {
        cout << cardLines[line] << endl;
    }
}

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

void getHiddenCardGraphic(char cardLines[6][7]) {
    strcpy(cardLines[0], "+-----+");
    strcpy(cardLines[1], "|#####|");
    strcpy(cardLines[2], "|#####|");
    strcpy(cardLines[3], "|#####|");
    strcpy(cardLines[4], "|#####|");
    strcpy(cardLines[5], "+-----+");
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

// Merge sort
static void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

static void mergeSort(int* arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

// Player implementation
Player::Player() {
    score[0] = 0;
    score[1] = 0;
    activeHands[0] = true;
    activeHands[1] = false;
    doubledDown[0] = false;
    doubledDown[1] = false;
    numberOfHands = 1;
}

void Player::addCard(int card, int handIndex) {
    hand[handIndex].insert(card);
    score[handIndex] = calculateScore(handIndex);
}

int Player::calculateScore(int handIndex) {
    int sz = 0;
    int* arr = hand[handIndex].toArray(sz);
    int aces = 0;
    int total = recursiveScore(arr, sz, aces);
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    delete[] arr;
    return total;
}

bool Player::hasBlackjack(int handIndex) {
    return score[handIndex] == 21;
}

void Player::showHand(bool hideFirstCard, int handIndex) {
    int sz = 0;
    int* arr = hand[handIndex].toArray(sz);

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
        cout << "Total: " << getScore(handIndex) << endl;
    }

    delete[] arr;
}

int Player::getScore(int handIndex) const {
    return score[handIndex];
}

void Player::clearHand() {
    hand[0].clear();
    hand[1].clear();
    score[0] = 0;
    score[1] = 0;
    activeHands[0] = true;
    activeHands[1] = false;
    doubledDown[0] = false;
    doubledDown[1] = false;
    numberOfHands = 1;
}

void Player::sortHand(int handIndex) {
    int sz = 0;
    int* arr = hand[handIndex].toArray(sz);
    mergeSort(arr, 0, sz - 1);
    hand[handIndex].clear();
    for (int i = 0; i < sz; i++) {
        hand[handIndex].insert(arr[i]);
    }
    delete[] arr;
}

void Player::showSortedHand(int handIndex) {
    int sz = 0;
    int* arr = hand[handIndex].toArray(sz);

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

    cout << "Total: " << getScore(handIndex) << endl;

    delete[] arr;
}

std::string Player::handToString(int handIndex) const {
    int sz = 0;
    int* arr = getHandArray(handIndex, sz);
    string result;
    for (int i = 0; i < sz; i++) {
        result += std::to_string(arr[i]) + " ";
    }
    delete[] arr;
    return result;
}

int Player::getNumberOfHands() const {
    return numberOfHands;
}

void Player::setNumberOfHands(int n) {
    numberOfHands = n;
}

bool Player::canSplit(int handIndex) {
    int sz = 0;
    int* arr = hand[handIndex].toArray(sz);
    bool result = false;
    if (sz == 2 && arr[0] == arr[1]) {
        result = true;
    }
    delete[] arr;
    return result;
}

void Player::splitHand() {
    if (numberOfHands < 2 && canSplit(0)) {
        int sz = 0;
        int* arr = hand[0].toArray(sz);
        hand[0].clear();
        hand[1].clear();
        hand[0].insert(arr[0]);
        hand[1].insert(arr[1]);
        activeHands[1] = true;
        numberOfHands = 2;
        score[0] = calculateScore(0);
        score[1] = calculateScore(1);
        delete[] arr;
    }
}

void Player::setDoubledDown(int handIndex, bool value) {
    doubledDown[handIndex] = value;
}

bool Player::isDoubledDown(int handIndex) const {
    return doubledDown[handIndex];
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

void BlackjackGame::printRules() const {
    cout << "Game Rules:" << endl;
    cout << "- Try to beat the house by getting as close to 21 as possible without going over." << endl;
    cout << "- Aces count as 1 or 11, face cards as 10, and cards 2-10 are face value." << endl;
    cout << "- You can double down on your initial two-card hand (double bet, one card only)." << endl;
    cout << "- You can split if your first two cards have the same rank, forming two hands." << endl;
    cout << "- The house draws until it has at least 17." << endl;
}

void BlackjackGame::displayBalanceReport() const {
    cout << "Current balance report: $" << balance << endl;
    cout << "Initial balance: $" << initialBalance << endl;
    cout << "Net earnings: $" << balance - initialBalance << endl;
}

void BlackjackGame::logDetailedState() {
    if (log.is_open()) {
        log << "Detailed game state: " << endl;
        queue<Player> tempPlayers = players;
        int pIndex = 1;
        while (!tempPlayers.empty()) {
            Player player = tempPlayers.front();
            for (int h = 0; h < player.getNumberOfHands(); h++) {
                log << "Player " << pIndex << " hand " << h+1 << ": " << player.handToString(h)
                    << " Score: " << player.getScore(h) << endl;
            }
            pIndex++;
            tempPlayers.pop();
        }

        log << "=====================================" << endl;
    }
}

void BlackjackGame::placeBet(float& bet) {
    cout << "Current balance: $" << fixed << setprecision(2) << balance << endl;
    cout << "Place your bet: ";
    cin >> bet;
    while (bet < 5 || bet > balance) {
        cout << "Invalid bet. Enter a valid amount (min $5, max your balance): ";
        cin >> bet;
    }
    balance -= bet;
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

        // Initial deal
        for (int i = 0; i < numPlayers; ++i) {
            Player& player = players.front();
            player.clearHand();
            player.addCard(deck.drawCard());
            player.addCard(deck.drawCard());
            cout << "Player " << i + 1 << "'s initial hand:" << endl;
            player.showHand(false,0);
            player.sortHand(0);
            cout << "Player " << i + 1 << "'s sorted hand:" << endl;
            player.showSortedHand(0);
            players.push(player);
            players.pop();
        }

        Player house;
        house.addCard(deck.drawCard());
        house.addCard(deck.drawCard());
        cout << "House's ";
        house.showHand(true,0);

        // Player decisions
        queue<Player> tempQueue;
        for (int i = 0; i < numPlayers; i++) {
            Player player = players.front();
            players.pop();

            bool doneWithHands = false;
            int currentHand = 0;
            while (!doneWithHands) {
                if (player.getNumberOfHands() == 2 && currentHand >= 2) {
                    doneWithHands = true;
                    break;
                }
                int hIndex = currentHand;

                bool canSplit = false;
                bool canDouble = false;

                canSplit = (player.getNumberOfHands() < 2 && player.canSplit(hIndex));

                int sz;
                int* arr = player.getHandArray(hIndex, sz);
                
                // Calculate total and aces to decide on double down conditions
                int acesCount = 0;
                int totalVal = 0;
                for (int idx = 0; idx < sz; idx++) {
                    int val = arr[idx];
                    if (val == 1) {
                        acesCount++;
                        val = 11;
                    } else if (val > 10) {
                        val = 10;
                    }
                    totalVal += val;
                }
                while (totalVal > 21 && acesCount > 0) {
                    totalVal -= 10;
                    acesCount--;
                }

                // Double down conditions:
                // Double if total 9,10,11 with no ace
                // or total 16,17,18 with an ace
                bool allowDouble = false;
                if (sz == 2 && !player.isDoubledDown(hIndex)) {
                    if ((acesCount == 0 && (totalVal == 9 || totalVal == 10 || totalVal == 11)) ||
                        (acesCount > 0 && (totalVal == 16 || totalVal == 17 || totalVal == 18))) {
                        allowDouble = true;
                    }
                }

                delete[] arr;
                canDouble = allowDouble;

                DecisionNode* head = DecisionTree::buildPlayerDecisionTree(canSplit, canDouble);

                bool turnOver = false;
                while (!turnOver && player.getScore(hIndex) <= 21) {
                    cout << "Player's hand " << (hIndex+1) << ":" << endl;
                    player.showHand(false,hIndex);
                    cout << "Available actions:" << endl;

                    DecisionNode* temp = head;
                    int actionCount = 0;
                    while (temp) {
                        actionCount++;
                        cout << actionCount << ". ";
                        if (temp->action == ACTION_HIT) cout << "Hit" << endl;
                        else if (temp->action == ACTION_STAND) cout << "Stand" << endl;
                        else if (temp->action == ACTION_DOUBLE) cout << "Double Down" << endl;
                        else if (temp->action == ACTION_SPLIT) cout << "Split" << endl;
                        temp = temp->next;
                    }

                    cout << "Choose an action (1-" << actionCount << "): ";
                    int choice;
                    cin >> choice;
                    if (choice < 1 || choice > actionCount) {
                        cout << "Invalid choice. Try again." << endl;
                        continue;
                    }

                    temp = head;
                    for (int c = 1; c < choice; c++) {
                        temp = temp->next;
                    }

                    ActionType chosenAction = temp->action;
                    if (chosenAction == ACTION_HIT) {
                        int card = deck.drawCard();
                        player.addCard(card,hIndex);
                        cout << "Dealt card:" << endl;
                        char cardLines[6][7];
                        getCardGraphic(card, cardLines);
                        for (int line = 0; line < 6; ++line) {
                            cout << cardLines[line] << endl;
                        }
                        player.showHand(false,hIndex);
                        player.sortHand(hIndex);
                        cout << "Sorted hand:" << endl;
                        player.showSortedHand(hIndex);
                        if (player.getScore(hIndex) > 21) {
                            cout << "Player busts this hand!" << endl;
                            turnOver = true;
                        }
                    } else if (chosenAction == ACTION_STAND) {
                        turnOver = true;
                    } else if (chosenAction == ACTION_DOUBLE) {
                        if (balance >= bet) {
                            balance -= bet;
                            bet = bet * 2;
                            player.setDoubledDown(hIndex,true);
                            cout << "Doubling down! New bet: $" << bet << endl;
                            int card = deck.drawCard();
                            player.addCard(card,hIndex);
                            cout << "Dealt card:" << endl;
                            char cardLines[6][7];
                            getCardGraphic(card, cardLines);
                            for (int line = 0; line < 6; ++line) {
                                cout << cardLines[line] << endl;
                            }
                            player.showHand(false,hIndex);
                            player.sortHand(hIndex);
                            cout << "Sorted hand:" << endl;
                            player.showSortedHand(hIndex);
                            turnOver = true;
                        } else {
                            cout << "Not enough balance to double down! Action not taken." << endl;
                        }
                    } else if (chosenAction == ACTION_SPLIT) {
                        player.splitHand();
                        cout << "Player splits the hand into two hands!" << endl;
                        turnOver = true;
                    }
                }

                DecisionTree::destroyTree(head);
                currentHand++;
                if (player.getNumberOfHands() == 1 && currentHand == 1) {
                    doneWithHands = true;
                }
                else if (player.getNumberOfHands() == 2 && currentHand == 2) {
                    doneWithHands = true;
                }
            }
            tempQueue.push(player);
        }

        players = tempQueue;

        cout << "House reveals second card." << endl;
        house.showHand(false,0);

        bool houseTurn = true;
        while (houseTurn && house.getScore(0) < 21) {
            DecisionNode* hTree = DecisionTree::buildHouseDecisionTree(house.getScore(0));
            if (hTree->action == ACTION_HIT) {
                int card = deck.drawCard();
                house.addCard(card,0);
                cout << "House dealt card:" << endl;
                char cardLines[6][7];
                getCardGraphic(card, cardLines);
                for (int line = 0; line < 6; ++line) {
                    cout << cardLines[line] << endl;
                }
                house.showHand(false,0);
            } else {
                houseTurn = false;
            }
            DecisionTree::destroyTree(hTree);
            if (house.getScore(0) >= 17) houseTurn = false;
        }

        queue<Player> finalPlayers = players;
        while (!finalPlayers.empty()) {
            Player player = finalPlayers.front();
            finalPlayers.pop();
            for (int h = 0; h < player.getNumberOfHands(); h++) {
                handleResult(player, house, bet, h);
            }
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

void BlackjackGame::handleResult(Player& player, Player& house, float& bet, int handIndex) {
    int pScore = player.getScore(handIndex);
    int hScore = house.getScore(0);
    if (pScore > 21) {
        cout << "Player busts! House wins." << endl;
        logResult("Player busts");
        gameHistory[historyCount++] = -1;
        stats.recordResult(-1);
    } else if (hScore > 21) {
        cout << "House busts! Player wins this hand!" << endl;
        float winAmount = bet*2;
        cout << "Player wins $" << winAmount << endl;
        balance += winAmount;
        logResult("House busts, player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    } else if (pScore > hScore) {
        cout << "Player wins this hand!" << endl;
        float winAmount = bet*2;
        cout << "Player wins $" << winAmount << endl;
        balance += winAmount;
        logResult("Player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    } else if (pScore == hScore) {
        cout << "It's a tie! House wins ties." << endl;
        logResult("Tie goes to dealer");
        gameHistory[historyCount++] = -1;
        stats.recordResult(-1);
    } else {
        cout << "House wins this hand." << endl;
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
