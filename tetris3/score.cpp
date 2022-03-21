#include <cstdlib>
#include <ncurses.h>
#include "score.hpp"
#include <bits/stdc++.h>
#include <vector>
using namespace std;
// Yuqiao Su, Zekai Shen 2022/3/12

//compare
bool my_cmp(const record& a, const record& b)
{
    // smallest comes first
    return a.score > b.score;
};

//compute the score using lines cleared
int compute_score(int previous_score, int lines_cleared){
    int score = lines_cleared * lines_cleared;
    return previous_score + score;
}

// Load response from the file
void loadScore(vector<record> &scores)
{
  ifstream records("highscores.txt");
  if (!records)
  {
    cout << "can't open file" << endl;
    EXIT_FAILURE;
  }

  string res;
  int v; 
  string b;
  while (getline(records, res))
  {
    int comma = res.find(",");
    v = stoi(res.substr(0,comma));
    b = res.substr(comma + 1);
    record temp = {v,b};

    scores.push_back(temp);
  }
}

// save the response to the file
void saveScore(vector<record> rec)
{
  ofstream records("highscores.txt");

  if (!records)
  {
    cout << "can't open file" << endl;
    EXIT_FAILURE;
  }
  for (auto r : rec)
  {
    records << r.score << "," << r.name << endl;
  }
}

//save the records when game over.
void save_record(string name, int score, vector<record> &records){
  record temp;
  temp.name = name;
  temp.score = score;
  records.push_back(temp);
  saveScore(records);
}

//print the response below the next teromino
void printScore(vector<record> &records, int row, int column)
{
  sort(records.begin(), records.end(), my_cmp);
  // print top 10
  int count = 1;
  mvprintw(row, column, "Top 10 Scores:", 0);
  for (auto record : records)
  {
    if (count <= 10)
    {
      mvprintw(row + count, column, "Top %d: %d %s", count, record.score, record.name.c_str());
      count++;
    }
    else
    {
      break;
    }
  }
}
