#include "ID.h"

LWM::local_ptr<CID> IDInstance;

void CID::Initialize()
{
	IDInstance.reset(new CID);
}

void CID::Destroy()
{
	IDInstance.reset();
}

LWM::local_ptr<CID> CID::Get()
{
	return IDInstance;
}

CID::CID()
{
	highest_id = 0;
}

CID::~CID()
{
	highest_id = 0;
	while(!removed_ids.empty()) removed_ids.pop();
}

TimerID CID::Generate()
{
	TimerID id;
	if (!removed_ids.empty()) {
		id = removed_ids.front();
		removed_ids.pop();
	} else {
		id = highest_id++;
	}
	return id;
}

void CID::Remove(TimerID id)
{
	removed_ids.push(id);
}