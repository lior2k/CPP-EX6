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
        int totalPoints = 0;
        std::vector<int> gameLog;
    public:
        Team(std::string, double);
        std::string& getName();
        double getSkill();
        void print();
        void addToGameLog(int);
        int getWinStreak();
        int getTotalWins();
        void addPoints(int);
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
        Team& getHomeTeam();
        Team& getAwayTeam();
        void setScoresAndWinner();
        void print();
};

class League {
    private:
        std::vector<Team *> teams;
    public:
        League(); // all random
        League(std::vector<Team *> teams); // all teams
        League(std::vector<Team *> teams, int); // teams and random
        std::vector<Team *> getTeams();
        void print();
};

class Schedule {
    
    class Cycle {
        private:
            std::vector<Game *> games;
        public:
            Cycle();
            void addGame(Game *game);
            std::vector<Game *>& getGames();
            void print();
    };

    private:
        League *league;
        std::vector<Cycle*> cycles;
    public:
        Schedule(League *league = nullptr);
        void print();
};