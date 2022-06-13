//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "sources/Ex6.hpp"
#include "doctest.h"

TEST_CASE("BUGS & BAD USAGE") {

    Team t1{"Team One", 0.75};
    Team t2{"Team Two", 0.35};

    Team *t3;
    CHECK_THROWS(t3 = new Team("Skill", -1)); // skill has to be > 0 and < 1
    CHECK_THROWS(t3 = new Team("Skill", 2)); // skill has to be > 0 and < 1

    CHECK_THROWS(t1.addPointsGained(-100)); // cant subtract, has to be >= 0
    CHECK_THROWS(t1.addPointsGiven(-50)); // cant subtract, has to be >= 0
    CHECK_THROWS(t1.addToGameLog(-1)); // game log has only 0's or 1's, 0 for lose, 1 for win

    Game *game;
    CHECK_THROWS(game = new Game(&t1, &t1)); // a team cant play vs itself

    League *league;
    std::vector<Team *> vec;
    for(uint i = 0; i < 21; i++) {
        vec.push_back(new Team(std::to_string(i), 0.5));
    }
    
    CHECK_THROWS(league = new League(vec)); // league cant have more then 20 teams

}

TEST_CASE("SHOULD WORK") {

    std::vector<Team *> vec;
    for(uint i = 0; i < 10; i++) {
        vec.push_back(new Team(std::to_string(i), 0.5));
    }

    League *league = new League();
    CHECK_EQ(league->getTeams().size(), 20); // all random teams, should be filled with 20 random teams

    league = new League(vec);
    CHECK_EQ(league->getTeams().size(), 20); // sent in 10 teams, should be filled with 10 random teams

    for(uint i = 0; i < 10; i++) {
        vec.push_back(new Team(std::to_string(i), 0.5));
    }
    league = new League(vec);
    CHECK_EQ(league->getTeams().size(), 20);


    // check that all scores are between 50-110 (55-110 for home teams) 110 and not 100 since the bonus
    Schedule sc{league};
    for(uint i = 0; i < sc.getCycles().size(); i++) {
        for(uint j = 0; j < sc.getCycles()[i]->getGames().size(); j++) {
            CHECK(sc.getCycles()[i]->getGames()[j]->getHomeTeamScore() < 110);
            CHECK(sc.getCycles()[i]->getGames()[j]->getHomeTeamScore() > 55);
            CHECK(sc.getCycles()[i]->getGames()[j]->getAwayTeamScore() < 110);
            CHECK(sc.getCycles()[i]->getGames()[j]->getAwayTeamScore() > 50);
        }
    }
}

