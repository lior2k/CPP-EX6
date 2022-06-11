#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
const int num_teams = 20;
const int win = 1;
const int lose = 0;
std::normal_distribution<> d{75, 5};
std::random_device rd{};
std::mt19937 gen{rd()};

class Team {
    private:
        std::string teamName;
        double skillLevel;
        int pointsGained = 0;
        int pointsGiven = 0;
        std::vector<int> gameLog;
    public:
        Team(std::string, double);
        const std::string& getName() const;
        double getSkill() const;
        void addToGameLog(int);
        int getWinStreak() const;
        int getLoseStreak() const;
        int getTotalWins() const;
        void addPointsGained(int);
        void addPointsGiven(int);
        int getPointsGained() const;
        int getPointsGiven() const;
        void print() const;
};

class Game {
    private:
        Team *homeTeam;
        Team *awayTeam;
        int homeTeamScore = 0;
        int awayTeamScore = 0;
        Team *winner;
    public:
        Game(Team *, Team *);
        Team& getHomeTeam() const;
        Team& getAwayTeam() const;
        void setScoresAndWinner();
        void print() const;
};

class League {
    private:
        std::vector<Team *> teams;
    public:
        League(); // all random
        League(std::vector<Team *> teams); // all teams
        League(std::vector<Team *> teams, int); // teams and random
        std::vector<Team *>& getTeams();
        void print() const;
};

class Schedule {
    
    class Cycle {
        private:
            std::vector<Game *> games;
        public:
            Cycle();
            void addGame(Game *game);
            std::vector<Game *>& getGames() const;
            void print() const;
    };

    private:
        League *league;
        std::vector<Cycle*> cycles;
    public:
        Schedule(League *league = nullptr);
        static void rotate(std::vector<Team *> &, std::vector<Team *> &);
        void sort();
        void showScoreTable() const;
        void showLeadingTeams(int) const;
        void longestWinStreak() const;
        void longestLoseStreak() const;
        void positiveDiff() const;
        void print() const;
};