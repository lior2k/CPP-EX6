#include "Ex6.hpp"

// ------------------TEAM----------------------

Team::Team(std::string name, double skill) {
    this->teamName = name;
    this->skillLevel = skill;
}

const std::string& Team::getName() const {
    return this->teamName;
}

double Team::getSkill() const {
    return this->skillLevel;
}

void Team::addToGameLog(int toAdd) {
    this->gameLog.push_back(toAdd);
}

int Team::getTotalWins() const {
    int total = 0;
    for(int num : gameLog) {
        total += num;
    }
    return total;
}

int Team::getWinStreak() const {
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

int Team::getLoseStreak() const {
    int streak = 0;
    int longestStreak = 0;
    for(int num : gameLog) {    
        if (num == lose) {
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

void Team::addPointsGained(int points) {
    this->pointsGained+=points;
}

void Team::addPointsGiven(int points) {
    this->pointsGiven+=points;
}

int Team::getPointsGained() const {
    return this->pointsGained;
}

int Team::getPointsGiven() const {
    return this->pointsGiven;
}

void Team::print() const {
    std::cout << "Team[name: " << this->teamName << ", skill: " << this->skillLevel
    << ", gained: " << pointsGained << ", given: " << pointsGiven
    << ", diff: " << pointsGained - pointsGiven <<", wins: " << getTotalWins()
    << ", streak: " << getWinStreak() << "]";
}

// ------------------TEAM----------------------

// ------------------GAME----------------------

Game::Game(Team *homeTeam, Team *awayTeam) {
    this->homeTeam = homeTeam;
    this->awayTeam = awayTeam;
    setScoresAndWinner();
}

Team& Game::getHomeTeam() const {
    return *(this->homeTeam);
}

Team& Game::getAwayTeam() const {
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
    this->homeTeam->addPointsGained(homeTeamScore);
    this->awayTeam->addPointsGained(awayTeamScore); 
    this->homeTeam->addPointsGiven(awayTeamScore);
    this->awayTeam->addPointsGiven(homeTeamScore);
}

void Game::print() const {
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

std::vector<Team *>& League::getTeams() {
    return this->teams;
}

void League::print() const {
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

void Schedule::Cycle::print() const {
    std::cout << "----- Begin Cycle -----" << std::endl;
    for (unsigned int i = 0; i < games.size(); i++) {
        games[i]->print();
    }
    std::cout << "----- End Cycle -----" << std::endl;
}

void Schedule::rotate(std::vector<Team *> &top, std::vector<Team *> &bot) {
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
}

Schedule::Schedule(League *league) {
    if (league == nullptr) {
        this->league = new League();
    } else {
        this->league = league;
    }
    
    std::vector<Team *> top;
    std::vector<Team *> bot;
    for (unsigned int i = 0; i < this->league->getTeams().size(); i++) {
        if (i < this->league->getTeams().size() / 2) {
            top.push_back(this->league->getTeams()[i]);
        } else {
            bot.push_back(this->league->getTeams()[i]);
        }
    }
    Cycle *cycle_a;
    Cycle *cycle_b;
    Game *game;
    for (unsigned int i = 0; i < this->league->getTeams().size()-1; i++) {
        if (i > 0) {
            rotate(top, bot);
        }
        cycle_a = new Cycle();
        cycle_b = new Cycle();
        for (int k = 0; k < top.size(); k++) {
            cycle_a->addGame(new Game(top[k], bot[k]));
            cycle_b->addGame(new Game(bot[k], top[k]));
        }
        this->cycles.push_back(cycle_a);
        this->cycles.push_back(cycle_b);
    }

    sort();
}

void Schedule::sort() {
    std::vector<Team *>& teams = league->getTeams();
    int size = teams.size();
    std::vector<Team *> sortedTeams;
    Team *currTeam;
    int currIndex;
    int ratio;
    int basketDifference;
    while(sortedTeams.size() < size) {
        double bestRatio = -1;
        int bestBasketDifference = 0;
        for (unsigned int i = 0; i < teams.size(); i++) {
            ratio = teams[i]->getTotalWins();
            currTeam = teams[i];
            if (ratio > bestRatio) {
                bestRatio = ratio;
                currIndex = i;
                bestBasketDifference = currTeam->getPointsGained() - currTeam->getPointsGiven();
            }
            if (ratio == bestRatio) {
                basketDifference = teams[i]->getPointsGained() - teams[i]->getPointsGiven();
                if (basketDifference > bestBasketDifference) {
                    bestBasketDifference = basketDifference;
                    currIndex = i;
                }
            }
        }
        sortedTeams.push_back(teams.at(currIndex));
        teams.erase(teams.begin() + currIndex);
    }

    // return teams to teams - should change
    for (unsigned int i = 0; i < sortedTeams.size(); i++) {
        teams.push_back(sortedTeams[i]);
    }
}

void Schedule::showScoreTable() const {
    for (unsigned int i = 0; i < league->getTeams().size(); i++) { 
        std::cout << "[Rank " << i+1 << ": ";
        league->getTeams()[i]->print();
        std::cout << "]" << std::endl;
    }
}

void Schedule::showLeadingTeams(int n) const {
    std::cout << "Leading Teams:" << std::endl;
    for (unsigned int i = 0; i < n; i++) {
        league->getTeams()[i]->print();
        std::cout << std::endl;
    }
}

void Schedule::longestWinStreak() const {
    int streak;
    int maxStreak = 0;
    for (unsigned int i = 0; i < league->getTeams().size(); i++) {
        streak = league->getTeams()[i]->getWinStreak();
        if (streak > maxStreak) {
            maxStreak = streak;
        }
    }
    std::cout << "Max win streak: " << maxStreak << std::endl;
}

void Schedule::longestLoseStreak() const {
    int streak;
    int maxStreak = 0;
    for (unsigned int i = 0; i < league->getTeams().size(); i++) {
        streak = league->getTeams()[i]->getLoseStreak();
        if (streak > maxStreak) {
            maxStreak = streak;
        }
    }
    std::cout << "Max lose streak: " << maxStreak << std::endl;
}

void Schedule::positiveDiff() const {
    int n = 0;
    std::vector<Team *> teams = league->getTeams();
    for (unsigned int i = 0; i < teams.size(); i++) {
        if (teams[i]->getPointsGained() - teams[i]->getPointsGiven() > 0) {
            n++;
        }
    }
    std::cout << "Number of teams that gained more points then given: " << n << std::endl;
}

void Schedule::print() const {
    for (unsigned int i = 0; i < cycles.size(); i++) {
        cycles[i]->print();
    }
}

// ------------------SCHEDULE----------------------


int main(int argc, char const *argv[]) {
    // Team *t1 = new Team("Team Liel", 0.1);
    // Team *t2 = new Team("Team Lior", 0.2);
    // Team *t3 = new Team("Team Moriya", 0.3);
    // Team *t4 = new Team("Team Chai", 0.4);
    // Team *t5 = new Team("Team A", 0.5);
    // Team *t6 = new Team("Team B", 0.6);
    // Team *t7 = new Team("Team C", 0.7);
    // Team *t8 = new Team("Team D", 0.8);
    // std::vector<Team *> vec = {t1, t2 ,t3 ,t4, t5, t6, t7, t8};
    // League *league = new League(vec);
    Schedule *s = new Schedule();
    // s->print();

    // for (int i = 0; i < 20; i++) {
    //     Game* game = new Game(t1, t2);
    //     game->print();
    // }
    // t1->print();
    // t2->print();
    s->showScoreTable();
    s->showLeadingTeams(5);
    s->longestWinStreak();
    s->longestLoseStreak();
    s->positiveDiff();
    return 0;
}
