#pragma once

#include "EzySocketDefine.h"
#include "../EzyMacro.h"
#include "../entity/EzyValue.h"

EZY_NAMESPACE_START
namespace socket {

enum EzySocketStatusType {
	NotConnection = 0,
	Connecting, //1
	Connected, //2
	ConnectFailure, //3
	LostConnection, //4
	Closed //5
};

const char* EzySocketStatusName(int status);

struct EzySocketStatusData {
	EzySocketStatusType preStatus;
	EzySocketStatusType status;
};

class EzySocketClientStatus {
	EzySocketStatusType clientStatus;
	std::mutex statusMutex;
	std::vector<EzySocketStatusData> statusEvent;
public:
	EzySocketClientStatus();
	~EzySocketClientStatus();

	void set(EzySocketStatusType status, bool isEvent);
	EzySocketStatusType get();

	void popAllStatus(std::vector<EzySocketStatusData> &buffer);
	void clear();
};

typedef entity::EzyValue EzySocketData;

}
EZY_NAMESPACE_END

