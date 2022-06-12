#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
const int NUM_TEAMS = 20;
const int WIN = 1;
const int LOSE = 0;
const int EXPECTATION = 75;
const int VARIANCE = 5;
const int MAX_WINS = 100;
const int MIN_WINS_HOME = 55;
const int MIN_WINS_AWAY = 50;
const int MAX_BONUS = 10;
std::normal_distribution<> d{EXPECTATION, VARIANCE};
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
        // ~Team(); // default is fine
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
        void setScoresAndWinner();
    public:
        Game(Team *, Team *);
        Team& getHomeTeam() const;
        Team& getAwayTeam() const;
        void print() const;
};

class League {
    private:
        std::vector<Team *> teams;
    public:
        League(); // all random
        League(std::vector<Team *> teams); // all teams or some teams some random
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
            std::vector<Game *>& getGames();
            void print() const;
    };

    private:
        League *league;
        std::vector<Cycle*> cycles;
    public:
        Schedule(League *league = nullptr);
        ~Schedule();
        static void rotate(std::vector<Team *> &, std::vector<Team *> &);
        void sort();
        void showScoreTable() const;
        void showLeadingTeams(int) const;
        void longestWinStreak() const;
        void longestLoseStreak() const;
        void positiveDiff() const;

        void avgScore() const;
        

        void print() const;
};