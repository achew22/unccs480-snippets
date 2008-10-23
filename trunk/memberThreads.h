#ifndef MEMBERTHREADS_H_INCLUDED
#define MEMBERTHREADS_H_INCLUDED

/********************************************************************
   Helper function to create a SDL_thread of a member function


   STOLEN STOLEN STOLEN STOLEN STOLEN STOLEN STOLEN STOLEN STOLEN
   STOLEN BY ANDREW ALLEN WHO FULLY OWNS UP THAT IT WAS STOLEN
   SHAMELESSLY

   Taken from:
   http://www.gamedev.net/community/forums/topic.asp?topic_id=438585
********************************************************************/

template <class ThisType, class FuncType>
struct MethodInfo {
	static int Stub(void *_Info) {

		//Dynamic thread creates pointer to the functype
		//which means it should be destroyed after function end
		//however if thread is killed it is not called and memory leak is found
		#ifdef _DYNAMIC_THREAD_

			MethodInfo <ThisType, FuncType> *Info;

			Info = (MethodInfo <ThisType, FuncType> *)(_Info);

			((Info->This)->*(Info->Func))();

			delete Info;

		//the static thread fixes this problem by using variable generated on the
		//stack which means it is created and destroyed automaticly
		//so no memory leak is found
		#else //_STATIC_THREAD_

			MethodInfo <ThisType, FuncType> Info;

			Info		= *(MethodInfo <ThisType, FuncType> *)(_Info);

			delete (MethodInfo <ThisType, FuncType> *)(_Info);

			((Info.This)->*(Info.Func))();

		#endif

		//delete Info;
		return 0;
	}



	ThisType *This;
	FuncType Func;

	MethodInfo(ThisType *_This, FuncType _F) : This(_This), Func(_F) {}
	MethodInfo(){};
};



template <class ThisType, class FuncType>
SDL_Thread* SDL_CreateMemberThread(ThisType *This, FuncType Func) {

	MethodInfo <ThisType,FuncType> *Info;

	Info = new MethodInfo <ThisType,FuncType> (This,Func);

	return SDL_CreateThread(( int(*)(void *) )(MethodInfo<ThisType,FuncType>::Stub),Info);


}

#endif // MEMBERTHREADS_H_INCLUDED
