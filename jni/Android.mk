TOP_PATH := $(call my-dir)
CJSON_PATH :=?


# Build executable Ronin
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := ronin
LOCAL_PATH := $(TOP_PATH)/ronin
LOCAL_SRC_FILES := ronin.c mapsharedlib.c stringutils.c memutils.c hash.c sharedlib.c elfutils.c thread.c symbol.c hook.c app.c jvm.c storage.c config.c CJSON_PATH/cJSON.c sll.c
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)



# Build executable Test
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := test
LOCAL_PATH := $(TOP_PATH)/tests
LOCAL_SRC_FILES := main.c
LOCAL_LDLIBS := -llog 
include $(BUILD_EXECUTABLE)


# Build Shared Library
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := shared
LOCAL_PATH := $(TOP_PATH)/shared
LOCAL_SRC_FILES := shared.c
include $(BUILD_SHARED_LIBRARY)

# Build Shared Library
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := network
LOCAL_PATH := $(TOP_PATH)/network
LOCAL_SRC_FILES := main.c
LOCAL_LDLIBS := -llog 
include $(BUILD_SHARED_LIBRARY)
