//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// 
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

#ifndef __player_h_
#define __player_h_

#include "definitions.h"
#include "creature.h"
#include "container.h"

#include <vector>
#include <ctime>
#include <algorithm>

class Protocol;

enum skills_t {
    SKILL_FIST=0,
    SKILL_CLUB=1,
    SKILL_SWORD=2,
    SKILL_AXE=3,
    SKILL_DIST=4,
    SKILL_SHIELD=5,
    SKILL_FISH=6
};

enum skillsid_t {
    SKILL_LEVEL=0,
    SKILL_TRIES=1,
    SKILL_PERCENT=2
};

class NetworkMessage;

typedef std::pair<unsigned char, Container*> containerItem;
typedef std::vector<containerItem> containerLayout;
typedef std::map<unsigned long, Container*> DepotMap;
typedef std::map<unsigned long,long> StorageMap;

//////////////////////////////////////////////////////////////////////
// Defines a player...

class Player : public Creature
{
public:
	Player(const char *name, Protocol* p);
	virtual ~Player();
	void setGUID(unsigned long _guid) {guid = _guid;};
	unsigned long getGUID() const { return guid;};
	virtual unsigned long idRange(){ return 0x10000000;}

  void speak(const std::string &text);	
	bool addItem(Item* item);
	bool internalAddItemContainer(Container *container,Item* item);
	//bool removeItem(unsigned short id,unsigned short count);
	//bool removeItem(Item* item);
	//int getItemCount(unsigned short id);

	int removeItemInventory(int pos, bool internal = false);
	int addItemInventory(Item* item, int pos, bool internal = false);

	containerLayout::const_iterator getContainers() const { return vcontainers.begin();}
	containerLayout::const_iterator getEndContainer() const { return vcontainers.end();}

	Container* getContainer(unsigned char containerid);
	unsigned char getContainerID(const Container* container) const;
	void addContainer(unsigned char containerid, Container *container);
	void closeContainer(unsigned char containerid);

	void addStorageValue(const unsigned long key, const long value);
	bool getStorageValue(const unsigned long key, long &value) const;
	inline StorageMap::const_iterator getStorageIteratorBegin() const {return storageMap.begin();}
	inline StorageMap::const_iterator getStorageIteratorEnd() const {return storageMap.end();}

	Item* getItem(int pos) const;
	Item* GetDistWeapon() const;
	
	const std::string& getName() const {return name;};
	
	//bool pendingCancelAutoWalk;
	unsigned long eventAutoWalk;
  int sex, voc;
  int cap;
  int food;
  void addManaspent(unsigned long spent);
  void addExp(unsigned long exp);
  unsigned char level_percent;
  unsigned char maglevel_percent;
  //bool cancelMove;
  bool SendBuffer;
  long internal_ping;
  long npings;
  virtual int getWeaponDamage() const;
  virtual int getArmor() const;
  virtual int getDefense() const;
  unsigned long getMoney();
  unsigned long getMoneyContainer(Container *container);
  bool substractMoney(unsigned long money);
  bool substractMoneyContainer(Container *container, unsigned long *money);
  char fightMode, followMode;
  int accountNumber;
  time_t lastlogin;
  
  std::string password;

  Item* items[11]; //equipement of the player
  unsigned int skills[7][3];
  
  //reminder: 0 = None, 1 = Sorcerer, 2 = Druid, 3 = Paladin, 4 = Knight
  unsigned short CapGain[5];          //for level advances
  unsigned short ManaGain[5];
  unsigned short HPGain[5];
  
  //for skill advances
  unsigned int getReqSkilltries (int skill, int level, int voc);
  
  //for magic level advances
  unsigned int getReqMana(int maglevel, int voc); 
  	
	//items
	containerLayout vcontainers;
  void preSave();
    virtual void useThing() {
		//std::cout << "Player: useThing() " << this << std::endl;
		useCount++;
	};
	
	virtual void releaseThing() {
		useCount--;
		//std::cout << "Player: releaseThing() " << this << std::endl;
		if (useCount == 0)
			delete this;
	};

	unsigned long getIP() const;
	Container* getDepot(unsigned long depotId);
	bool addDepot(Container* depot,unsigned long depotIs);
	//depots	
	DepotMap depots;

	void RemoveDistItem();
  fight_t getFightType();
	subfight_t getSubFightType();

  bool CanSee(int x, int y, int z) const;

  void sendIcons();  
  void sendNetworkMessage(NetworkMessage *msg);
  void sendCancelAttacking();
  void sendChangeSpeed(Creature* creature);
  void sendToChannel(Creature *creature, SpeakClasses type, const std::string &text, unsigned short channelId);
  virtual void sendCancel(const char *msg) const;
  virtual void sendCancelWalk() const;
	//virtual void sendCancelAutoWalking();
  int sendInventory(unsigned char sl_id);
  void sendStats();
  void sendTextMessage(MessageClasses mclass, const char* message) const;
  void sendTextMessage(MessageClasses mclass, const char* message,const Position &pos, unsigned char type) const;
  void sendPing();
  void sendCloseContainer(unsigned char containerid);
  void sendContainer(unsigned char index, Container *container);
  void sendTextWindow(Item* item,const unsigned short maxlen, const bool canWrite);  
  void sendDistanceShoot(const Position &from, const Position &to, unsigned char type);
  void sendMagicEffect(const Position &pos, unsigned char type);
  void sendAnimatedText(const Position &pos, unsigned char color, std::string text);
  void sendCreatureHealth(const Creature *creature);
	void sendTradeItemRequest(const Player* player, const Item* item, bool ack);
	void sendCloseTrade();
  void receivePing();
  void flushMsg();

  void addSkillTry(int skilltry);
  void addSkillShieldTry(int skilltry);
  void die();      //player loses exp/skills/maglevel on death

  virtual void setAttackedCreature(unsigned long id);
  virtual bool isAttackable() const { return (access == 0); };
  virtual bool isPushable() const;
	virtual void dropLoot(Container *corpse);
	virtual int getLookCorpse();
	bool NeedUpdateStats();	
	//ground	
	void onThingAppear(const Thing* thing);  
	void onThingTransform(const Thing* thing,int stackpos);
	void onThingDisappear(const Thing* thing, unsigned char stackPos);
	void onThingRemove(const Thing* thing); //auto-close containers
	//container
	void onItemAddContainer(const Container* container,const Item* item);
	void onItemRemoveContainer(const Container* container,const unsigned char slot);
	void onItemUpdateContainer(const Container* container,const Item* item,const unsigned char slot);
	//invnetory - for this use int sendInventory(unsigned char sl_id)
	//void onItemAddInvnetory(const unsigned char sl_id);
	//void onItemRemoveInvnetory(const unsigned char sl_id);
	//void onItemUpdateInvnetory(const unsigned char sl_id);
	
	void setAcceptTrade(bool b);
	bool getAcceptTrade() {return acceptTrade;};
	Item* getTradeItem() {return tradeItem;};

protected:
  int useCount;
	unsigned long tradePartner;
	bool acceptTrade;
	Item *tradeItem;
	std::list<Direction> pathlist;
	
	long long getSleepTicks() const;
	virtual void onCreatureAppear(const Creature *creature);
  virtual void onCreatureDisappear(const Creature *creature, unsigned char stackPos, bool tele);
  virtual void onCreatureTurn(const Creature *creature, unsigned char stackpos);
  virtual void onCreatureSay(const Creature *creature, SpeakClasses type, const std::string &text);
  virtual void onCreatureChangeOutfit(const Creature* creature);
  virtual void onTeleport(const Creature *creature, const Position *oldPos, unsigned char oldstackpos); 
  virtual int onThink(int& newThinkTicks);
  //virtual void onPlace();
  //virtual void onRemove();
  virtual std::string getDescription(bool self = false) const;
	virtual void onTileUpdated(const Position &pos);

	//container to container
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, Container *toContainer, unsigned char to_slotid,
		const Item *toItem, int oldToCount, int count);

	//inventory to container
	virtual void onThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
		int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count);

	//inventory to inventory
	virtual void onThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
		int oldFromCount, slots_t toSlot, const Item* toItem, int oldToCount, int count);

	//container to inventory
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count);

	//container to ground
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count);

	//inventory to ground
	virtual void onThingMove(const Creature *creature, slots_t fromSlot,
		const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count);

	//ground to container
	virtual void onThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
		int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count);

	//ground to inventory
	virtual void onThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
		int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count);

  //ground to ground
	virtual void onThingMove(const Creature *creature, const Thing *thing, const Position *oldPos,
		unsigned char oldstackpos, unsigned char oldcount, unsigned char count);

	void addSkillTryInternal(int skilltry,int skill);
	std::string getSkillName(int skillid);

  friend class Game;
  friend class ActionScript;
  friend class Map;

	Protocol *client;
	//cache some data
	struct SkillCache{
		unsigned int tries;
		int level;
		int voc;
	};
	SkillCache SkillAdvanceCache[7][2];
	struct SentStats{
		int health;
		int healthmax;
		unsigned long experience;
		int level;
		int cap;
		int mana;
		int manamax;
		int manaspent;
		int maglevel;
	};
	SentStats lastSentStats;
	// we need our name
	std::string name;	
	unsigned long guid;
	
	StorageMap storageMap;
	
	friend OTSYS_THREAD_RETURN ConnectionHandler(void *dat);
};


#endif // __player_h_
