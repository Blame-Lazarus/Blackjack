/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 12/08/2024
 * Purpose: Blackjack Game Project
 */

// System Libraries
#include "blackjack.h"  // Header
#include <iostream>
#include <ctime>

using namespace std;

// Function prototypes
void displayWelcomeMessage();
void displayGameMenu();
void displayGoodbyeMessage();

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    //Welcome message
    displayWelcomeMessage();
    displayGameMenu();

    // Game
    BlackjackGame game;
    game.playGame();

    // Final message
    displayGoodbyeMessage();

    // Exit
    return 0;
}

// Function definitions

// Welcome message
void displayWelcomeMessage() {
    cout << "=========================================" << endl;
    cout << "        Welcome to the Blackjack Game!   " << endl;
    cout << "=========================================" << endl;
    cout << "Rules: The objective is to have a higher hand value than the dealer\n"
         << "without exceeding 21." << endl
         << "The dealer must hit if under 17. Ties go to the dealer.\n"
         << "Up to three players are allowed.\n"
         << "=========================================" << endl << endl;
}

// Game menu
void displayGameMenu() {
    cout << "Game Options:" << endl;
    cout << "1. Start a new game" << endl;
    cout << "2. View game rules" << endl;
    cout << "3. Exit the game" << endl;
    cout << "Please enter your choice (1-3): ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Starting a new game..." << endl << endl;
            break;
        case 2:
            // Rules
            cout << "=========================================" << endl;
            cout << "              Game Rules                " << endl;
            cout << "=========================================" << endl;
            cout << "- Try to beat the house by getting as close to 21 as possible without going over.\n"
                 << "- Aces count as 1 or 11, face cards as 10, and cards 2-10 are face value.\n"
                 << "- Place your bets before each round.\n"
                 << "- You can double down on your initial two-card hand.\n"
                 << "- You can split if your initial two cards have the same rank, forming two separate hands.\n"
                 << "- The house draws until it has at least 17.\n"
                 << "=========================================" << endl << endl;
            break;
        case 3:
            cout << "Exiting the game. See you next time!" << endl;
            exit(0);
        default:
            // Default settings if choice is invalid
            cout << "Invalid choice. Starting a new game by default." << endl << endl;
            break;
    }
}

// Goodbye message
void displayGoodbyeMessage() {
    cout << "=========================================" << endl;
    cout << "      Thank you for playing Blackjack!   " << endl;
    cout << "=========================================" << endl;
}
