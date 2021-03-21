
#/****************************************************************************
# Copyright (c) 2021 youngmonkeys.org
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

project(ezyfox-client)

set(EZY_CLIENT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(EZY_CLIENT_HEADER

    src/handler/EzyPluginDataHandlers.h
    src/handler/EzyEventHandler.h
    src/handler/EzyPluginDataHandler.h
    src/handler/EzyDataHandlers.h
    src/handler/EzyEventHandlers.h
    src/handler/EzyAppDataHandler.h
    src/handler/EzyDataHandler.h
    src/handler/EzyAppDataHandlers.h
    src/EzyClient.h
    src/EzyClients.h
    src/EzyUTClient.h
    src/logger/EzyLogger.h
    src/util/EzyStack.h
    src/util/EzyMaps.h
    src/config/EzyClientConfig.h
    src/entity/EzyUser.h
    src/entity/EzyPlugin.h
    src/entity/EzyPrimitive.h
    src/entity/EzyArray.h
    src/entity/EzyValue.h
    src/entity/EzyZone.h
    src/entity/EzyApp.h
    src/entity/EzyObject.h
    src/entity/EzyString.h
    src/codec/EzyDataEncoder.h
    src/codec/EzyMessage.h
    src/codec/EzyDataDecoder.h
    src/setup/EzySetup.h
    src/EzyHeaders.h
    src/manager/EzyHandlerManager.h
    src/manager/EzyAppManager.h
    src/manager/EzyPingManager.h
    src/manager/EzyPluginManager.h
    src/EzyMacro.h
    src/constant/EzyStatusCode.h
    src/constant/EzyConnectionStatus.h
    src/constant/EzyTransportType.h
    src/constant/EzyConnectionFailedReason.h
    src/constant/EzyDisconnectReason.h
    src/constant/EzyCommand.h
    src/concurrent/EzyThread.h
    src/concurrent/EzyScheduleAtFixedRate.h
    src/gc/EzyReleasePool.h
    src/gc/EzyAutoReleasePool.h
    src/request/EzyRequest.h
    src/request/EzyRequestSerializer.h
    src/event/EzyEventType.h
    src/event/EzyEvent.h
    src/base/EzyRef.h
    src/socket/EzySocketDelegate.h
    src/socket/EzySocketAdapter.h
    src/socket/EzySocketClient.h
    src/socket/EzySocketReader.h
    src/socket/EzySocketDefine.h
    src/socket/EzyTcpSocketClient.h
    src/socket/EzySocketPool.h
    src/socket/EzyUdpSocketClient.h
    src/socket/EzyUTSocketClient.h
    src/socket/EzySender.h
    src/socket/EzySocketCore.h
    src/socket/EzySocketWriter.h
    src/socket/EzyPingSchedule.h
    src/socket/EzyMainEventsLoop.h

    )

set(EZY_CLIENT_SRC

    src/handler/EzyEventHandlers.cpp
    src/handler/EzyDataHandlers.cpp
    src/handler/EzyEventHandler.cpp
    src/handler/EzyAppDataHandlers.cpp
    src/handler/EzyPluginDataHandlers.cpp
    src/handler/EzyPluginDataHandler.cpp
    src/handler/EzyAppDataHandler.cpp
    src/handler/EzyDataHandler.cpp
    src/EzyUTClient.cpp
    src/logger/EzyLogger.cpp
    src/config/EzyClientConfig.cpp
    src/entity/EzyApp.cpp
    src/entity/EzyString.cpp
    src/entity/EzyZone.cpp
    src/entity/EzyPrimitive.cpp
    src/entity/EzyObject.cpp
    src/entity/EzyArray.cpp
    src/entity/EzyValue.cpp
    src/entity/EzyUser.cpp
    src/entity/EzyPlugin.cpp
    src/codec/EzyMessage.cpp
    src/codec/EzyDataDecoder.cpp
    src/codec/EzyDataEncoder.cpp
    src/EzyClient.cpp
    src/setup/EzySetup.cpp
    src/EzyClients.cpp
    src/manager/EzyPluginManager.cpp
    src/manager/EzyAppManager.cpp
    src/manager/EzyHandlerManager.cpp
    src/manager/EzyPingManager.cpp
    src/constant/EzyConnectionStatus.cpp
    src/constant/EzyCommand.cpp
    src/constant/EzyDisconnectReason.cpp
    src/constant/EzyConnectionFailedReason.cpp
    src/concurrent/EzyThread.cpp
    src/concurrent/EzyScheduleAtFixedRate.cpp
    src/gc/EzyReleasePool.cpp
    src/gc/EzyAutoReleasePool.cpp
    src/request/EzyRequest.cpp
    src/request/EzyRequestSerializer.cpp
    src/event/EzyEvent.cpp
    src/event/EzyEventType.cpp
    src/base/EzyRef.cpp
    src/socket/EzyTcpSocketClient.cpp
    src/socket/EzySocketClient.cpp
    src/socket/EzySocketAdapter.cpp
    src/socket/EzySocketCore.cpp
    src/socket/EzyMainEventsLoop.cpp
    src/socket/EzyPingSchedule.cpp
    src/socket/EzySocketPool.cpp
    src/socket/EzySocketReader.cpp
    src/socket/EzyUTSocketClient.cpp
    src/socket/EzyUdpSocketClient.cpp
    src/socket/EzySocketWriter.cpp

    )

list(APPEND EZY_CLIENT_SRC ${EZY_CLIENT_HEADER})

add_library(ezyfox-client ${EZY_CLIENT_SRC})

target_include_directories( ezyfox-client
    PUBLIC src
    PUBLIC src/handler
    PUBLIC src/logger
    PUBLIC src/util
    PUBLIC src/config
    PUBLIC src/entity
    PUBLIC src/codec
    PUBLIC src/setup
    PUBLIC src/manager
    PUBLIC src/constant
    PUBLIC src/concurrent
    PUBLIC src/gc
    PUBLIC src/request
    PUBLIC src/event
    PUBLIC src/base
    PUBLIC src/socket
)
