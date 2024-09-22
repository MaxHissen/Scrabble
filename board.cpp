
#include <iostream>  // This header file is needed for input and output operations
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <cmath>
#include <unordered_map>

const int MAXPLAYERS = 2;
const int BOARDSIZE = 15;
const char EMPTY = ' ';
const char DW = '-';
const char TW = '=';
const char DL = '\'';
const char TL = '"';

const std::array<char, 26> letters = {{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}};

struct Board{
    char cells[BOARDSIZE][BOARDSIZE];

    bool double_letter[BOARDSIZE][BOARDSIZE];
    bool triple_letter[BOARDSIZE][BOARDSIZE];

    bool double_word[BOARDSIZE][BOARDSIZE];
    bool triple_word[BOARDSIZE][BOARDSIZE];

    int scores[MAXPLAYERS]; //each player's score
    int turn; //who's turn is it

    std::unordered_map<char, int> values;


    void initialize() {
        //
        for (int i=0; i<MAXPLAYERS; i++){
            scores[i] = 0;
        }
        turn = 0;


        //assign board
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                cells[i][j] = EMPTY; // Initialize with space or any other default value
                double_letter[i][j] = false;
                triple_letter[i][j] = false;
                double_word[i][j] = false;
                triple_word[i][j] = false;
            }
        }

        //assign double and triple word values
        double_word[7][7] = true;

        triple_word[0][0] = true;
        double_word[1][1] = true;
        double_word[2][2] = true;
        double_word[3][3] = true;
        double_word[4][4] = true;
        triple_letter[5][5] = true;
        double_letter[6][6] = true;

        triple_word[14][0] = true;
        double_word[13][1] = true;
        double_word[12][2] = true;
        double_word[11][3] = true;
        double_word[10][4] = true;
        triple_letter[9][5] = true;
        double_letter[8][6] = true;

        triple_word[0][14] = true;
        double_word[1][13] = true;
        double_word[2][12] = true;
        double_word[3][11] = true;
        double_word[4][10] = true;
        triple_letter[5][9] = true;
        double_letter[6][8] = true;

        triple_word[14][14] = true;
        double_word[13][13] = true;
        double_word[12][12] = true;
        double_word[11][11] = true;
        double_word[10][10] = true;
        triple_letter[9][9] = true;
        double_letter[8][8] = true;

        triple_word[0][7] = true;
        triple_word[7][0] = true;
        triple_word[14][7] = true;
        triple_word[7][14] = true;

        double_letter[0][3] = true;
        double_letter[0][11] = true;
        double_letter[3][0] = true;
        double_letter[11][0] = true;
        double_letter[14][3] = true;
        double_letter[14][11] = true;
        double_letter[3][14] = true;
        double_letter[11][14] = true;

        triple_letter[1][5] = true;
        triple_letter[1][9] = true;
        triple_letter[5][1] = true;
        triple_letter[9][1] = true;
        triple_letter[13][5] = true;
        triple_letter[13][9] = true;
        triple_letter[5][13] = true;
        triple_letter[9][13] = true;

        double_letter[2][6] = true;
        double_letter[2][8] = true;
        double_letter[6][2] = true;
        double_letter[8][2] = true;
        double_letter[12][6] = true;
        double_letter[12][8] = true;
        double_letter[6][12] = true;
        double_letter[8][12] = true;

        double_letter[3][7] = true;
        double_letter[7][3] = true;
        double_letter[11][7] = true;
        double_letter[7][11] = true;


        //assing letter values
        values['A'] = 1;
        values['B'] = 3;
        values['C'] = 3;
        values['D'] = 2;
        values['E'] = 1;
        values['F'] = 4;
        values['G'] = 2;
        values['H'] = 4;
        values['I'] = 1;
        values['J'] = 8;
        values['K'] = 5;
        values['L'] = 1;
        values['M'] = 3;
        values['N'] = 1;
        values['O'] = 1;
        values['P'] = 3;
        values['Q'] = 10;
        values['R'] = 1;
        values['S'] = 1;
        values['T'] = 1;
        values['U'] = 1;
        values['V'] = 4;
        values['W'] = 4;
        values['X'] = 8;
        values['Y'] = 4;
        values['Z'] = 10;
    }

    // Method to print the board
    void print() const {
        std::cout << "PLAYER " << turn << '\n';
        std::cout << "SCORES:" << '\n';
        for (int i=0; i<MAXPLAYERS; i++){
            std::cout << "Player " << i << ": " << scores[i] << '\n';
        }

        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                int x = j;
                int y = BOARDSIZE - i - 1;
                if (cells[x][y] != EMPTY){
                    std::cout << cells[x][y] << ' ';
                }
                else if (double_letter[x][y]){
                    std::cout << DL << ' ';
                }
                else if (triple_letter[x][y]){
                    std::cout << TL << ' ';
                }
                else if (double_word[x][y]){
                    std::cout << DW << ' ';
                }
                else if (triple_word[x][y]){
                    std::cout << TW << ' ';
                }
                else{
                    std::cout << EMPTY << ' ';
                }
            }
            std::cout << '\n';
        }
    }

    //puts word on board
    //returns move score
    int write_word(int pos_x, int pos_y, bool vertical, std::string word) {

        int word_length = word.length();

        int total_score = 0;
        int start_x = pos_x;
        int start_y = pos_y;
        int letter_index = 0;
        for (;cells[start_x - (1-vertical)][start_y + vertical] != ' ' && start_x >= 0 && start_y < BOARDSIZE; letter_index--){
            start_x -= (1-vertical);
            start_y += vertical;
        }
        //start_x+=(1-vertical);
        //start_y-=vertical;

        std::cout << letter_index << '\n';


        int series_score = 0;
        int series_multiplication = 1;
        int total_parallel_score = 0;
        int parallel_multiplier = 1;
        while (start_x < BOARDSIZE && start_y >= 0 && (cells[start_x][start_y] != ' ' || letter_index < word_length)){
            std::cout << letter_index << '\n';
            if (cells[start_x][start_y] == ' '){
                //going to be part of word
                std::cout << "a" << '\n';
                parallel_multiplier = 1;
                int letter_score;

                if (double_word[start_x][start_y]){
                    series_multiplication *= 2;
                    parallel_multiplier = 2;
                    letter_score = values.find(word[letter_index])->second;
                }
                else if (triple_word[start_x][start_y]){
                    series_multiplication *= 3;
                    parallel_multiplier = 3;
                    letter_score = values.find(word[letter_index])->second;
                }
                else if (double_letter[start_x][start_y]){
                    letter_score = values.find(word[letter_index])->second * 2;
                }
                else if (triple_letter[start_x][start_y]){
                    letter_score = values.find(word[letter_index])->second * 3;
                }
                else {
                    std::cout << letter_index << '\n';
                    letter_score = values.find(word[letter_index])->second;
                }

                std::cout << "b" << '\n';

                series_score += letter_score;


                //get score from parallel words if can add

                int parallel_score = 0;
                bool can_add = false;

                //go up
                int up_x = start_x - vertical;
                int up_y = start_y + (1-vertical);
                while (up_x >= 0 && up_y < BOARDSIZE && cells[up_x][up_y] != ' '){
                    std::cout << "d" << '\n';
                    parallel_score += values.find(cells[up_x][up_y])->second * parallel_multiplier;
                    can_add = true;

                    up_x -= vertical;
                    up_y += (1-vertical);
                    std::cout << "e" << '\n';
                }

                //go down
                int down_x = start_x + vertical;
                int down_y = start_y - (1-vertical);
                while (down_x < BOARDSIZE && down_y >= 0 && cells[down_x][down_y] != ' '){
                    parallel_score += values.find(cells[down_x][down_y])->second * parallel_multiplier;
                    can_add = true;

                    down_x += vertical;
                    down_y -= (1-vertical);
                }


                if (can_add){
                    total_parallel_score += (parallel_score + letter_score) * parallel_multiplier;
                }

                cells[start_x][start_y] = word[letter_index];
            }
            else{
                series_score += values.find(cells[start_x][start_y])->second;
            }

            letter_index += 1;
            start_x += (1-vertical);
            start_y -= vertical;
        }
        

        scores[turn] += series_score*series_multiplication + total_parallel_score;

        turn += 1;
        turn = turn % MAXPLAYERS;
        return series_score*series_multiplication + total_parallel_score;
    }

};
