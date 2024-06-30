#ifndef SCORE_INFO_H
#define SCORE_INFO_H

#include <list>

class ScoreInfo
{
public:
    ScoreInfo(unsigned int startScore, unsigned int scoresNumber);

    void ChangeScore(int value);
    void AddScoreInHistory(int value);

    unsigned int getCurrentScore();
    std::list<int>& getReceivedScoresHistory();

private:
    unsigned int currentScore;
    std::list<int> receivedScoresHistory;
    unsigned int scoresNumber;
};

#endif