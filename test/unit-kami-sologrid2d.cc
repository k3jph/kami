/*-
 * Copyright (c) 2022 The Johns Hopkins University Applied Physics
 * Laboratory LLC
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <algorithm>
#include <iterator>
#include <set>
#include <unordered_set>

#include <kami/agent.h>
#include <kami/error.h>
#include <kami/sologrid2d.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace kami;
using namespace kami::error;
using namespace std;

TEST(SoloGrid2D, DefaultConstructor) {
    // There is really no way this can go wrong, but
    // we add this check anyway in case of future
    // changes.
    EXPECT_NO_THROW(
            SoloGrid2D sologrid2d_foo(10, 10, true, true);
    );
}

TEST(SoloGrid2D, add_agent) {
    SoloGrid2D sologrid2d_foo(10, 10, true, true);
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord2(2, 5), coord3(3, 7);

    {
        auto agent_id_baz = sologrid2d_foo.add_agent(agent_id_foo, coord2);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.add_agent(agent_id_bar, coord2), LocationUnavailable);
    }
    {
        auto agent_id_baz = sologrid2d_foo.add_agent(agent_id_bar, coord3);
        EXPECT_EQ(agent_id_baz, agent_id_bar);
    }
}

TEST(SoloGrid2D, delete_agent) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord2(2, 5), coord3(3, 7);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_bar), AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo, coord2);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo, coord2);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_bar, coord2), AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo, coord3), AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_foo, coord3), AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.delete_agent(agent_id_bar, coord3), AgentNotFound);
    }
}

TEST(SoloGrid2D, is_location_valid) {
    SoloGrid2D sologrid2d_foo(10, 10, true, true);
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coordm1(-1, -1), coord0(0, 0), coord2(2, 5), coord3(3, 7), coord10(10, 5), coord100(100, 100);

    {
        EXPECT_TRUE(sologrid2d_foo.is_location_valid(coord0));
        EXPECT_TRUE(sologrid2d_foo.is_location_valid(coord2));
        EXPECT_TRUE(sologrid2d_foo.is_location_valid(coord2));

        EXPECT_FALSE(sologrid2d_foo.is_location_valid(coordm1));
        EXPECT_FALSE(sologrid2d_foo.is_location_valid(coord10));
        EXPECT_FALSE(sologrid2d_foo.is_location_valid(coord100));
    }
}

TEST(SoloGrid2D, is_location_empty) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord2(2, 5), coord3(3, 7);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        EXPECT_TRUE(sologrid2d_foo.is_location_empty(coord2));
        EXPECT_TRUE(sologrid2d_foo.is_location_empty(coord3));
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_FALSE(sologrid2d_foo.is_location_empty(coord2));
        EXPECT_TRUE(sologrid2d_foo.is_location_empty(coord3));
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_FALSE(sologrid2d_foo.is_location_empty(coord2));
        EXPECT_TRUE(sologrid2d_foo.is_location_empty(coord3));
    }
}

TEST(SoloGrid2D, move_agent) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord2(2, 5), coord7(7, 2), coord10(10, 5);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        auto agent_id_baz = sologrid2d_foo.move_agent(agent_id_foo, coord7);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(auto agent_id_baz = sologrid2d_foo.move_agent(agent_id_foo, coord10), LocationInvalid);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        EXPECT_THROW(static_cast<void>(sologrid2d_foo.add_agent(agent_id_bar, coord2)), LocationUnavailable);
        auto agent_id_baz = sologrid2d_foo.move_agent(agent_id_foo, coord7);
        EXPECT_EQ(agent_id_baz, agent_id_foo);
    }
}

TEST(SoloGrid2D, get_neighborhood_VonNeumann) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord0(0, 0), coord1(1, 1), coord2(2, 5), coord3(3, 7), coord9(9, 4);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                0, 9
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord1, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord1, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                0, 9
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                0, 9
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        EXPECT_THROW(auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::VonNeumann),
                AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);
        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                9, 0
            },
            {
                0, 9
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                0, 9
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                0, 1
            },
            {
                1, 2
            },
            {
                2, 1
            },
            {
                1, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::VonNeumann);

        EXPECT_EQ(tval, *rval);
    }
}

TEST(SoloGrid2D, get_neighborhood_Moore) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord0(0, 0), coord1(1, 1), coord2(2, 5), coord3(3, 7), coord9(9, 4);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                9, 9
            },
            {
                9, 1
            },
            {
                1, 1
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                1, 9
            },
            {
                0, 9
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, true, GridNeighborhoodType::Moore);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord1, true, GridNeighborhoodType::Moore);

        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 1
            },
            {
                0, 1
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord1, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                9, 9
            },
            {
                9, 1
            },
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            },
            {
                0, 9
            },
            {
                1, 9
            },
            {
                9, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        auto tval = unordered_set < GridCoord2D > ({
            {
                9, 9
            },
            {
                9, 1
            },
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            },
            {
                0, 9
            },
            {
                1, 9
            },
            {
                9, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(coord0, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        EXPECT_THROW(auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::Moore),
                AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);
        auto tval = unordered_set < GridCoord2D > ({
            {
                9, 9
            },
            {
                9, 1
            },
            {
                1, 1
            },
            {
                0, 1
            },
            {
                9, 0
            },
            {
                1, 9
            },
            {
                0, 9
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 1
            },
            {
                0, 1
            },
            {
                1, 0
            },
            {
                0, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                1, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, true, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                9, 9
            },
            {
                9, 1
            },
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            },
            {
                0, 9
            },
            {
                1, 9
            },
            {
                9, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord0);

        auto tval = unordered_set < GridCoord2D > ({
            {
                1, 0
            },
            {
                1, 1
            },
            {
                0, 1
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, false, false);
        sologrid2d_foo.add_agent(agent_id_foo, coord1);

        auto tval = unordered_set < GridCoord2D > ({
            {
                2, 2
            },
            {
                2, 0
            },
            {
                0, 1
            },
            {
                0, 2
            },
            {
                1, 2
            },
            {
                0, 0
            },
            {
                2, 1
            },
            {
                1, 0
            }
        });
        auto rval = sologrid2d_foo.get_neighborhood(agent_id_foo, false, GridNeighborhoodType::Moore);

        EXPECT_TRUE(rval);
        EXPECT_EQ(tval, *rval);
    }
}

TEST(SoloGrid2D, get_location_by_agent) {
    const AgentID agent_id_foo, agent_id_bar;
    const GridCoord2D coord2(2, 5);

    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        EXPECT_THROW(auto loc1 = sologrid2d_foo.get_location_by_agent(agent_id_foo), AgentNotFound);
        EXPECT_THROW(auto loc2 = sologrid2d_foo.get_location_by_agent(agent_id_bar), AgentNotFound);
    }
    {
        SoloGrid2D sologrid2d_foo(10, 10, true, true);

        static_cast<void>(sologrid2d_foo.add_agent(agent_id_foo, coord2));
        auto local = sologrid2d_foo.get_location_by_agent(agent_id_foo);
        EXPECT_EQ(local, coord2);
        EXPECT_THROW(auto loc = sologrid2d_foo.get_location_by_agent(agent_id_bar), AgentNotFound);
    }
}

int main(
        int argc,
        char** argv
) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

