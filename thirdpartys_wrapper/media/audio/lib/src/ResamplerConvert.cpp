
#include <libThirdWrap_Audio/webrtc/resampler.h>


int16_t *ResamplerConvert(int16_t *data_in, size_t totalSampleCount, size_t in_sample_rate, 
	size_t out_sample_rate, int16_t out_channel, size_t& outLen)
{
	if (data_in == nullptr)
		return nullptr;

	if (in_sample_rate == 0) return nullptr;
	size_t lengthIn = in_sample_rate / 100;
	size_t maxLen = out_sample_rate / 100;
	int channels = out_channel;
	if (out_sample_rate == 8000)
		channels = 1;

	webrtc::Resampler rs;
	if (rs.Reset(in_sample_rate, out_sample_rate, channels) == -1)
		return nullptr;

	/*size_t*/ outLen = (size_t)(totalSampleCount * out_sample_rate / in_sample_rate) * 2;
	int16_t *data_out = (int16_t *)malloc(outLen * sizeof(int16_t));
	if (data_out == nullptr) return nullptr;
	size_t nCount = (totalSampleCount / lengthIn);
	size_t nLast = totalSampleCount - (lengthIn * nCount);
	int16_t *samplesIn = data_in;
	int16_t *samplesOut = data_out;

	size_t  Length = 0;

	for (int i = 0; i < nCount; i++) 
	{
		rs.Push(samplesIn, lengthIn, samplesOut, maxLen, Length);
		samplesIn += lengthIn;
		samplesOut += Length;
	}

	if (nLast != 0) 
	{
		const int max_samples = 1920;
		int16_t samplePatchIn[max_samples] = { 0 };
		int16_t samplePatchOut[max_samples] = { 0 };
		memcpy(samplePatchIn, samplesIn, nLast * sizeof(int16_t));
		rs.Push(samplesIn, nLast, samplePatchOut, maxLen, outLen);
		memcpy(samplesOut, samplePatchOut, (nLast * out_sample_rate / in_sample_rate) * sizeof(int16_t));
	}
	return data_out;
}
