//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// The database of items.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


#ifndef __OTSERV_ITEMS_H
#define __OTSERV_ITEMS_H

#include <map>
#include <string>

enum WeaponType 
{
  NONE, SWORD, CLUB, AXE, DIST, MAGIC, AMO, SHIELD
};

class ItemType {
public:
	ItemType();
	~ItemType();

	unsigned short id;

  unsigned short maxItems;   // maximum size if this is a container
	unsigned short weight;     // weight of the item, e.g. throwing distance depends on it
	std::string    name;		   // the name of the item
	std::string description;	// additional description... as in "The blade is a magic flame." for fireswords
  WeaponType     weaponType;
  int            attack;
  int            defence;

  unsigned short decayTo;
  unsigned short decayTime;

  uint8_t speed;


	// other bools
	bool iscontainer;
	bool stackable;
	bool multitype;
	bool useable;
	bool notMoveable;
	bool alwaysOnTop;
	bool groundtile; 
	bool blocking;   // people can walk on it
	bool pickupable; // people can pick it up
};


class Items {
public:
	Items();
	~Items();
	
	int loadFromDat(std::string);
  int loadXMLInfos(std::string);
	
	const ItemType& operator[](int id);
		 
protected:
	typedef std::map<unsigned short, ItemType*> ItemMap;
	ItemMap items;

	ItemType dummyItemType; // use this for invalid ids
};

#endif









