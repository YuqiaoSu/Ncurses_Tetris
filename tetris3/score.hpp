// Yuqiao Su, Zekai Shen 2022/3/12
#ifndef MY_CODE3520
#define MY_CODE3520
#include <bits/stdc++.h>
#include <vector>
using namespace std;
//calculating the score base on current score and lines cleared.
struct record{
  int score;
  string name;

};



// Load response from the file
void loadScore(vector<record> &scores);

// save the response to the file
void saveScore(vector<record> rec);
//save the records when game over.
void save_record(string name, int score, vector<record> &records);
//print the response below the next teromino
void printScore(vector<record> &records, int row, int column);
int compute_score(int previous_score, int lines_cleared);

#endif