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

#pragma once
#ifndef KAMI_AGENT_H
#define KAMI_AGENT_H

#include <kami/KAMI_EXPORT.h>

#include <iostream>
#include <string>

namespace kami {

///  \brief A unique identifier for each agent.
///  \details A unqiue identifier is created for each Agent at
///  runtime.  The unique identifier is an unsigned integer that
///  increments monotonically with each new Agent instantiated.
///  AgentIDs are not guaranteed to be unique from session-to-session.
class LIBKAMI_EXPORT AgentID {
   public:
    ///  \brief Constructs a new unique identifier.
    AgentID();

    ///  \brief Convert the identifier to a human readable string.
    std::string toString() const;

    ///  \brief Test if two AgentID instances are equal.
    friend bool operator==(const AgentID &lhs, const AgentID &rhs);

    ///  \brief Test if two AgentID instances are not equal.
    friend bool operator!=(const AgentID &lhs, const AgentID &rhs);

    ///  \brief Test if one AgentID is less than another.
    ///  \details Due to the way AgentID instance are used internally,
    ///  the AgentID must be ordered.  The `<` operator provides
    ///  basic order sufficient for `std::map`.
    friend bool operator<(const AgentID &lhs, const AgentID &rhs);

    ///  \brief Output an AgentID to the specified output stream.
    friend std::ostream &operator<<(std::ostream &lhs, const AgentID &rhs);

   private:
    static uint64_t idGen;
    uint64_t id;
};

///  \brief A superclass for Agents
///
///  \details All agents should subclass the Agent class.
///  At a minimum, subclasses must implement the `step()`
///  function, to execute a single time step for each agent.
class LIBKAMI_EXPORT Agent {
   public:
    /// \brief Get the agent's self identification number.
    AgentID getAgentID() const;

    /// \brief Execute a time-step for the agent
    ///
    /// \details This function should step the agent instance.  Any activities that the
    /// agent should perform as part of its time step should be in this function.
    virtual void step() = 0;

    friend bool operator==(const Agent &lhs, const Agent &rhs);

    friend bool operator!=(const Agent &lhs, const Agent &rhs);

   private:
    AgentID agentID;
};

///  \brief A class for agents with and post-`step()` advancement.
///
///  \details Staged agents use a two-phase step to allow agents to take actions without
///  updating the state of the model before all agents have been allowed to
///  update.
class LIBKAMI_EXPORT StagedAgent : public Agent {
   public:
    ///  \brief Post-step advance the agent
    ///
    ///  \details This method should be called after `step()`.  Any updates or cleanups
    ///  to the agent should occur here that must happen after the step is complete.
    virtual void advance() = 0;
};

}  // namespace kami

#endif  // KAMI_AGENT_H
