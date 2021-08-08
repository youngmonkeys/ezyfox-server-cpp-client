LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ezyfox_client_static

LOCAL_MODULE_FILENAME := libezyfox_client_static

LOCAL_SRC_FILES := \
    $(LOCAL_PATH)/../../../src/handler/EzyEventHandlers.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyDataHandlers.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyEventHandler.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyAppDataHandlers.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyPluginDataHandlers.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyPluginDataHandler.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyAppDataHandler.cpp \
    $(LOCAL_PATH)/../../../src/handler/EzyDataHandler.cpp \
    $(LOCAL_PATH)/../../../src/EzyUTClient.cpp \
    $(LOCAL_PATH)/../../../src/logger/EzyLogger.cpp \
    $(LOCAL_PATH)/../../../src/config/EzyClientConfig.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyApp.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyString.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyZone.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyPrimitive.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyObject.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyNull.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyByteArray.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyArray.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyValue.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyUser.cpp \
    $(LOCAL_PATH)/../../../src/entity/EzyPlugin.cpp \
    $(LOCAL_PATH)/../../../src/codec/EzyMessage.cpp \
    $(LOCAL_PATH)/../../../src/codec/EzyDataDecoder.cpp \
    $(LOCAL_PATH)/../../../src/codec/EzyEncryption.cpp \
    $(LOCAL_PATH)/../../../src/codec/EzyDataEncoder.cpp \
    $(LOCAL_PATH)/../../../src/EzyClient.cpp \
    $(LOCAL_PATH)/../../../src/setup/EzySetup.cpp \
    $(LOCAL_PATH)/../../../src/EzyClients.cpp \
    $(LOCAL_PATH)/../../../src/manager/EzyPluginManager.cpp \
    $(LOCAL_PATH)/../../../src/manager/EzyAppManager.cpp \
    $(LOCAL_PATH)/../../../src/manager/EzyHandlerManager.cpp \
    $(LOCAL_PATH)/../../../src/manager/EzyPingManager.cpp \
    $(LOCAL_PATH)/../../../src/constant/EzyConnectionStatus.cpp \
    $(LOCAL_PATH)/../../../src/constant/EzyCommand.cpp \
    $(LOCAL_PATH)/../../../src/constant/EzyDisconnectReason.cpp \
    $(LOCAL_PATH)/../../../src/constant/EzyConnectionFailedReason.cpp \
    $(LOCAL_PATH)/../../../src/concurrent/EzyThread.cpp \
    $(LOCAL_PATH)/../../../src/concurrent/EzyScheduleAtFixedRate.cpp \
    $(LOCAL_PATH)/../../../src/gc/EzyReleasePool.cpp \
    $(LOCAL_PATH)/../../../src/gc/EzyAutoReleasePool.cpp \
    $(LOCAL_PATH)/../../../src/request/EzyRequest.cpp \
    $(LOCAL_PATH)/../../../src/request/EzyRequestSerializer.cpp \
    $(LOCAL_PATH)/../../../src/event/EzyEvent.cpp \
    $(LOCAL_PATH)/../../../src/event/EzyEventType.cpp \
    $(LOCAL_PATH)/../../../src/base/EzyRef.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzyTcpSocketClient.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketClient.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketAdapter.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketCore.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzyMainEventsLoop.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzyPingSchedule.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketPool.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketReader.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzyUTSocketClient.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzyUdpSocketClient.cpp \
    $(LOCAL_PATH)/../../../src/socket/EzySocketWriter.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../../src \
    $(LOCAL_PATH)/../../../src/handler \
    $(LOCAL_PATH)/../../../src/logger \
    $(LOCAL_PATH)/../../../src/util \
    $(LOCAL_PATH)/../../../src/config \
    $(LOCAL_PATH)/../../../src/entity \
    $(LOCAL_PATH)/../../../src/codec \
    $(LOCAL_PATH)/../../../src/setup \
    $(LOCAL_PATH)/../../../src/manager \
    $(LOCAL_PATH)/../../../src/constant \
    $(LOCAL_PATH)/../../../src/concurrent \
    $(LOCAL_PATH)/../../../src/gc \
    $(LOCAL_PATH)/../../../src/request \
    $(LOCAL_PATH)/../../../src/event \
    $(LOCAL_PATH)/../../../src/base \
    $(LOCAL_PATH)/../../../src/socket

LOCAL_EXPORT_C_INCLUDES := \
    $(LOCAL_PATH)/../../../src

LOCAL_EXPORT_LDLIBS := -lz -latomic -llog

LOCAL_CFLAGS := -std=c++11

include $(BUILD_STATIC_LIBRARY)