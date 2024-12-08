/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 11/10/2024
 * Purpose: Blackjack Game Project
 */

// System Libraries
#include "blackjack.h"
#include <iostream>
#include <ctime>

// namespace
using namespace std;

// Function declarations for utility display
void displayWelcomeMessage();
void displayGameMenu();
void displayGoodbyeMessage();

// Main function execution begins here!
int main() {
    // Set a random number seed
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    // Display welcome message and rules
    displayWelcomeMessage();

    // Display the game menu
    displayGameMenu();

    // Start the Blackjack game
    BlackjackGame game;
    game.playGame();

    // Display goodbye message
    displayGoodbyeMessage();

    // Finish game
    return 0;
}

// Function to display a welcome message
void displayWelcomeMessage() {
    cout << "=========================================" << endl;
    cout << "        Welcome to the Blackjack Game!   " << endl;
    cout << "=========================================" << endl;
    cout << "Rules: The objective is to have a higher hand value than the dealer\n"
         << "without exceeding 21. You are initially dealt two cards and can choose\n"
         << "to 'hit' (receive an additional card) or 'stand' (keep your current hand).\n"
         << "If your hand exceeds 21, you 'bust' and lose the game.\n"
         << "The dealer must hit if their total is under 17. Ties go to the dealer.\n"
         << "This game allows up to three players.\n"
         << "=========================================" << endl << endl;
}

// Function to display a game menu with options
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
            cout << "=========================================" << endl;
            cout << "              Game Rules                " << endl;
            cout << "=========================================" << endl;
            cout << "- Try to beat the house by getting as close to 21 as possible without going over." << endl;
            cout << "- Aces count as 1 or 11, face cards as 10, and cards 2-10 are face value." << endl;
            cout << "- Place your bets before each round begins." << endl;
            cout << "- Up to 3 players can join each game." << endl;
            cout << "=========================================" << endl << endl;
            break;
        case 3:
            cout << "Exiting the game. See you next time!" << endl;
            exit(0);
        default:
            cout << "Invalid choice. Starting a new game by default." << endl << endl;
            break;
    }
}

// Function to display a goodbye message
void displayGoodbyeMessage() {
    cout << "=========================================" << endl;
    cout << "      Thank you for playing Blackjack!   " << endl;
    cout << "=========================================" << endl;
    cout << "We hope you enjoyed the game. Goodbye!" << endl;
}