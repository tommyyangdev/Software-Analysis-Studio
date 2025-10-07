//===- Assignment-3.cpp -- Taint analysis ------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013-2022>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/*
 * Graph reachability, Andersen's pointer analysis and taint analysis
 *
 * Created on: Feb 18, 2024
 */

#include "Assignment-3.h"
#include "WPA/Andersen.h"
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace SVF;
using namespace llvm;
using namespace std;

/// TODO: Implement your context-sensitive ICFG traversal here to traverse each program path
/// by matching calls and returns while maintaining a `callstack`.
/// Sources and sinks are identified by implementing and calling `readSrcSnkFromFile`
/// Each path including loops, qualified by a `callstack`, should only be traversed once using a `visited` set.
/// You will need to collect each path from src to snk and then add the path to the `paths` set.
/// Add each path (a sequence of node IDs) as a string into std::set<std::string> paths
/// in the format "START->1->2->4->5->END", where -> indicate an ICFGEdge connects two ICFGNode IDs
static void collectICFGPath(std::set<std::string>& paths, const std::vector<unsigned> &path) {
    std::string newPath = "START->";
    for (auto nodeID : path) { 
        newPath += std::to_string(nodeID);
        newPath += "->";
    }
    newPath += "END";
    std::cout << newPath << std::endl;
    paths.insert(newPath);
}

void ICFGTraversal::reachability(const ICFGNode* src, const ICFGNode* dst) {
    auto newPair = std::make_pair(src, callstack);
    if (visited.find(newPair) != visited.end()) {
        return;
    }
    visited.insert(newPair);
    path.push_back(src -> getId());

    if (src == dst) {
        collectICFGPath(paths, path);
    }
    
    for (const ICFGEdge *edge : src -> getOutEdges()) {
        if (edge -> isIntraCFGEdge()) {
            reachability(edge -> getDstNode(), dst);
        }
        else if (edge -> isCallCFGEdge()) {
            callstack.push_back(edge -> getSrcNode());
            reachability(edge -> getDstNode(), dst);
            callstack.pop_back();
        }
        else if (edge -> isRetCFGEdge()) {
            const RetCFGEdge *RetEdge = SVFUtil::dyn_cast<RetCFGEdge>(edge);
            if (callstack.empty() == false && callstack.back() == RetEdge -> getCallSite()) {
                callstack.pop_back();
                reachability(RetEdge -> getDstNode(), dst);
                callstack.push_back(RetEdge -> getCallSite());
            }
            else if (callstack.empty() == true) {
                reachability(RetEdge -> getDstNode(), dst);
            }
        }
    }
    visited.erase(newPair);
    path.pop_back();
}

/// TODO: Implement your code to parse the two lines to identify sources and sinks from `SrcSnk.txt` for your
/// reachability analysis The format in SrcSnk.txt is in the form of
/// line 1 for sources  "{ api1 api2 api3 }"
/// line 2 for sinks    "{ api1 api2 api3 }"
void ICFGTraversal::readSrcSnkFromFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Hmm... Something went wrong while opening this file: " << filename << '\n';
		return;
	}

	auto parseLine = [](const std::string& lineIn, std::set<std::string>& apiOut) {
		if (lineIn.find("//") != std::string::npos || lineIn.find('#') != std::string::npos) {
			return;
		}
		auto left = lineIn.find('{');
		auto right = lineIn.rfind('}');
		if (left == std::string::npos || right == std::string::npos || right <= left + 1) {
			return;
		}

		std::string inside = lineIn.substr(left + 1, right - left - 1);
		std::istringstream iss(inside);
		std::string tok;
		while (iss >> tok) {
			apiOut.insert(tok);
		}

		if (apiOut.empty()) {
			std::cerr << "We've searched left and right, and didn't find APIs in this line.\n";
			return;
		}
	};

	std::string line;
	checker_source_api.clear();
	checker_sink_api.clear();
	if (std::getline(file, line)) {
		parseLine(line, checker_source_api);
	}
	if (std::getline(file, line)) {
		parseLine(line, checker_sink_api);
	}
	file.close();
}

/// TODO: Implement your Andersen's Algorithm here
/// The solving rules are as follows:
/// p <--Addr-- o        =>  pts(p) = pts(p) ∪ {o}
/// q <--COPY-- p        =>  pts(q) = pts(q) ∪ pts(p)
/// q <--LOAD-- p        =>  for each o ∈ pts(p) : q <--COPY-- o
/// q <--STORE-- p       =>  for each o ∈ pts(q) : o <--COPY-- p
/// q <--GEP, fld-- p    =>  for each o ∈ pts(p) : pts(q) = pts(q) ∪ {o.fld}
/// pts(q) denotes the points-to set of q
void AndersenPTA::solveWorklist() {
    for (ConstraintGraph::const_iterator itBegin = consCG->begin(), itEnd = consCG->end(); itBegin != itEnd; ++itBegin) {
        ConstraintNode* cgNode = itBegin -> second;
        for (ConstraintEdge* addrInEdge : cgNode -> getAddrInEdges()) {
            const AddrCGEdge* addr = SVFUtil::cast<AddrCGEdge>(addrInEdge);
            NodeID dst = addr->getDstID();
            NodeID src = addr->getSrcID();
            if (addPts(dst, src)) {
                pushIntoWorklist(dst);
            }
        }
    }

    while (!isWorklistEmpty()) {
        NodeID p = popFromWorklist();
        ConstraintNode* pNode = consCG->getConstraintNode(p);
        const PointsTo& pPts = getPts(p);

        for (ConstraintEdge* edge : pNode->getOutEdges()) {
            NodeID q = edge -> getDstID();
            const auto edgeKind = edge -> getEdgeKind();

            if (edgeKind == ConstraintEdge::Copy) {
                if (unionPts(q, pPts)) {
                    pushIntoWorklist(q);
                }
            }
            else if (edgeKind == ConstraintEdge::Load) {
                for (NodeID o : pPts) {
                    if (addCopyEdge(o, q)) {
                        if (unionPts(q, getPts(o))) {
                            pushIntoWorklist(q);
                        }
                    }
                }
            }
            else if (edgeKind == ConstraintEdge::Store) {
                const PointsTo& qPts = getPts(q);
                for (NodeID o : qPts) {
                    if (addCopyEdge(p, o)) {
                        if (unionPts(o, pPts)) {
                            pushIntoWorklist(o);
                        }
                    }
                }
            }
            else if (edgeKind == ConstraintEdge::NormalGep || edgeKind == ConstraintEdge::VariantGep) {
                if (unionPts(q, pPts)) {
                     pushIntoWorklist(q);
                }
            }
        }

        for (ConstraintEdge* e : pNode -> getStoreInEdges()) {
            NodeID r = e -> getSrcID();
            const PointsTo& rPts = getPts(r);
            for (NodeID o : rPts) {
                if (addCopyEdge(p, o)) {
                    if (unionPts(o, pPts)) {
                        pushIntoWorklist(o);
                    }
                }
            }
        }
    }
}

/*g = < V,E > !" Constraint Graph
V: a set of nodes in graph
E: a set of edges in graph
WorkList: a vector of nodes
foreach do
    pts(p) = {o}
    pushIntoWorklist(p)
while WorkList ≠ ∅ do
    p !% popFromWorklist()
    foreach o ∈ pts(p) do
        foreach do
        if ∉ E then
            E !% E ∪ { }
            pushIntoWorklist(q)
        foreach do
            if ∉ E then
            E !% E ∪ { }
            pushIntoWorklist(o)
    foreach do
        pts(x) !% pts(x) ∪ pts(p)
        if pts(x) changed then
        pushIntoWorklist(x) */

/// TODO: Checking aliases of the two variables at source and sink. For example:
/// src instruction:  actualRet = source();
/// snk instruction:  sink(actualParm,...);
/// return true if actualRet is aliased with any parameter at the snk node (e.g., via ander->alias(..,..))
bool ICFGTraversal::aliasCheck(const CallICFGNode* src, const CallICFGNode* snk) {
    const RetICFGNode* retNode = src -> getRetICFGNode();
    if (!retNode) return false;

    const SVFVar* actualRet = retNode -> getActualRet();
    if (!actualRet) return false;

    for (const ValVar* actualParm : snk -> getActualParms()) {
        if (ander -> alias(actualRet -> getId(), actualParm -> getId())) {
            return true;
        }
    }
    return false;
}

// Start taint checking.
// There is a tainted flow from p@source to q@sink
// if (1) alias(p,q)==true and (2) source reaches sink on ICFG.
void ICFGTraversal::taintChecking() {
	const fs::path& config = CUR_DIR() / "Tests/SrcSnk.txt";
	// configure sources and sinks for taint analysis
	readSrcSnkFromFile(config);

	// Set file permissions to read-only for user, group and others
	if (chmod(config.string().c_str(), S_IRUSR | S_IRGRP | S_IROTH) == -1) {
		std::cerr << "Error setting file permissions for " << config << ": " << std::strerror(errno) << std::endl;
		abort();
	}
	ander = new AndersenPTA(pag);
	ander->analyze();
	for (const CallICFGNode* src : identifySources()) {
		for (const CallICFGNode* snk : identifySinks()) {
			if (aliasCheck(src, snk))
				reachability(src, snk);
		}
	}
}

/*!
 * Andersen analysis
 */
void AndersenPTA::analyze() {
	initialize();
	initWorklist();
	do {
		reanalyze = false;
		solveWorklist();
		if (updateCallGraph(getIndirectCallsites()))
			reanalyze = true;
	} while (reanalyze);
	finalize();
}