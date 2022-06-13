#include "Ex6.hpp"
std::normal_distribution<> d{EXPECTATION, VARIANCE};
std::random_device rd{};
std::mt19937 gen{rd()};

// ------------------TEAM----------------------

Team::Team(std::string name, double skill) {
    if (skill >= 1 or skill <= 0) {
        throw std::logic_error("Skill of a team has to be between 0 and 1");
    }
    this->teamName = std::move(name);
    this->skillLevel = skill;
}

const std::string& Team::getName() const {
    return this->teamName;
}

double Team::getSkill() const {
    return this->skillLevel;
}

void Team::addToGameLog(int toAdd) {
    if (toAdd != 0 and toAdd != 1) {
        throw std::logic_error("Can only add 1's or 0's to game log");
    }
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
        if (num == WIN) {
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
        if (num == LOSE) {
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
    if (points < 0) {
        throw std::logic_error("Cant add negative points");
    }
    this->pointsGained+=points;
}

void Team::addPointsGiven(int points) {
    if (points < 0) {
        throw std::logic_error("Cant add negative points");
    }
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
    if (homeTeam == awayTeam) {
        throw std::logic_error("A team cant play vs itself");
    }
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

int Game::getHomeTeamScore() const {
    return homeTeamScore;
}

int Game::getAwayTeamScore() const {
    return awayTeamScore;
}

void Game::setScoresAndWinner() {
    // set scores between 50-100
    while (homeTeamScore < MIN_WINS_HOME or homeTeamScore > MAX_WINS) {
        this->homeTeamScore = d(gen);
    }
    while (awayTeamScore < MIN_WINS_AWAY or awayTeamScore > MAX_WINS) {
        this->awayTeamScore = d(gen);
    }

    //add bonus according to team's skill
    homeTeamScore = homeTeamScore + (int)(MAX_BONUS * homeTeam->getSkill());
    awayTeamScore = awayTeamScore + (int)(MAX_BONUS * awayTeam->getSkill());

    //handle match results
    if (homeTeamScore > awayTeamScore) {
        this->homeTeam->addToGameLog(1);
        this->awayTeam->addToGameLog(0);
        
    } else if (homeTeamScore < awayTeamScore) {
        this->homeTeam->addToGameLog(0);
        this->awayTeam->addToGameLog(1);
    } else {
        if (this->homeTeam->getSkill() >= this->awayTeam->getSkill()) {
            this->homeTeam->addToGameLog(1);
            this->awayTeam->addToGameLog(0);
        } else {
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
    std::cout << "Game[home: " << homeTeam->getName() << ", " << homeTeamScore << " || away: " << awayTeam->getName() << ", " << awayTeamScore << "]" << std::endl;  
}

// ------------------GAME----------------------

// ------------------LEAGUE----------------------

League::League() {
    for (int i = 0; i < NUM_TEAMS; i++) {
        std::string team = "Team";
        teams.push_back(new Team(team.append(std::to_string(i)), ((double) rand() / (RAND_MAX))));
    }
}

League::League(std::vector<Team *> teams) {
    if (teams.size() > NUM_TEAMS) {
        throw std::logic_error("Number of teams in a league is 20, can insert 20 or less and rest will be random");
    }
    for (unsigned int i = 0; i < teams.size(); i++) {
        this->teams.push_back(teams.at(i));
    }
    for (unsigned int i = teams.size(); i < NUM_TEAMS; i++) {
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

std::vector<Game *>& Schedule::Cycle::getGames() {
    return this->games;
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
    for (unsigned int i = bot.size()-2; i >= 0; i--) {
        if (i == (unsigned int) -1) {
            break;
        }
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
    Cycle *cycle_a = nullptr;
    Cycle *cycle_b = nullptr;
    Game *game = nullptr;
    for (unsigned int i = 0; i < this->league->getTeams().size()-1; i++) {
        if (i > 0) {
            rotate(top, bot);
        }
        cycle_a = new Cycle();
        cycle_b = new Cycle();
        for (unsigned int k = 0; k < top.size(); k++) {
            cycle_a->addGame(new Game(top[k], bot[k]));
            cycle_b->addGame(new Game(bot[k], top[k]));
        }
        this->cycles.push_back(cycle_a);
        this->cycles.push_back(cycle_b);
    }
    sort();
}

Schedule::~Schedule() {
    for (unsigned int i = 0; i < league->getTeams().size(); i++) {
        delete(league->getTeams()[i]);
    }
    for (unsigned int i = 0; i < cycles.size(); i++) {
        for (uint j = 0; j < cycles[i]->getGames().size(); j++) {
            delete(cycles[i]->getGames()[j]);
        }
        delete(cycles[i]);
    }
    delete(this->league);
}

std::vector<Schedule::Cycle *>& Schedule::getCycles() {
    return this->cycles;
}

void Schedule::sort() {
    std::vector<Team *>& teams = league->getTeams();
    int size = teams.size();
    std::vector<Team *> sortedTeams;
    Team *currTeam = nullptr;
    uint currIndex = 0;
    int ratio = 0;
    int basketDifference = 0;
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
    int streak = 0;
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
    int streak = 0;
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

void Schedule::avgScore() const {
    double totalPoints = 0;
    for (Team *team : league->getTeams()) {
        totalPoints+=team->getPointsGained();
    }
    size_t totalGames = (league->getTeams().size() * (league->getTeams().size() -1)) * 2;
    std::cout << "Average points of a team per game: " << totalPoints / totalGames << std::endl;
}

void Schedule::avgSkill() const {
    double sum = 0;
    for (unsigned int i = 0; i < league->getTeams().size(); i++) {
        sum+=league->getTeams()[i]->getSkill();
    }
    std::cout << "Average skill of a team in the league: " << sum / league->getTeams().size() << std::endl;
}

void Schedule::print() const {
    for (unsigned int i = 0; i < cycles.size(); i++) {
        cycles[i]->print();
    }
}

// ------------------SCHEDULE----------------------


// int main(int argc, char const *argv[]) {
//     // Team *t1 = new Team("Team Liel", 0.1);
//     // Team *t2 = new Team("Team Lior", 0.2);
//     // Team *t3 = new Team("Team Moriya", 0.3);
//     // Team *t4 = new Team("Team Chai", 0.4);
//     // Team *t5 = new Team("Team A", 0.5);
//     // Team *t6 = new Team("Team B", 0.6);
//     // Team *t7 = new Team("Team C", 0.7);
//     // Team *t8 = new Team("Team D", 0.8);
//     // std::vector<Team *> vec = {t1, t2 ,t3 ,t4, t5, t6, t7, t8};
//     // std::vector<Team *> vec = {t1, t2 ,t3 ,t4};
//     // League *league = new League(vec);

//     Schedule *s = new Schedule();

//     s->showScoreTable();
//     // s->showLeadingTeams(5);
//     s->longestWinStreak();
//     s->longestLoseStreak();
//     s->positiveDiff();
//     s->avgScore();
//     s->avgSkill();
//     // s->~Schedule();
//     delete(s);
//     return 0;
// }
