#include <thread>
#include <chrono>
#include "EzySocketAdapter.h"
#include "../logger/EzyLogger.h"
#include "../entity/EzyValue.h"
#include "../entity/EzyJson.h"
#include "../entity/EzyObject.h"
#include "../pool/EzyAutoReleasePool.h"

EZY_NAMESPACE_START
namespace socket {

EzySocketPool::EzySocketPool(){
	mDataQueue = new std::queue<EzySocketData*>();
}

EzySocketPool::~EzySocketPool(){
	if (mDataQueue){
		while (!mDataQueue->empty()) {
			EzySocketData* data = mDataQueue->front();
			if (data){
				//delete data;
				data->release();
			}
			mDataQueue->pop();
		}

		delete mDataQueue;
		mDataQueue = 0;
	}

}

void EzySocketPool::push(EzySocketData* data){
	std::unique_lock<std::mutex> lk(mPoolMutex);
	if (mDataQueue){
		data->retain();
		mDataQueue->push(data);
	}

	mPoolCondition.notify_one();
}

void EzySocketPool::clear(){
	std::unique_lock<std::mutex> lk(mPoolMutex);
	if (mDataQueue){
		while (!mDataQueue->empty()) {
			EzySocketData* data = mDataQueue->front();
			//delete data;
			data->release();
			mDataQueue->pop();
		}
	}

	mPoolCondition.notify_all();
}

EzySocketData* EzySocketPool::take(){
	std::unique_lock<std::mutex> lk(mPoolMutex);
	if (mDataQueue){
		if (!mDataQueue->empty()){
			EzySocketData* data = mDataQueue->front();
			mDataQueue->pop();

			//data->retain();
			data->autorelease();
			return data;
		}

		mPoolCondition.wait(lk);

		if (mDataQueue && !mDataQueue->empty()){
			EzySocketData* data = mDataQueue->front();
			mDataQueue->pop();

			//data->retain();
			data->autorelease();
			return data;
		}
		else{
			return 0;
		}
	}

	return 0;
}

EzySocketData* EzySocketPool::pop() {
	std::unique_lock<std::mutex> lk(mPoolMutex);
	if (mDataQueue && !mDataQueue->empty()){
		auto data = mDataQueue->front();
		mDataQueue->pop();

		//data->retain();
		data->autorelease();
		return data;
	}
	return 0;
}

/****/
EzySocketAdapter::EzySocketAdapter() {
	mRunning = false;
	mSocketPool = 0;
}

EzySocketAdapter::~EzySocketAdapter() {
	EZY_DELETE_0(mSocketPool)
}

void EzySocketAdapter::updateThread(){
	this->update();
    pool::EzyAutoReleasePool::getInstance()->removePool();
	this->release();
}

bool EzySocketAdapter::isRunning() {
	std::unique_lock<std::mutex> lk(mMutex);
	return mRunning;
}

void EzySocketAdapter::setRunning(bool running) {
	std::unique_lock<std::mutex> lk(mMutex);
	this->mRunning = running;
}

void EzySocketAdapter::start() {
	if (!isRunning()){
		this->setRunning(true);
		//running = true;

		this->retain();
		std::thread newThread(&EzySocketAdapter::updateThread, this);
		newThread.detach();
	}
}

void EzySocketAdapter::stop(){
	this->setRunning(false);
	mSocketPool->clear();
}

void EzySocketAdapter::update(){
}

void EzySocketAdapter::pushMessage(EzySocketData* data){
	mSocketPool->push(data);
}

EzySocketData* EzySocketAdapter::popMessage(){
	return mSocketPool->pop();
}

EzySocketWriter::EzySocketWriter(){
	mSocketPool = new EzySocketPool();
    mEncoder = new codec::EzyDataEncoder();
}

EzySocketWriter::~EzySocketWriter(){
	delete mEncoder;
}

void EzySocketWriter::toBufferData(EzySocketData* data){
	mEncoder->clear();
	data->writeToBuffer(mEncoder);

#ifdef USE_MESSAGE_HEADER
	uint32_t dataSize = writer->getSize();
	dataSize = htonl(dataSize);
	writer->insertHeader((const char*) &dataSize, 4);
#endif

#ifdef EZY_DEBUG
	logger::console("\n----------------\n");
	logger::console("[SEND] <==\n");
	data->printDebug();
	logger::console("\n----------------\n");
#endif
}


/****/
EzySocketReader::EzySocketReader(){
	mSocketPool = new EzySocketPool();
    mDecoder = new codec::EzyDataDecoder();
	mDecoder->setDelegate(this);
}

EzySocketReader::~EzySocketReader(){
	EZY_DELETE_0(mDecoder)
}

void EzySocketReader::updateThread(){
#ifdef USE_MESSAGE_HEADER
	recvHeader = true;
	dataSize = 0;
	byteBuffer.reserve(100 * 1024); // 100KB RAM
#endif
	EzySocketAdapter::updateThread();
}

void EzySocketReader::recvData(const char* data, int size){
	if (size <= 0){
		return;
	}
#ifdef USE_MESSAGE_HEADER
	byteBuffer.insert(byteBuffer.end(), data, data + size);
	this->onRecvData();
#else
	mDecoder->addData(data, size);
#endif
}


#ifdef USE_MESSAGE_HEADER
void EzySocketReader::onRecvData(){
	if (byteBuffer.size() <= 0){
		return;
	}
	if (recvHeader){
		onUpdateDataHeader();
	}
	else{
		onUpdateData();
	}
}

void EzySocketReader::onUpdateDataHeader(){
	if (byteBuffer.size() >= 4){
		memcpy(&dataSize, byteBuffer.data(), sizeof(dataSize));
		dataSize = ntohl(dataSize);

		recvHeader = false;
		byteBuffer.erase(byteBuffer.begin(), byteBuffer.begin() + 4);
		this->onRecvData();
	}
}

void EzySocketReader::onUpdateData(){
	if (byteBuffer.size() >= dataSize){
		reader->addData(byteBuffer.data(), dataSize);
		recvHeader = true;
		byteBuffer.erase(byteBuffer.begin(), byteBuffer.begin() + dataSize);
		this->onRecvData();
	}
}
#endif

void EzySocketReader::onRecvMessage(entity::EzyValue* value){
	if (!value){
#ifdef EZY_DEBUG
		logger::log("error parse data");
#endif
		this->setRunning(false);
		return;
	}
	
#ifdef EZY_DEBUG
	logger::console("\n-------------------\n");
	logger::console("[RECV] ==>\n");
	value->printDebug();
	logger::console("\n-------------------\n");
#endif		
    this->pushMessage(entity::EzyJson::create(value));
}

/**/
EzySocketClient::EzySocketClient() {
    mPort = 0;
    mHost = "";
	mConnectTime = 0;
	mSocketReader = 0;
	mSocketWriter = 0;
	mRecvCallback = nullptr;
    mStatusCallback = nullptr;
	mReleasePool = 0;
}

EzySocketClient::~EzySocketClient() {
	this->closeSocket();
	this->clearAdapter();
}

void EzySocketClient::closeSocket() {

}

void EzySocketClient::createAdapter() {

}

bool EzySocketClient::connectThread() {
	return false;
}

void EzySocketClient::connectTo(const std::string& host, int port){
	clearAdapter();
	createAdapter();
	closeSocket();

	mClientStatus.clear();
	this->setStatus(EzySocketStatusType::Connecting);
	this->mHost = host;
	this->mPort = port;

	this->retain();
	std::thread newThread(&EzySocketClient::updateConnection, this);
	newThread.detach();
}

void EzySocketClient::updateConnection(){
	auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto dt = currentTime - mConnectTime;
	if (dt < 2000){ //delay 2000ms
		std::this_thread::sleep_for(std::chrono::milliseconds(2000 - dt));
	}

	bool b = this->connectThread();
	mConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	if (b){
		this->startAdapter();
		this->setStatus(EzySocketStatusType::Connected);
	}
	else{
		this->resetSocket();
		if (this->getStatus() == EzySocketStatusType::Connecting){
			this->setStatus(EzySocketStatusType::ConnectFailure);
		}
	}

    pool::EzyAutoReleasePool::getInstance()->removePool();
	this->release();
}

void EzySocketClient::startAdapter(){

}

void EzySocketClient::closeClient(){
	if (mSocketWriter) {
		mSocketWriter->stop();
	}
    if(mSocketReader) {
        mSocketReader->stop();
    }

	mClientStatus.clear();
	this->setStatus(EzySocketStatusType::Closed);
	closeSocket();
}

EzySocketStatusType EzySocketClient::getStatus(){
	return mClientStatus.get();
}

void EzySocketClient::setStatus(EzySocketStatusType status, bool isEvent){
	mClientStatus.set(status, isEvent);
}

void EzySocketClient::sendMessage(EzySocketData* data){
	if (mSocketWriter){
		mSocketWriter->pushMessage(data);
	}
}

void EzySocketClient::processEvent(){
	if (mStatusCallback){
		mClientStatus.popAllStatus(mStatusBuffer);
		for (int i = 0; i < mStatusBuffer.size(); i++){
			mStatusCallback(mStatusBuffer[i]);
		}
		mStatusBuffer.clear();
	}
}

void EzySocketClient::processRecvMessage(){
	if (mRecvCallback){
		auto data = mSocketReader->popMessage();
		while (data){
			if (this->getStatus() != EzySocketStatusType::Connected){
				break;
			}
			mRecvCallback(data);
			data = mSocketReader->popMessage();
		}
	}
}

void EzySocketClient::clearAdapter(){
	std::unique_lock<std::mutex> lk(mClientMutex);
	if (mSocketReader){
		mSocketReader->stop();
		mSocketReader->release();
		mSocketReader = 0;
	}
	if (mSocketWriter){
		mSocketWriter->stop();
		mSocketWriter->release();
		mSocketWriter = 0;
	}
}

void EzySocketClient::resetSocket(){

}

void EzySocketClient::processSocketError(){
	if (this->getStatus() == EzySocketStatusType::Connected){
		this->setStatus(EzySocketStatusType::LostConnection);
	}
}

void EzySocketClient::processMessage(){
	if (this->getStatus() == EzySocketStatusType::Connected){
		if (mSocketReader && mSocketWriter){
			processRecvMessage();
			if (!mSocketReader->isRunning()){
				if (this->getStatus() == EzySocketStatusType::Connected){
					this->processSocketError();
					this->closeSocket();
					this->clearAdapter();
				}
			}
		}
	}

	processEvent();

	if (!mReleasePool){
        mReleasePool = pool::EzyAutoReleasePool::getInstance()->getPool();
	}
	mReleasePool->releaseAll();
}

}
EZY_NAMESPACE_END
