#include "Mutex_Z.h"
#include "Memory_Z.h"
#include "StreamManager_Z.h"

#define STREAM_THREAD_STACK_SIZE 0x1000
#define STREAM_THREAD_STACK_OFFSET 0x400
#define STREAM_THREAD_PRIORITY 16

static void* ThreadStack;

class SharedResource_Z::Primitive {
public:
};

SharedResource_Z::SharedResource_Z()
    : m_Primitive(NewL_Z(12) Primitive) {
}

SharedResource_Z::~SharedResource_Z() {
    Delete_Z m_Primitive;
}

void SharedResource_Z::Lock() {
}

void SharedResource_Z::Unlock() {
}

SharedResourceGuard_Z::SharedResourceGuard_Z(SharedResource_Z& i_SharedResource)
    : m_SharedResource(i_SharedResource) {
    i_SharedResource.Lock();
}

SharedResourceGuard_Z::~SharedResourceGuard_Z() {
    m_SharedResource.Unlock();
}

Bool Event_Z::Create() {
    return TRUE;
}

Bool Event_Z::Wait(S32 i_Time) {
    return TRUE;
}

Bool StreamManager_Z::Init() {
    ThreadStack = AllocL_Z(STREAM_THREAD_STACK_SIZE, 111);
    void* l_Stack = (void*)((Char*)ThreadStack + STREAM_THREAD_STACK_SIZE);
    return TRUE;
}

StreamManager_Z::~StreamManager_Z() {
    Free_Z(ThreadStack);
    ThreadStack = NULL;
}

// TODO: Since Thread isn't blocking we're doing it all in the main thread now.
//       Investigate if it's worth it or needed to actually run the streaming in a separate thread.
void StreamManager_Z::Update(Float i_DeltaTime) {
    Heartbeat(i_DeltaTime);
    Thread();
}

void* StreamManager_Z::ThreadProcVoid(void* i_Param) {
    StreamManager_Z* l_StreamManager = (StreamManager_Z*)i_Param;
    while (!l_StreamManager->m_Finished) {
        l_StreamManager->Thread();
    }
    return NULL;
}
