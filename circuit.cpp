#include "circuit.h"
#include "utility.h"
#include <iostream> // TODO: Used for testing: note may want to git grep to remove these at some point
#include <map>

void Circuit::addGate(Gate *newGate){
  gates.push_back(newGate);
}

void Circuit::addGate(Gate *newGate, int pos){
  gates.insert(gates.begin()+pos,newGate);
}

Gate* Circuit::getGate(int pos){
  return gates.at(pos);
}

int Circuit::numGates(){
  return gates.size();
}

int Circuit::QCost(){
  int totalCost =0;
  for(int i = 0; i < numGates(); i++){
    totalCost = totalCost + getGate(i)->QCost(numLines());
  }
  return totalCost;
}

string Line::getInputLabel(){
  if (constant){
    return intToString(initValue);
  }
  return lineName;
}

string  Line::getOutputLabel(){
  if (garbage){
    return "Garbage";
  }
  if (outLabel.compare("")==0){
    return lineName;
  }
  return outLabel;
}

int Circuit::numLines(){
  return lines.size();
}

Line* Circuit::getLine(int pos){
  return &lines.at(pos);
}

Line::Line(string name){
  lineName  = name;
  garbage   = true;
  constant  = true;
  initValue = 0;
}

void Circuit::addLine(string lineName){
  lines.push_back(Line(lineName));
}

vector<int> Circuit::getParallel(){
	vector<int>	returnValue;
	map<int,int> linesUsed;
	int test;
	for(int i = 0; i<numGates(); i++){
		start:
		for(int j = 0; j < getGate(i)->controls.size(); j++){
			if (linesUsed.find(getGate(i)->controls.at(j).wire) != linesUsed.end()){
				returnValue.push_back(i-1); //Push back the gate number before this one
				linesUsed.clear();
				goto start; //Go back to begining of main loop (redo this iteration because this gate is in the next block)
			}
			linesUsed[getGate(i)->controls.at(j).wire];
		}
		for(int j = 0; j < getGate(j)->targets.size(); j++){
			if (linesUsed.find(getGate(i)->targets.at(j)) != linesUsed.end()){
				returnValue.push_back(i-1); 
				linesUsed.clear();
				goto start;
			}
			linesUsed[getGate(i)->targets.at(j)];
		}
	}
	return returnValue;
}
