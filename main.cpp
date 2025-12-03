#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <cctype>    // tolower, isalpha
using namespace std;

// Draw the hangman based on number of wrong guesses
void drawHangman(int wrongGuesses) {
    cout << " +----+" << endl;
    cout << " |    |" << endl;

    if (wrongGuesses >= 1) cout << " O    |" << endl;
    else                   cout << "      |" << endl;

    if (wrongGuesses == 2)       cout << " |    |" << endl;
    else if (wrongGuesses == 3)  cout << "/|    |" << endl;
    else if (wrongGuesses >= 4)  cout << "/|\\   |" << endl;
    else                         cout << "      |" << endl;

    if (wrongGuesses == 5)       cout << "/     |" << endl;
    else if (wrongGuesses >= 6)  cout << "/ \\   |" << endl;
    else                         cout << "      |" << endl;

    cout << "      |" << endl;
    cout << " ========" << endl;
    cout << endl;
}

// Choose a random word from the list
string chooseRandomWord(const vector<string>& words) {
    int index = rand() % words.size();
    return words[index];
}

// Check if a letter is already guessed
bool alreadyGuessed(char letter, const vector<char>& guessedLetters) {
    for (char c : guessedLetters) {
        if (c == letter) return true;
    }
    return false;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // ---- INTRO SCREEN ----
    cout << "==========================" << endl;
    cout << "        HANGMAN           " << endl;
    cout << "==========================" << endl;
    cout << "Welcome to the Hangman Game!" << endl;
    cout << "In this game, you will choose from 3 categories:" << endl;
    cout << "- Car Brands" << endl;
    cout << "- Sports" << endl;
    cout << "- Foods" << endl;
    cout << "Each game also lets you choose a difficulty:" << endl;
    cout << "- Easy   (8 lives)" << endl;
    cout << "- Medium (6 lives)" << endl;
    cout << "- Hard   (4 lives)" << endl;
    cout << "You also get 1 hint per game. Type '!' to use it." << endl;
    cout << endl;
    cout << "Are you ready to start? (y/n): ";

    char startChoice;
    cin >> startChoice;
    startChoice = static_cast<char>(tolower(startChoice));

    if (startChoice != 'y') {
        cout << "Okay! Come back when you're ready. Goodbye!" << endl;
        return 0;
    }

    // ---- Categories ----
    vector<string> carBrands = {
        "toyota", "honda", "ford", "chevrolet", "tesla",
        "bmw", "audi", "mercedes", "nissan"
    };

    vector<string> sports = {
        "baseball", "basketball", "soccer", "tennis", "volleyball",
        "swimming", "boxing", "football", "golf"
    };

    vector<string> foods = {
        "pizza", "hamburger", "pasta", "sandwich", "taco",
        "sushi", "salad", "pancake", "omelette"
    };

    bool playAgain = true;
    int wins = 0;
    int losses = 0;

    while (playAgain) {
        cout << endl;
        cout << "==========================" << endl;
        cout << "        NEW GAME          " << endl;
        cout << "==========================" << endl;

        // ---- Choose category ----
        cout << "Choose a category:" << endl;
        cout << "1. Car brands" << endl;
        cout << "2. Sports" << endl;
        cout << "3. Foods" << endl;

        int categoryChoice;
        while (true) {
            cout << "Enter 1, 2, or 3: ";
            cin >> categoryChoice;

            if (!cin) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Enter 1, 2, or 3." << endl;
                continue;
            }

            if (categoryChoice >= 1 && categoryChoice <= 3) break;
            cout << "Please choose 1, 2, or 3." << endl;
        }

        string secretWord;

        if (categoryChoice == 1) {
            cout << "\nCategory chosen: Car brands\n" << endl;
            secretWord = chooseRandomWord(carBrands);
        } else if (categoryChoice == 2) {
            cout << "\nCategory chosen: Sports\n" << endl;
            secretWord = chooseRandomWord(sports);
        } else {
            cout << "\nCategory chosen: Foods\n" << endl;
            secretWord = chooseRandomWord(foods);
        }

        // ---- Choose difficulty ----
        cout << "Choose difficulty:" << endl;
        cout << "1. Easy   (8 wrong guesses)" << endl;
        cout << "2. Medium (6 wrong guesses)" << endl;
        cout << "3. Hard   (4 wrong guesses)" << endl;

        int difficultyChoice;
        while (true) {
            cout << "Enter 1, 2, or 3: ";
            cin >> difficultyChoice;

            if (!cin) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Enter 1, 2, or 3." << endl;
                continue;
            }

            if (difficultyChoice >= 1 && difficultyChoice <= 3) break;
            cout << "Please choose 1, 2, or 3." << endl;
        }

        int maxWrong;
        if (difficultyChoice == 1) {
            maxWrong = 8;
            cout << "\nDifficulty: Easy\n" << endl;
        } else if (difficultyChoice == 2) {
            maxWrong = 6;
            cout << "\nDifficulty: Medium\n" << endl;
        } else {
            maxWrong = 4;
            cout << "\nDifficulty: Hard\n" << endl;
        }

        // ---- Setup ----
        string currentState(secretWord.size(), '_');
        vector<char> wrongLetters;
        vector<char> allGuessedLetters;

        int wrongGuesses = 0;
        bool wordGuessed = false;
        int hintsLeft = 1;   // 1 hint per game

        // ---- Game Loop ----
        while (wrongGuesses < maxWrong && !wordGuessed) {
            drawHangman(wrongGuesses);

            cout << "Word: ";
            for (char c : currentState) cout << c << ' ';
            cout << endl;

            cout << "Wrong letters: ";
            if (wrongLetters.empty()) cout << "(none)";
            else for (char c : wrongLetters) cout << c << ' ';
            cout << endl;

            cout << "Lives left: " << (maxWrong - wrongGuesses) << endl;
            cout << "Hints left: " << hintsLeft << endl;
            cout << "Enter a letter (or '!' for a hint): ";

            char guess;
            cin >> guess;
            guess = static_cast<char>(tolower(guess));

            // ---- Hint handling ----
            if (guess == '!') {
                if (hintsLeft <= 0) {
                    cout << "No hints left this game!" << endl;
                } else {
                    bool revealed = false;
                    for (size_t i = 0; i < secretWord.size(); ++i) {
                        if (currentState[i] == '_') {
                            char letter = secretWord[i];

                            // Reveal ALL occurrences of that letter
                            for (size_t j = 0; j < secretWord.size(); ++j) {
                                if (tolower(secretWord[j]) == tolower(letter)) {
                                    currentState[j] = secretWord[j];
                                }
                            }

                            // Add to guessed letters if not already there
                            if (!alreadyGuessed(tolower(letter), allGuessedLetters)) {
                                allGuessedLetters.push_back(tolower(letter));
                            }

                            hintsLeft--;
                            cout << "Hint used! The letter '" << letter
                                 << "' has been revealed." << endl;
                            revealed = true;
                            break;
                        }
                    }
                    if (!revealed) {
                        cout << "No letters left to reveal!" << endl;
                    }

                    if (currentState == secretWord) {
                        wordGuessed = true;
                    }
                }

                cout << endl;
                continue; // Skip normal letter handling
            }

            // Not a letter
            if (!isalpha(guess)) {
                cout << "Please enter a valid letter (a-z) or '!' for a hint." << endl;
                continue;
            }

            // Repeated letter check
            if (alreadyGuessed(guess, allGuessedLetters)) {
                cout << "⚠️  The letter '" << guess << "' has already been used!" << endl;
                cout << "Try another letter." << endl;
                continue;
            }
            allGuessedLetters.push_back(guess);

            bool found = false;
            for (size_t i = 0; i < secretWord.size(); ++i) {
                if (tolower(secretWord[i]) == guess) {
                    currentState[i] = secretWord[i];
                    found = true;
                }
            }

            if (found) {
                cout << "Good guess! '" << guess << "' is in the word." << endl;
            } else {
                cout << "Incorrect! '" << guess << "' is NOT in the word." << endl;
                wrongGuesses++;
                wrongLetters.push_back(guess);
            }

            if (currentState == secretWord) {
                wordGuessed = true;
            }

            cout << endl;
        }

        // ---- End Round ----
        drawHangman(wrongGuesses);

        if (wordGuessed) {
            cout << "🎉 You guessed the word: " << secretWord << "!" << endl;
            wins++;
        } else {
            cout << "💀 You lost! The word was: " << secretWord << endl;
            losses++;
        }

        cout << "Record -> Wins: " << wins
             << " | Losses: " << losses << endl;

        cout << "Play again? (y/n): ";
        char answer;
        cin >> answer;
        answer = static_cast<char>(tolower(answer));
        if (answer != 'y') {
            playAgain = false;
        }
    }

    // ---- Final Stats & Big Congrats ----
    cout << "\nFinal Record -> Wins: " << wins
         << " | Losses: " << losses << endl;

    if (wins > losses) {
        cout << endl;
        cout << "===============================" << endl;
        cout << "       C O N G R A T S !       " << endl;
        cout << " You finished with more wins!  " << endl;
        cout << "===============================" << endl;
        cout << endl;
    }

    cout << "Thanks for playing Hangman!" << endl;

    return 0;
}
