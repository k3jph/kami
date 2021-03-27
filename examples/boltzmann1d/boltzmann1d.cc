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

#include "boltzmann1d.h"

#include <kami/agent.h>
#include <kami/config.h>
#include <kami/grid2d.h>
#include <kami/kami.h>
#include <kami/multigrid2d.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <stdlib.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <iostream>
#include <map>
#include <string>

MultiGrid1D *MoneyAgent::world = nullptr;
BoltzmannWealthModel *MoneyAgent::model = nullptr;
shared_ptr<spdlog::logger> console;

template <>
struct fmt::formatter<AgentID> : fmt::formatter<string> {
    auto format(AgentID agentID, format_context &ctx) {
        return format_to(ctx.out(), "{}", agentID.to_string());
    }
};

template <>
struct fmt::formatter<GridCoord1D> : fmt::formatter<string> {
    auto format(GridCoord1D coord, format_context &ctx) {
        return format_to(ctx.out(), "{}", coord.toString());
    }
};

MoneyAgent::MoneyAgent() {
    stepCounter = 0;
    agentWealth = 1;
}

void MoneyAgent::step() {
    stepCounter++;

    moveAgent();
    if (agentWealth > 0)
        giveMoney();
}

void MoneyAgent::setWorld(MultiGrid1D *w) {
    world = w;
}

void MoneyAgent::setModel(class BoltzmannWealthModel *m) {
    model = m;
}

void MoneyAgent::moveAgent() {
    auto agentID = this->get_agent_id();
    auto moveList = world->getNeighborhood(agentID, false);
    auto newLocation = moveList[static_cast<unsigned int>(rand()) % moveList.size()];

    console->trace("Moving Agent {} to location {}", agentID, newLocation);
    world->moveAgent(agentID, newLocation);
}

int MoneyAgent::getWealth() {
    return agentWealth;
}

void MoneyAgent::setWealth(int newWealth) {
    agentWealth = newWealth;
}

void MoneyAgent::giveMoney() {
    AgentID agentID = get_agent_id();
    GridCoord1D location = world->getLocationByAgent(agentID);
    vector<AgentID> *cellMates = world->getCellContents(location);

    if (cellMates->size() > 1) {
        AgentID otherAgentID = cellMates->at(static_cast<unsigned int>(rand()) % cellMates->size());
        auto otherAgent = model->getAgentByID(otherAgentID);

        console->trace("Agent {} giving unit of wealth to agent {}", agentID, otherAgentID);
        otherAgent->agentWealth += 1;
        agentWealth -= 1;
    }
}

void MoneyAgent::prinfo(void) const {
    AgentID agentID = get_agent_id();
    GridCoord1D location = world->getLocationByAgent(agentID);

    console->trace("Agent state for agent {}, step {}, wealth {}, location {}", agentID, stepCounter, agentWealth, location);
}

BoltzmannWealthModel::BoltzmannWealthModel(unsigned int numberAgents, unsigned int lengthX, unsigned int newSeed) {
    world = new MultiGrid1D(lengthX, true);
    sched = new RandomScheduler(this, newSeed);

    console->debug("Scheduler initiated with seed {}", sched->get_seed());

    stepCount = 0;
    MoneyAgent::setWorld(world);
    MoneyAgent::setModel(this);

    for (unsigned int i = 0; i < numberAgents; i++) {
        MoneyAgent *newAgent = new MoneyAgent();

        agentList.insert(pair<AgentID, MoneyAgent *>(newAgent->get_agent_id(), newAgent));
        sched->add_agent(newAgent->get_agent_id());
        world->addAgent(newAgent->get_agent_id(), GridCoord1D(rand() % static_cast<int>(lengthX)));
    }
}

BoltzmannWealthModel::~BoltzmannWealthModel() {
    for (auto agentPair = agentList.begin(); agentPair != agentList.end(); agentPair++) {
        delete agentPair->second;
    }

    delete sched;
    delete world;
}

void BoltzmannWealthModel::step() {
    stepCount++;
    sched->step();
}

void BoltzmannWealthModel::run(unsigned int n) {
    for (unsigned int i = 0; i < n; i++)
        step();
}

void BoltzmannWealthModel::prinfo(void) const {
    console->trace("Model state step {}", stepCount);
    for (auto agent = agentList.begin(); agent != agentList.end(); ++agent) {
        agent->second->prinfo();
    }
}

MoneyAgent *BoltzmannWealthModel::getAgentByID(AgentID agentID) const {
    MoneyAgent *agentPair = agentList.at(agentID);

    return agentPair;
}

int main(int argc, char **argv) {
    string ident = "boltzmann1d";
    CLI::App app{ident};
    string logLevelOption = "info";
    unsigned int xSize = 16, agentCount = xSize, maxSteps = 100, initialSeed = 42;

    app.add_option("-c", agentCount, "Set the number of agents")->check(CLI::PositiveNumber);
    app.add_option("-l", logLevelOption, "Set the logging level")->check(CLI::IsMember(SPDLOG_LEVEL_NAMES));
    app.add_option("-n", maxSteps, "Set the number of steps to run the model")->check(CLI::PositiveNumber);
    app.add_option("-s", initialSeed, "Set the initial seed")->check(CLI::Number);
    app.add_option("-x", xSize, "Set the number of colums")->check(CLI::PositiveNumber);
    CLI11_PARSE(app, argc, argv);

    console = spdlog::stdout_color_st(ident);
    console->set_level(spdlog::level::from_str(logLevelOption));
    console->info("Compiled with Kami/{}, log level {}", KAMI_VERSION_STRING, logLevelOption);
    console->info("Starting Boltzmann Wealth Model with {} agents on a {}-unit grid for {} steps", agentCount, xSize, maxSteps);

    BoltzmannWealthModel model(agentCount, xSize, initialSeed);

    spdlog::stopwatch sw;
    for (int i = 0; i < maxSteps; i++) {
        console->trace("Initiating model step {}", i);
        model.step();
    }
    console->info("Boltzman Wealth Model simulation complete, requiring {} seconds", sw);

    model.prinfo();
}