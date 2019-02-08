#pragma once
#include "ObjectData.h"
struct Point {
	float x;
	float y;
};

struct rectrangle {
	rectrangle(float x=0,float y=0, float width=100,float height=100) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	bool contains(ObjectData point) {
		return(point.position.x > this->x - this->width		&&
			point.position.x	< this->x + this->width		&&
			point.position.y	> this->y - this->height	&&
			point.position.y < this->y + this->height);

	}
	float x;
	float y;
	float width;
	float height;

};

class QuadTree
{
public:
	QuadTree(rectrangle boundary,int cap);
	~QuadTree();
	void insert(ObjectData object);
	bool gotObject();
	std::string toTextSize();
	ObjectData* getObject();
	QuadTree *northEast, *northWest, *southWest, *southEast;

private:
	void Subdivide();
	rectrangle boundary;
	int capacity;
	bool full;
	ObjectData* Object;
	bool divided;
};

QuadTree::QuadTree(rectrangle boundary, int cap = 1)
{
	this->boundary = boundary;
	this->capacity = cap;
	this->divided = false;
	this->full = false;

}

QuadTree::~QuadTree()
{
}

void QuadTree::insert(ObjectData object)
{

	if (this->boundary.contains(object)) {

		if (!full) {
			this->Object = new ObjectData(object);
			full = true;
		}
		else {
			if (!this->divided) {
				this->Subdivide();
			}
			northWest->insert(object);
			northEast->insert(object);
			southWest->insert(object);
			southEast->insert(object);


		}
	}
}

inline bool QuadTree::gotObject()
{
	return full;
}

std::string QuadTree::toTextSize()
{
	std::string temp;
	temp = std::to_string(this->Object->_size) +" \n";

	if (this->divided) {
		if (this->northEast->gotObject())
			temp += "North East block \n" + this->northEast->toTextSize();
		if (this->northWest->gotObject())
			temp += "North West block \n" +this->northWest->toTextSize();
		if (this->southEast->gotObject())
			temp += "SouthEast Block \n" +this->southEast->toTextSize();
		if (this->southWest->gotObject())
			temp += "SouthWest block \n" + this->southWest->toTextSize();
	}
	//std::cout << temp;

	return temp;
}

inline ObjectData* QuadTree::getObject()
{
	return this->Object;
}


 void QuadTree::Subdivide()
{
	 float x, y, w, h;
	 x = this->boundary.x;
	 y = this->boundary.y;
	 w = this->boundary.width;
	 h = this->boundary.height;

	 rectrangle nw(x + w / 2, y - w / 2, w / 2, h / 2);
	 this->northWest = new QuadTree(nw);
	 rectrangle ne(x - w / 2, y - w / 2, w / 2, h / 2);
	 this->northEast = new QuadTree(ne);
	 rectrangle sw(x + w / 2, y + w / 2, w / 2, h / 2);
	 this->southWest = new QuadTree(sw);
	 rectrangle se(x - w / 2, y + w / 2, w / 2, h / 2);
	 this->southEast = new QuadTree(se);

	 this; divided = true;

}
