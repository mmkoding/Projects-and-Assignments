#ifndef __EDGE_H__
#define __EDGE_H__

#include <string>
#include <vector>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////
//    DO NOT CHANGE *PUBLIC PARTS* IN THIS FILE!    //
//////////////////////////////////////////////////////

enum Password 
{   SEND_ME_STRAIGHT,               // password for StraightEdge
    SEND_ME_OUTWARDS,               // password for InwardsEdge
    SEND_ME_INWARDS,                // password for OutwardsEdge
    SEND_ME_COMPLETING_COMPOSITE    // password for CompositeEdge
};


/****************************************************/
/****                *** EDGE ***                ****/
class Edge {

private: 

protected:
	int id;
	Password password;
	Edge* matchingEdge;

	Edge();

    // DO NOT MODIFY BELOW
public:
	Edge(int, Password);    
	virtual ~Edge();
	int getId() const;
	Edge* getMatchingEdge() const;
	virtual Edge* clone() const = 0;
	virtual bool matchWith(Edge&);
	virtual bool matchWithHelper(Password) = 0;
	virtual void breakMatch();
};

/****************************************************/
/****            *** STRAIGHT EDGE ***           ****/
class StraightEdge : public Edge {

	bool matchWithHelper(Password);
	StraightEdge();

    // DO NOT MODIFY BELOW
public:
	StraightEdge(int);
	~StraightEdge();
	Edge* clone() const;
};

/****************************************************/
/****            *** INWARDS EDGE ***            ****/
class InwardsEdge : public Edge {

	bool matchWithHelper(Password);

	InwardsEdge();

    // DO NOT MODIFY BELOW
public:
	InwardsEdge(int);
	~InwardsEdge();
	Edge* clone() const;
};

/****************************************************/
/****            *** OUTWARDS EDGE ***           ****/
class OutwardsEdge : public Edge {

	bool matchWithHelper(Password);

	OutwardsEdge();

        // DO NOT MODIFY BELOW
public:
	OutwardsEdge(int);
	~OutwardsEdge();
	Edge* clone() const;
};

/****************************************************/
/****            *** COMPOSITE EDGE ***          ****/
class CompositeEdge : public Edge {
    
	vector<Edge*> edges;
	int matchIndex;

	bool matchWithHelper(Password);

        // DO NOT MODIFY BELOW
public:
	CompositeEdge(int);
	~CompositeEdge();
	Edge* clone() const;
	bool matchWith(Edge&);
	void breakMatch();
	CompositeEdge* addEdge(Edge*);
};

#endif