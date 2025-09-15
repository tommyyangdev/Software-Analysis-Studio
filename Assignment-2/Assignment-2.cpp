//===- SVF-Teaching Assignment 2-------------------------------------//
//
//     SVF: Static Value-Flow Analysis Framework for Source Code
//
// Copyright (C) <2013->
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===-----------------------------------------------------------------------===//
/*
 // SVF-Teaching Assignment 2 : Source Sink ICFG DFS Traversal
 //
 // 
 */

#include <set>
#include "Assignment-2.h"
#include <iostream>
using namespace SVF;
using namespace std;

/// TODO: print each path once this method is called, and
/// add each path as a string into std::set<std::string> paths
/// Print the path in the format "START->1->2->4->5->END", where -> indicate an ICFGEdge connects two ICFGNode IDs

void ICFGTraversal::collectICFGPath(std::vector<unsigned> &path) {
    std::string newPath = "START->";
    for (auto nodeID : path) { 
        newPath += std::to_string(nodeID);
        newPath += "->";
    }
    newPath += "END";
    std::cout << newPath << std::endl;
    paths.insert(newPath);
}


/// TODO: Implement your context-sensitive ICFG traversal here to traverse each program path (once for any loop) from src to dst
void ICFGTraversal::reachability(const ICFGNode *src, const ICFGNode *dst) {
    auto newPair = std::make_pair(src, callstack);
    if (visited.find(newPair) != visited.end()) {
        return;
    }
    visited.insert(newPair);
    path.push_back(src -> getId());

    if (src == dst) {
        collectICFGPath(path);
    }
    
    for (const ICFGEdge *edge : src->getOutEdges()) {
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

/*pair = ⟨curNode, callstack⟩;
    if pair ∈ visited then
        return;
    visited.insert(pair);
    path.push back(curNode);

    if src == snk then
        collectICFGPath(path);
    foreach edge ∈ curNode.getOutEdges() do
        if edge.isIntraCFGEdge() then
            reachability(edge.dst, snk);
        else if edge.isCallCFGEdge() then
            callstack.push back(edge.src);
            reachability(edge.dst, snk);
            callstack.pop back();
        else if edge.isRetCFGEdge() then
            if callstack ̸ = ∅ && callstack.back() == edge.getCallSite() then
                callstack.pop back();
                reachability(edge.dst, snk);
                callstack.push back(edge.getCallSite());
            else if callstack == ∅ then
                reachability(edge.dst, snk);
    visited.erase(pair);
    path.pop back();*/