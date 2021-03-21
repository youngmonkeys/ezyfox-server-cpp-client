APP_STL := c++_static
APP_CPPFLAGS := -frtti -std=c++11 -fsigned-char
APP_MODULES := ezyfox_client_static
APP_ABI := armeabi-v7a
APP_SHORT_COMMANDS := true

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DEZY_DEBUG=1
  APP_OPTIM := debug
else
  APP_OPTIM := release
endif