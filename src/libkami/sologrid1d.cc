/*-
 * Copyright (c) 2020 The Johns Hopkins University Applied Physics
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

#include <fmt/format.h>

#include <kami/agent.h>
#include <kami/error.h>
#include <kami/sologrid1d.h>

namespace kami {

    SoloGrid1D::SoloGrid1D(
            unsigned int maximum_x,
            bool wrap_x
    )
            :Grid1D(maximum_x, wrap_x) {
    }

    AgentID SoloGrid1D::add_agent(
            const AgentID agent_id,
            const GridCoord1D& coord
    ) {
        if (!is_location_valid(coord))
            throw error::LocationInvalid(fmt::format("Coordinates {} are invalid", coord.to_string()));
        if (!is_location_empty(coord))
            throw error::LocationUnavailable(fmt::format("Coordinates {} already occupied", coord.to_string()));

        _agent_index->insert(std::pair<AgentID, GridCoord1D>(agent_id, coord));
        _agent_grid->insert(std::pair<GridCoord1D, AgentID>(coord, agent_id));
        return agent_id;
    }

}  // namespace kami
