#include "Ex6.hpp"

// ------------------TEAM----------------------

Team::Team(std::string name, double skill) {
    this->teamName = name;
    this->skillLevel = skill;
}

std::string& Team::getName() {
    return this->teamName;
}

double Team::getSkill() {
    return this->skillLevel;
}

void Team::addToGameLog(int toAdd) {
    this->gameLog.push_back(toAdd);
}

int Team::getTotalWins() {
    int total = 0;
    for(int num : gameLog) {
        total += num;
    }
    return total;
}

int Team::getWinStreak() {
    int streak = 0;
    int longestStreak = 0;
    for(int num : gameLog) {    
        if (num == win) {
            streak++;
        } else {
            streak = 0;
        }
        if (streak > longestStreak) {
            longestStreak = streak;
        }
    }
    return longestStreak;
}

void Team::addPoints(int points) {
    this->totalPoints+=points;
}

void Team::print() {
    std::cout << "Team[name: " << this->teamName << ", skill: " << this->skillLevel << ", points: " << totalPoints << ", wins: " << getTotalWins() << ", highest streak: " << getWinStreak() << "]" << std::endl;
}

// ------------------TEAM----------------------

// ------------------GAME----------------------

Game::Game(Team *homeTeam, Team *awayTeam) {
    this->homeTeam = homeTeam;
    this->awayTeam = awayTeam;
    setScoresAndWinner();
}

Team& Game::getHomeTeam() {
    return *(this->homeTeam);
}

Team& Game::getAwayTeam() {
    return *(this->awayTeam);
}

void Game::setScoresAndWinner() {
    // set scores between 50-100
    while (homeTeamScore < 55 or homeTeamScore > 100) {
        this->homeTeamScore = d(gen);
    }
    while (awayTeamScore < 50 or awayTeamScore > 100) {
        this->awayTeamScore = d(gen);
    }

    //add bonus according to team's skill
    homeTeamScore = homeTeamScore + (int)(10 * homeTeam->getSkill());
    awayTeamScore = awayTeamScore + (int)(10 * awayTeam->getSkill());

    //handle match results
    if (homeTeamScore > awayTeamScore) {
        winner = this->homeTeam;
        this->homeTeam->addToGameLog(1);
        this->awayTeam->addToGameLog(0);
        
    } else if (homeTeamScore < awayTeamScore) {
        winner = this->awayTeam;
        this->homeTeam->addToGameLog(0);
        this->awayTeam->addToGameLog(1);
    } else {
        if (this->homeTeam->getSkill() >= this->awayTeam->getSkill()) {
            winner = this->homeTeam;
            this->homeTeam->addToGameLog(1);
            this->awayTeam->addToGameLog(0);
        } else {
            winner = this->awayTeam;
            this->homeTeam->addToGameLog(0);
            this->awayTeam->addToGameLog(1);
        }
    }
    this->homeTeam->addPoints(homeTeamScore);
    this->awayTeam->addPoints(awayTeamScore); 
}

void Game::print() {
    std::cout << "Game[home: " << homeTeam->getName() << ", " << homeTeamScore << " || away: " << awayTeam->getName() << ", " << awayTeamScore << " || winner: " << winner->getName() << "]" << std::endl;  
}

// ------------------GAME----------------------

// ------------------LEAGUE----------------------

League::League() {
    for (int i = 0; i < num_teams; i++) {
        std::string team = "Team";
        teams.push_back(new Team(team.append(std::to_string(i)), ((double) rand() / (RAND_MAX))));
    }
}

League::League(std::vector<Team *> teams) {
    for (unsigned int i = 0; i < teams.size(); i++) {
        this->teams.push_back(teams[i]);
    }
}

League::League(std::vector<Team *> teams, int dummy) {
    for (unsigned int i = 0; i < teams.size(); i++) {
        this->teams.push_back(teams.at(i));
    }
    for (unsigned int i = teams.size(); i < num_teams; i++) {
        std::string team = "Team";
        this->teams.push_back(new Team(team.append(std::to_string(i)), ((double) rand() / (RAND_MAX))));
    }
}

std::vector<Team *> League::getTeams() {
    return this->teams;
}

void League::print() {
    for (unsigned int i = 0; i < teams.size(); i++) {
        teams[i]->print();
    }
}

// ------------------LEAGUE----------------------

// ------------------SCHEDULE----------------------

Schedule::Cycle::Cycle() {

}

void Schedule::Cycle::addGame(Game *game) {
    this->games.push_back(game);
}

void Schedule::Cycle::print() {
    std::cout << "----- Begin Cycle -----" << std::endl;
    for (unsigned int i = 0; i < games.size(); i++) {
        games[i]->print();
    }
    std::cout << "----- End Cycle -----" << std::endl;
}

void rotate(std::vector<Team *> &v1, std::vector<Team *> &v2) {
    std::vector<Team *> v1copy{v1.begin(), v1.end()};
    std::vector<Team *> v2copy{v2.begin(), v2.end()};

    v1[1] = v2copy[0];
    for (unsigned int i = 2; i < v1.size(); i++) {
        v1[i] = v1copy[i-1];
    }

    v2[v2.size()-2] = v1copy[v1copy.size()-1];
    for (unsigned int i = v2.size()-2; i >= 0; i--) {
        v2[i] = v2copy[i+1];
    }
}

Schedule::Schedule(League *league) {
    if (league == nullptr) {
        this->league = new League();
    } else {
        this->league = league;
    }

    std::vector<Team *> top;
    std::vector<Team *> bot;
    for (unsigned int i = 0; i < league->getTeams().size(); i++) {
        if (i < league->getTeams().size() / 2) {
            top.push_back(league->getTeams()[i]);
        } else {
            bot.push_back(league->getTeams()[i]);
        }
    }
    // std::cout << "topsize: " << top.size() << std::endl;
    // std::cout << "botsize: " << bot.size() << std::endl;
    Cycle *cycle;
    Game *game;
    for (unsigned int i = 0; i < league->getTeams().size()-1; i++) {
        cycle = new Cycle();
        if (i > 0) {
            // rotate
            std::vector<Team *> v1copy{top.begin(), top.end()};
            std::vector<Team *> v2copy{bot.begin(), bot.end()};

            top[1] = v2copy[0];
            for (unsigned int i = 2; i < top.size(); i++) {
                top[i] = v1copy[i-1];
            }

            bot[bot.size()-1] = v1copy[v1copy.size()-1];
            for (int i = bot.size()-2; i >= 0; i--) {
                bot[i] = v2copy[i+1];
            }
            // rotate(top, bot);
        }
        for (int k = 0; k < top.size(); k++) {
            cycle->addGame(new Game(top[k], bot[k]));
        }
        this->cycles.push_back(cycle);
    }
}

void Schedule::print() {
    for (unsigned int i = 0; i < cycles.size(); i++) {
        cycles[i]->print();
    }
}

// ------------------SCHEDULE----------------------


int main(int argc, char const *argv[])
{
    Team *t1 = new Team("Team Liel", 0.1);
    Team *t2 = new Team("Team Lior", 0.2);
    Team *t3 = new Team("Team Moriya", 0.3);
    Team *t4 = new Team("Team Chai", 0.4);
    Team *t5 = new Team("Team A", 0.5);
    Team *t6 = new Team("Team B", 0.6);
    Team *t7 = new Team("Team C", 0.7);
    Team *t8 = new Team("Team D", 0.8);
    std::vector<Team *> vec = {t1, t2 ,t3 ,t4, t5, t6, t7, t8};
    League *league = new League(vec);
    Schedule *s = new Schedule(league);
    s->print();

    // for (int i = 0; i < 20; i++) {
    //     Game* game = new Game(t1, t2);
    //     game->print();
    // }
    // t1->print();
    // t2->print();


    return 0;
}
