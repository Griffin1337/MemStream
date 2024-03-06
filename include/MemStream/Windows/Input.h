//
// Created by Kegan Hollern on 12/23/23.
//

#ifndef MEMSTREAM_INPUT_H
#define MEMSTREAM_INPUT_H

#if defined(_WIN32)
#if defined(MEMSTREAM_EXPORTS)
#define MEMSTREAM_API __declspec(dllexport)
#else
#define MEMSTREAM_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define MEMSTREAM_API __attribute__ ((visibility ("default")))
#else
#define MEMSTREAM_API
#endif
#endif

#include <cstdint>

#include <vmmdll.h>

#include <MemStream/FPGA.h>
#include <MemStream/Process.h>

namespace memstream::windows {

    typedef struct MousePoint {
        long x;
        long y;
    } MousePoint;

    class MEMSTREAM_API Input {
    public:
        explicit Input(FPGA *pFPGA);

        Input();
        virtual ~Input();

        bool Update();

        bool IsKeyDown(uint32_t vk); // if key is down _this_ frame
        bool WasKeyDown(uint32_t vk); // if key was down _last_ frame

        // set a callback to be run on key state change
        //  Callback(VIRTUAL_KEY_CODE, IS_DOWN)
        void OnKeyStateChange(void(*callback)(int, bool)); 
        // set a callback to be run on mouse pos change
        // Callback(CHANGE_IN_MOUSE_POSITION, NEW_MOUSE_POSITION)
        void OnMouseMove(void(*callback)(MousePoint,MousePoint));
         
        Process* GetKernelProcess();

        MousePoint GetCursorPos();
    private:
        Process *kernel;

        // kernel virtual address of async keystate value
        uint64_t gafAsyncKeyStateAddr;
        uint64_t gptCursorAsync;

        MousePoint prevPos{0};
        MousePoint cursorPos{0};
        uint8_t state[64]{0};
        uint8_t prevState[64]{0};
        void(*key_callback)(int, bool) = nullptr;
        void(*mouse_callback)(MousePoint,MousePoint) = nullptr;

        bool first_update = true;
    };



    Process* getUserSessionKernelProcess(FPGA* pFPGA);
}

#endif //MEMSTREAM_INPUT_H
