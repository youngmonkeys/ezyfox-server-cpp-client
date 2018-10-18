#include <map>
#include "EzySocketCore.h"

EZY_NAMESPACE_START_WITH(socket)

static std::map<int, std::string> sSocketStatusNames = {
	{ NotConnection, "NotConnection" },
	{ Connecting, "Connecting" },
	{ Connected, "Connected" },
	{ ConnectFailure, "ConnectFailure" },
	{ LostConnection, "LostConnection" },
	{ Closed, "Closed" },
};

const char* SocketStatusName(int status){
	auto it = sSocketStatusNames.find(status);
	if (it != sSocketStatusNames.end()){
		return it->second.c_str();
	}
	return "";
}

/****/
EzySocketClientStatus::EzySocketClientStatus(){
	mClientStatus = EzySocketStatusType::NotConnection;
}

EzySocketClientStatus::~EzySocketClientStatus(){

}

void EzySocketClientStatus::set(EzySocketStatusType status, bool event){
	std::unique_lock<std::mutex> lk(mStatusMutex);
	if (mClientStatus != status){
		if (event){
			EzySocketStatusData mEvent;
			mEvent.preStatus = mClientStatus;
			mEvent.status = status;
			mStatusEvent.push_back(mEvent);
		}
		mClientStatus = status;
	}
}

EzySocketStatusType EzySocketClientStatus::get() {
	std::unique_lock<std::mutex> lk(mStatusMutex);
	return mClientStatus;
}

void EzySocketClientStatus::popAllStatus(std::vector<EzySocketStatusData> &buffer){
	std::unique_lock<std::mutex> lk(mStatusMutex);
	for (int i = 0; i < mStatusEvent.size(); i++){
		buffer.push_back(mStatusEvent[i]);
	}
	mStatusEvent.clear();
}

void EzySocketClientStatus::clear(){
	std::unique_lock<std::mutex> lk(mStatusMutex);
	mClientStatus = EzySocketStatusType::NotConnection;
	mStatusEvent.clear();
}

EZY_NAMESPACE_END_WITH
