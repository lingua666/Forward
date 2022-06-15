/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*
 * A wrapper for resampling a numerous amount of sampling combinations.
 */

#ifndef __AUDIO_DEVICE_RESAMPLER_CONVERT_2020010411380210_H_
#define __AUDIO_DEVICE_RESAMPLER_CONVERT_2020010411380210_H_

#include <libThirdWrap_Audio/webrtc/resampler.h>


int16_t *ResamplerConvert(int16_t *data_in, size_t totalSampleCount, size_t in_sample_rate,
	size_t out_sample_rate, int16_t out_channel, size_t& outLen);


#endif // WEBRTC_RESAMPLER_RESAMPLER_H_
