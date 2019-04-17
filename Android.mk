#----------------------------------------------------------------------------
# INTEL CONFIDENTIAL
# Copyright (2002-2014) Intel Corporation All Rights Reserved.
# The source code contained or described herein and all documents related to
# the source code ("Material") are owned by Intel Corporation or its suppliers
# or licensors. Title to the Material remains with Intel Corporation or its
# suppliers and licensors. The Material contains trade secrets and proprietary
# and confidential information of Intel or its suppliers and licensors. The
# Material is protected by worldwide copyright and trade secret laws and
# treaty provisions. No part of the Material may be used, copied, reproduced,
# modified, published, uploaded, posted, transmitted, distributed, or
# disclosed in any way without Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or
# delivery of the Materials, either expressly, by implication, inducement,
# estoppel or otherwise. Any license under such intellectual property rights
# must be express and approved by Intel in writing.
#
#----------------------------------------------------------------------------
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CPPFLAGS += \
        -DANDROID \
        -DANDROID_VERSION=800 \
	-DTESTREG_LOG_TAG="\"REG_READ_WRITE\"" \

#WA
LOCAL_CPPFLAGS += \
    -Wno-unused-parameter \
    -Wno-error

LOCAL_SRC_FILES := \
     reg_read_write.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    liblog \
    libpciaccess \
    libdrm_pri \
    libdrm_intel_pri

LOCAL_EXPORT_C_INCLUDE_DIRS = \
    $(LOCAL_PATH)/

LOCAL_MODULE := reg_read_write
LOCAL_PROPRIETARY_MODULE := true

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)
