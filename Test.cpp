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

    // Game game{&t1, &t2};
    // CHECK_THROWS(game.);

    // league throws

}

