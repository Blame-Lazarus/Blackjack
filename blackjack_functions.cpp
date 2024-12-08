/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 11/10/2024
 * Purpose: Blackjack Game Project
 */

#include "blackjack.h"
#include <iostream>
#include <algorithm> // for random_shuffle, sort
#include <ctime>
#include <cstring>   // strcpy and sprintf

using namespace std;

// Utility function for displaying cards using for_each
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
    // Initialize card lines with spaces and null terminators
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            cardLines[i][j] = ' ';
        }
        cardLines[i][6] = '\0'; // Null terminator
    }

    // Top and bottom borders
    strcpy(cardLines[0], "+-----+");
    strcpy(cardLines[5], "+-----+");

    // Map card number to rank
    char rankStr[3]; // Maximum of 2 characters plus null terminator
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
        // Single character rank
        cardLines[1][0] = '|';
        cardLines[1][1] = rankStr[0];
        cardLines[1][5] = '|';

        cardLines[4][0] = '|';
        cardLines[4][4] = rankStr[0];
        cardLines[4][5] = '|';
    } else if (strlen(rankStr) == 2) {
        // Two character rank, e.g., "10"
        cardLines[1][0] = '|';
        cardLines[1][1] = rankStr[0];
        cardLines[1][2] = rankStr[1];
        cardLines[1][5] = '|';

        cardLines[4][0] = '|';
        cardLines[4][3] = rankStr[0];
        cardLines[4][4] = rankStr[1];
        cardLines[4][5] = '|';
    }

    // Set the vertical borders
    cardLines[2][0] = '|';
    cardLines[2][5] = '|';
    cardLines[3][0] = '|';
    cardLines[3][5] = '|';

    // Ensure null terminators are set
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

// CardDeck class implementation

CardDeck::CardDeck() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    initializeDeck();
}

// Initialize the deck and reset card counts
void CardDeck::initializeDeck() {
    cardCounts.clear();
    for (int i = 1; i <= 13; i++) {
        cardCounts[i] = 28; // 28 cards of each rank in a 7-deck set
    }
    usedCards.clear();
    while (!returnedCards.empty()) {
        returnedCards.pop();
    }

    // Populate the deck array for shuffling
    int index = 0;
    for (int i = 1; i <= 13; ++i) {
        for (int j = 0; j < 28; ++j) {
            deckArray[index++] = i;
        }
    }
    shuffleDeck();
}

void CardDeck::shuffleDeck() {
    cout << "Shuffling the deck with std::random_shuffle..." << endl;
    random_shuffle(deckArray, deckArray + 364);
}

int CardDeck::drawCard() {
    if (needsReshuffling()) {
        reshuffleDeck();
    }

    int card;

    //do loop
    do {
        card = rand() % 13 + 1; // Random card between 1 and 13
    } while (cardCounts[card] == 0); // Ensure card is available

    cardCounts[card]--;
    usedCards.insert(card);
    return card; // Return the card number (1-13)
}

// Trigger to reshuffle
bool CardDeck::needsReshuffling() const {
    int totalCardsUsed = 0;
    for (const auto& p : cardCounts) {
        totalCardsUsed += (28 - p.second); // Calculate total used cards
    }
    return totalCardsUsed >= (52 * 7 * 3 / 4); // Check if reshuffling is needed (3/4 mark)
}

// Reshuffle deck
void CardDeck::reshuffleDeck() {
    cout << "Reshuffling the deck..." << endl;
    while (!returnedCards.empty()) {
        int card = returnedCards.top();
        cardCounts[card]++;
        returnedCards.pop();
    }
    initializeDeck();
    shuffleDeck(); // Shuffle the deck after reinitializing
}

// Return used card to stack
void CardDeck::returnCard(int card) {
    returnedCards.push(card); // Push returned card to discard stack
}

void CardDeck::printCardCounts() const {
    cout << "Current card counts:" << endl;
    for (const auto& pair : cardCounts) {
        cout << "Card " << pair.first << ": " << pair.second << endl;
    }
}

// display the deck status
void CardDeck::displayDeckStatus() const {
    cout << "Deck status:" << endl;
    cout << "Total unique cards used: " << usedCards.size() << endl;
}

// Player class implementation

Player::Player() : score(0) {}

// Deal cards to the player
void Player::addCard(int card) {
    int cardValue;
    if (card > 10) {
        cardValue = 10;
    } else if (card == 1) {
        cardValue = 11; // Initially count Ace as 11
    } else {
        cardValue = card;
    }
    hand.push_front(card);
    score = calculateScore();
}

// Calculate score
int Player::calculateScore() {
    int total = 0, aces = 0;
    for (int card : hand) {
        int cardValue;
        if (card > 10) {
            cardValue = 10;
        } else if (card == 1) {
            cardValue = 11;
            aces++;
        } else {
            cardValue = card;
        }
        total += cardValue;
    }
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    return total;
}

// Check if player has blackjack
bool Player::hasBlackjack() {
    return score == 21;
}

// Display player's hand with card graphics
// Display the player's or house's hand with card graphics
void Player::showHand(bool hideFirstCard) {
    const int maxCards = 10;  // Maximum cards in hand
    char allCardLines[maxCards][6][7]; // Array to store card graphics
    int numCards = 0;

    // Generate graphics for each card
    slist<int>::iterator it = hand.begin();
    for (int i = 0; it != hand.end(); ++it, ++numCards) {
        if (hideFirstCard && numCards == 0) {
            getHiddenCardGraphic(allCardLines[numCards]); // Use hidden card graphic
        } else {
            getCardGraphic(*it, allCardLines[numCards]); // Generate actual card graphic
        }
    }

    // Print the cards line by line
    for (int line = 0; line < 6; ++line) { // Each card has 6 lines
        for (int c = 0; c < numCards; ++c) {
            cout << allCardLines[c][line]; // Print the current line of the current card
            if (c < numCards - 1) {
                cout << " "; // Add a space between cards on the same line
            }
        }
        cout << endl; // Move to the next line after printing one row of all cards
    }

    // Print the total score if the first card is hidden
    if (hideFirstCard) {
        cout << "Total: ??" << endl; // Hide total score
    } else {
        cout << "Total: " << getScore() << endl;
    }
}


int Player::getScore() const {
    return score;
}

// Clear the hand
void Player::clearHand() {
    hand.clear();
    score = 0;
}

// Sort the hand for the player
void Player::sortHand() {
    int handSize = distance(hand.begin(), hand.end());
    int* handArray = new int[handSize];
    int index = 0;

    for (int card : hand) {
        handArray[index++] = card;
    }

    sort(handArray, handArray + handSize);

    hand.clear();
    for (int i = 0; i < handSize; ++i) {
        hand.push_front(handArray[handSize - i - 1]);
    }

    delete[] handArray;
}

// Display sorted hand with card graphics
void Player::showSortedHand() {
    // Max number of cards in hand
    const int maxHandSize = 10;
    char allCardLines[maxHandSize][6][7];
    int numCards = 0;

    // Iterate over the hand and generate card graphics
    slist<int>::iterator it;
    int cardIndex = 0;
    for (it = hand.begin(); it != hand.end(); ++it) {
        // Generate card graphic
        getCardGraphic(*it, allCardLines[cardIndex]);
        cardIndex++;
    }
    numCards = cardIndex;

    // Now, print the cards line by line
    for (int line = 0; line < 6; ++line) {
        for (int c = 0; c < numCards; ++c) {
            cout << allCardLines[c][line] << " ";
        }
        cout << endl;
    }
    // Print total score
    cout << "Total: " << getScore() << endl;
}

string Player::handToString() const {
    string result;
    for (int card : hand) {
        result += to_string(card) + " ";
    }
    return result;
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

// Display game information
void GameStatistics::displayStatistics() const {
    cout << "Game Statistics:" << endl;
    cout << "Total games played: " << totalGames << endl;
    cout << "Player wins: " << playerWins << " (" << fixed << setprecision(2)
         << (static_cast<float>(playerWins) / totalGames * 100) << "%)" << endl;
    cout << "House wins: " << houseWins << " (" << fixed << setprecision(2)
         << (static_cast<float>(houseWins) / totalGames * 100) << "%)" << endl;
    cout << "Ties: " << ties << " (" << fixed << setprecision(2)
         << (static_cast<float>(ties) / totalGames * 100) << "%)" << endl;
}

// BlackjackGame class

BlackjackGame::BlackjackGame() : balance(100.0), initialBalance(100.0), historyCount(0) {
    gameHistory = new int[100]; // Array to store game outcomes
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

    // prompt for number of players
    cout << "Enter the number of players (1-3): ";
    cin >> numPlayers;
    if (numPlayers < 1 || numPlayers > 3) {
        // game will switch to default 1 player
        cout << "Invalid number of players. Starting with 1 player." << endl;
        numPlayers = 1;
    }

    //initialize game with number of players
    initializePlayers(numPlayers);

    // Game cycle
    while (playing) {
        float bet;
        placeBet(bet);

        // Deal cards to players
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

        //Deal cards to house
        Player house;
        house.addCard(deck.drawCard());
        house.addCard(deck.drawCard());

        cout << "House's ";
        house.showHand(true);

        for (int i = 0; i < numPlayers; ++i) {
            Player& player = players.front();
            bool playerBust = false;
            char choice;

            // Array for card graphics
            char allCardLines[1][6][7]; // Adjusted to hold a single card graphic

            // Ask player for choice
            while (!playerBust) {
                cout << "Player " << i + 1 << ", hit or stand? (h/s): ";
                cin >> choice;

                if (choice == 'h') {
                    int card = deck.drawCard();
                    player.addCard(card);
                    cout << "Player " << i + 1 << " dealt card: " << endl;

                    // Generate and print card graphic
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

        // Reveal the house hidden card
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

        // Display statistics after each round
        stats.displayStatistics();

        // Prompt for a new game
        cout << "Play again? (y/n): ";
        char playAgain;
        cin >> playAgain;
        if (playAgain == 'n' || playAgain == 'N') {
            playing = false;
        }
    }

    // Display game history
    displayHistory();
}

// Function to place bet
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

// Function to handle results
void BlackjackGame::handleResult(Player& player, Player& house, float& bet) {

    //Player bust case
    if (player.getScore() > 21) {
        cout << "Player busts! House wins." << endl;
        logResult("Player busts");
        gameHistory[historyCount++] = -1;
        stats.recordResult(-1);
    }

    // If not busted

    else if (house.getScore() > 21) {
        cout << "House busts! Player wins!" << endl;
        balance += bet * 2;
        logResult("House busts, player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    }

    // If player wins
    else if (player.getScore() > house.getScore()) {
        cout << "Player wins!" << endl;
        balance += bet * 2;
        logResult("Player wins");
        gameHistory[historyCount++] = 1;
        stats.recordResult(1);
    }

    // Tie case
    else if (player.getScore() == house.getScore()) {
        cout << "It's a tie!" << endl;
        balance += bet;
        logResult("Tie");
        gameHistory[historyCount++] = 0;
        stats.recordResult(0);
    }

    // House wins

    else {
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

// This function saves the results of the game into a log
void BlackjackGame::logResult(const string& result) {
    if (log.is_open()) {
        log << "Result: " << result << ", Balance: $" << fixed << setprecision(2) << balance << endl;
    } else {
        cerr << "Error: Log file is not open." << endl;
    }
}

// Detailed log
void BlackjackGame::logDetailedState() {
    if (log.is_open()) {
        log << "Detailed game state: " << endl;
        queue<Player> tempPlayers = players; // Create a copy of the queue to iterate over

        while (!tempPlayers.empty()) {
            Player player = tempPlayers.front();
            log << "Player hand: " << player.handToString() << " Score: " << player.getScore() << endl;
            tempPlayers.pop(); // Remove the front element after processing
        }

        log << "=====================================" << endl;
    }
}

// Rules of the game
void BlackjackGame::printRules() const {
    cout << "Game Rules:" << endl;
    cout << "- Try to beat the house by getting as close to 21 as possible without going over." << endl;
    cout << "- Aces count as 1 or 11, face cards as 10, and cards 2-10 are face value." << endl;
    cout << "- Place your bets before the round begins." << endl;
    cout << "- Up to 3 players can join each game." << endl;
}

// Money Balance report
void BlackjackGame::displayBalanceReport() const {
    cout << "Current balance report: $" << balance << endl;
    cout << "Initial balance: $" << initialBalance << endl;
    cout << "Net earnings: $" << balance - initialBalance << endl;
}
