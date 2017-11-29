#include <map>
#include "EzySocketCore.h"

EZY_NAMESPACE_START
namespace socket {

static std::map<int, std::string> s_socketstatus_name = {
	{ NotConnection, "NotConnection" },
	{ Connecting, "Connecting" },
	{ Connected, "Connected" },
	{ ConnectFailure, "ConnectFailure" },
	{ LostConnection, "LostConnection" },
	{ Closed, "Closed" },
};

const char* SocketStatusName(int status){
	auto it = s_socketstatus_name.find(status);
	if (it != s_socketstatus_name.end()){
		return it->second.c_str();
	}
	return "";
}

/****/
EzySocketClientStatus::EzySocketClientStatus(){
	//	mStatusCallback = nullptr;
	clientStatus = EzySocketStatusType::NotConnection;
}

EzySocketClientStatus::~EzySocketClientStatus(){

}

void EzySocketClientStatus::set(EzySocketStatusType status, bool isEvent){
	std::unique_lock<std::mutex> lk(statusMutex);
	//statusMutex.lock();

	if (clientStatus != status){
		if (isEvent){
			EzySocketStatusData mEvent;
			mEvent.preStatus = clientStatus;
			mEvent.status = status;
			statusEvent.push_back(mEvent);
		}

		clientStatus = status;
	}
	//statusMutex.unlock();
}

EzySocketStatusType EzySocketClientStatus::get() {
	std::unique_lock<std::mutex> lk(statusMutex);
	return clientStatus;
}

void EzySocketClientStatus::popAllStatus(std::vector<EzySocketStatusData> &buffer){
	std::unique_lock<std::mutex> lk(statusMutex);
	for (int i = 0; i < statusEvent.size(); i++){
		buffer.push_back(statusEvent[i]);
	}
	statusEvent.clear();
}

void EzySocketClientStatus::clear(){
	std::unique_lock<std::mutex> lk(statusMutex);
	clientStatus = EzySocketStatusType::NotConnection;
	statusEvent.clear();
}

}
EZY_NAMESPACE_END
