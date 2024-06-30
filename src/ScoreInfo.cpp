#include "ScoreInfo.h"

ScoreInfo::ScoreInfo(unsigned int startScore, unsigned int scoresNumber) : currentScore(startScore) , scoresNumber(scoresNumber) {}

void ScoreInfo::ChangeScore(int value)
{
    int newCurrentScore = currentScore + value;

    newCurrentScore >= 0 ? currentScore = newCurrentScore : currentScore = 0;
}

void ScoreInfo::AddScoreInHistory(int value)
{
    receivedScoresHistory.push_front(value);

    if (receivedScoresHistory.size() > scoresNumber)
        receivedScoresHistory.pop_back();
}

unsigned int ScoreInfo::getCurrentScore()
{
    return currentScore;
}

std::list<int>& ScoreInfo::getReceivedScoresHistory()
{
    return receivedScoresHistory;
}
