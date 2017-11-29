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
	socketDataQueue = new std::queue<EzySocketData*>();
}

EzySocketPool::~EzySocketPool(){
	if (socketDataQueue){
		while (!socketDataQueue->empty()) {
			EzySocketData* data = socketDataQueue->front();
			if (data){
				//delete data;
				data->release();
			}
			socketDataQueue->pop();
		}

		delete socketDataQueue;
		socketDataQueue = 0;
	}

}

void EzySocketPool::push(EzySocketData* data){
	std::unique_lock<std::mutex> lk(poolMutex);
	if (socketDataQueue){
		data->retain();
		socketDataQueue->push(data);
	}

	poolCondition.notify_one();
}

void EzySocketPool::clear(){
	std::unique_lock<std::mutex> lk(poolMutex);
	if (socketDataQueue){
		while (!socketDataQueue->empty()) {
			EzySocketData* data = socketDataQueue->front();
			//delete data;
			data->release();
			socketDataQueue->pop();
		}
	}

	poolCondition.notify_all();
}

EzySocketData* EzySocketPool::take(){
	std::unique_lock<std::mutex> lk(poolMutex);
	if (socketDataQueue){
		if (!socketDataQueue->empty()){
			EzySocketData* data = socketDataQueue->front();
			socketDataQueue->pop();

			//data->retain();
			data->autorelease();
			return data;
		}

		poolCondition.wait(lk);

		if (socketDataQueue && !socketDataQueue->empty()){
			EzySocketData* data = socketDataQueue->front();
			socketDataQueue->pop();

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
	std::unique_lock<std::mutex> lk(poolMutex);
	if (socketDataQueue && !socketDataQueue->empty()){
		auto data = socketDataQueue->front();
		socketDataQueue->pop();

		//data->retain();
		data->autorelease();
		return data;
	}
	return 0;
}

/****/
EzySocketAdapter::EzySocketAdapter() {
	running = false;
	socketPool = 0;
}

EzySocketAdapter::~EzySocketAdapter() {
	EZY_DELETE_0(socketPool)
}

void EzySocketAdapter::updateThread(){
	this->update();
    pool::EzyAutoReleasePool::getInstance()->removePool();
	this->release();
}

bool EzySocketAdapter::isRunning() {
	std::unique_lock<std::mutex> lk(mMutex);
	return running;
}

void EzySocketAdapter::setRunning(bool running) {
	std::unique_lock<std::mutex> lk(mMutex);
	this->running = running;
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
	socketPool->clear();
}

void EzySocketAdapter::update(){
}

void EzySocketAdapter::pushMessage(EzySocketData* data){
	socketPool->push(data);
}

EzySocketData* EzySocketAdapter::popMessage(){
	return socketPool->pop();
}

EzySocketWriter::EzySocketWriter(){
	socketPool = new EzySocketPool();
    encoder = new codec::EzyDataEncoder();
}

EzySocketWriter::~EzySocketWriter(){
	delete encoder;
}

void EzySocketWriter::toBufferData(EzySocketData* data){
	encoder->clear();
	data->writeToBuffer(encoder);

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
	socketPool = new EzySocketPool();
    decoder = new codec::EzyDataDecoder();
	decoder->setDelegate(this);
}

EzySocketReader::~EzySocketReader(){
	EZY_DELETE_0(decoder)
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
	decoder->addData(data, size);
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
    port = 0;
    host = "";
	connectTime = 0;
	socketReader = 0;
	socketWriter = 0;
	recvCallback = nullptr;
    statusCallback = nullptr;
	releasePool = 0;
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

	clientStatus.clear();
	this->setStatus(EzySocketStatusType::Connecting);
	this->host = host;
	this->port = port;

	this->retain();
	std::thread newThread(&EzySocketClient::updateConnection, this);
	newThread.detach();
}

void EzySocketClient::updateConnection(){
	auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto dt = currentTime - connectTime;
	if (dt < 2000){ //delay 2000ms
		std::this_thread::sleep_for(std::chrono::milliseconds(2000 - dt));
	}

	bool b = this->connectThread();
	connectTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

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
	if (socketWriter) {
		socketWriter->stop();
	}
    if(socketReader) {
        socketReader->stop();
    }

	clientStatus.clear();
	this->setStatus(EzySocketStatusType::Closed);
	closeSocket();
}

EzySocketStatusType EzySocketClient::getStatus(){
	return clientStatus.get();
}

void EzySocketClient::setStatus(EzySocketStatusType status, bool isEvent){
	clientStatus.set(status, isEvent);
}

void EzySocketClient::sendMessage(EzySocketData* data){
	if (socketWriter){
		socketWriter->pushMessage(data);
	}
}

void EzySocketClient::processEvent(){
	if (statusCallback){
		clientStatus.popAllStatus(statusBuffer);
		for (int i = 0; i < statusBuffer.size(); i++){
			statusCallback(statusBuffer[i]);
		}
		statusBuffer.clear();
	}
}

void EzySocketClient::processRecvMessage(){
	if (recvCallback){
		auto data = socketReader->popMessage();
		while (data){
			if (this->getStatus() != EzySocketStatusType::Connected){
				break;
			}
			recvCallback(data);
			data = socketReader->popMessage();
		}
	}
}

void EzySocketClient::clearAdapter(){
	std::unique_lock<std::mutex> lk(clientMutex);
	if (socketReader){
		socketReader->stop();
		socketReader->release();
		socketReader = 0;
	}
	if (socketWriter){
		socketWriter->stop();
		socketWriter->release();
		socketWriter = 0;
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
		if (socketReader && socketWriter){
			processRecvMessage();
			if (!socketReader->isRunning()){
				if (this->getStatus() == EzySocketStatusType::Connected){
					this->processSocketError();
					this->closeSocket();
					this->clearAdapter();
				}
			}
		}
	}

	processEvent();

	if (!releasePool){
        releasePool = pool::EzyAutoReleasePool::getInstance()->getPool();
	}
	releasePool->releaseAll();
}

}
EZY_NAMESPACE_END
