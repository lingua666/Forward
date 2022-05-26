
#ifndef __DECODERFACTORY_20170111090004_1444199361_H__
#define __DECODERFACTORY_20170111090004_1444199361_H__

#include "IDecoder.h"
#include "External.h"
#include <vector>

namespace _ffmpeg_{

	class DecoderFactory
	{
	public:
		typedef IDecoder* (*CreateCallback)( void );
		typedef void (*DestroyCallback)( IDecoder* );

		static void FreeDecoder(IDecoder* pDecoder);

	public:
		DecoderFactory( void );

		IDecoder* CreateRender ( enDecoderType Type );

	private:
		std::vector<CreateCallback>		_CreateVec;
	};

	DecoderFactory* DecoderFactory_GetInstance( void );

} //namespace _ffmpeg_

#endif//__DECODERFACTORY_20170111090004_1444199361_H__
