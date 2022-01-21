#include <appdef.hpp>
#include <sdk/cpu/ubc.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/string.hpp>
#include <sdk/os/file.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/serial.hpp>

#include <stdio.h>
#include <stdint.h>

#include "utils.h"

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
#ifndef PC
APP_NAME("TarrasK")
APP_DESCRIPTION("RE Toolkit")
APP_AUTHOR("Phoebe C.")
APP_VERSION("1.0.0")
#endif

struct CPUState {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
    uint32_t r15;

    uint32_t gbr;
    uint32_t pr;

    uint32_t mach;
    uint32_t macl;
};

// Defined in breakpoint_handler_stub.s
extern "C" void BreakpointHandlerStub();

/**
 * Called by @c BreakpointHandlerStub when a breakpoint is triggered.
 *
 * @param cpuState The state of the CPU when the breakpoint was triggered.
 */
extern "C"
void BreakpointHandler(struct CPUState *cpuState) {

    int carret_x = 0;

    Debug_SetCursorPosition(0, 0);
    //Example for Debug_Printf(x,y,invert_color,0,format_string) //(small text)
    Debug_Printf(0,carret_x,false,0,":: TRK :: Breakpoint                               ::"); //(small text)

    Debug_Printf(0,++carret_x,true,0," r0  %08X r1  %08X r2  %08X r3  %08X ", cpuState->r0, cpuState->r1, cpuState->r2, cpuState->r3); //(small text)
    Debug_Printf(0,++carret_x,true,0," r4  %08X r5  %08X r6  %08X r7  %08X ", cpuState->r4, cpuState->r5, cpuState->r6, cpuState->r7); //(small text)
    Debug_Printf(0,++carret_x,true,0," r8  %08X r9  %08X r10 %08X r11 %08X ", cpuState->r8, cpuState->r9, cpuState->r10, cpuState->r11); //(small text)
    Debug_Printf(0,++carret_x,true,0," r12 %08X r13 %08X r14 %08X r15 %08X ", cpuState->r12, cpuState->r13, cpuState->r14, cpuState->r15); //(small text)
    Debug_Printf(0,++carret_x,true,0," gbr %08X pr  %08X ach %08X acl %08X ", cpuState->gbr, cpuState->pr, cpuState->mach, cpuState->macl); //(small text)
    Debug_Printf(0,++carret_x,false,0," r| Hex Dump                          | Ascii        ");

    uint32_t registerAddr = cpuState->r0;
    if (isAddressWritable(registerAddr)) {
        // Try to dump its content, EG for strings
        carret_x = printBytes(registerAddr, 0, ++carret_x);
    }
    registerAddr = cpuState->r1;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 1,++carret_x);
    }
    registerAddr = cpuState->r2;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 2 ,++carret_x);
    }
    registerAddr = cpuState->r3;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 3 ,++carret_x);
    }
    registerAddr = cpuState->r4;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 4 ,++carret_x);
    }
    registerAddr = cpuState->r5;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 5 ,++carret_x);
    }
    registerAddr = cpuState->r6;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 6 ,++carret_x);
    }
    registerAddr = cpuState->r7;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 7 ,++carret_x);
    }
    registerAddr = cpuState->r8;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 8 ,++carret_x);
    }
    registerAddr = cpuState->r9;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 9 ,++carret_x);
    }
    registerAddr = cpuState->r10;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 10,++carret_x);
    }
    registerAddr = cpuState->r11;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 11,++carret_x);
    }
    registerAddr = cpuState->r12;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 12,++carret_x);
    }
    registerAddr = cpuState->r14;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 14,++carret_x);
    }
    registerAddr = cpuState->r15;
    if (isAddressWritable(registerAddr)) {
        carret_x = printBytes(registerAddr, 15,++carret_x);
    }

    LCD_Refresh();

    // Add virtual delay so that we can read the text
//    int8_t i = 80;
//    while (i > 0) {
//        // Eat CPU cycles + Kool animation ! :D
//        switch (i%4) {
//            case 0:
//                Debug_Printf(0,0,false,0,"::");
//                break;
//            case 1:
//                Debug_Printf(0,0,false,0,".:");
//                break;
//            case 2:
//                Debug_Printf(0,0,false,0,"..");
//                break;
//            case 3:
//                Debug_Printf(0,0,false,0,":.");
//                break;
//        }
////        LCD_Refresh();
//        asm volatile ("nop"::);
//    }

    while(true){
        uint32_t key1, key2;	//First create variables
        key1 = 0;
        key2 = 0;
        getKey(&key1, &key2);	//then read the keys
        if(testKey(key1, key2, KEY_CLEAR)){ //Use testKey() to tests if a specific key is pressed
             break;
        }
    }
}


/**
 * Sets a breakpoint at @p address, triggered after an instruction at the
 * address is executed.
 *
 * @param address The address to set the breakpoint at.
 */
void SetBreakpoint(uint32_t address) {
    // Set our breakpoint handler stub as the DBR (debug base register)
    #ifndef PC
    __asm__ volatile(
    "ldc %0, dbr\n"
    :
    : "r"(&BreakpointHandlerStub)
    :
    );

    // Tell the UBC to use the function pointed to by DBR
    UBC_REG_CBCR = 1 << UBC_CBCR_UBDE;

    // Set the breakpoint to occur on an instruction fetch cycle
    UBC_REG_CBR0 = 1 << UBC_CBR_ID;

    // Set the address of the function we want to debug
    UBC_REG_CAR0 = address;

    // Set address mask to 0 (only trigger on the exact address)
    UBC_REG_CAMR0 = 0;

    // Enable the breakpoint and set the breakpoint to trigger after instruction
    // execution
    UBC_REG_CRR0 = (1 << UBC_CRR_RESERVED) | (1 << UBC_CRR_PCB) | (1 << UBC_CRR_BIE);

    // Enable the channel
    UBC_REG_CBR0 |= 1 << UBC_CBR_CE;

    #endif
}

/**
 * Disables any breakpoint which was set.
 */
void RemoveBreakpoint() {
    // Disable the channel
    UBC_REG_CBR0 = 0 << UBC_CBR_CE;
}

class BreakpointDialog : public GUIDialog {
public:
    BreakpointDialog() : GUIDialog(
            Height55, AlignCenter,
            "Breakpoint Utility",
            KeyboardStateABC
    ), m_breakpointAddressLabel(
            GetLeftX() + 10, GetTopY() + 10,
            "Breakpoint address (hex)"
    ), m_breakpointAddress(
            GetLeftX() + 10, GetTopY() + 30, GetRightX() - GetLeftX() - 20,
            8, true
    ), m_setBreakpoint(
            GetLeftX() + 10, GetTopY() + 60, GetRightX() - 10, GetTopY() + 90,
            "Set breakpoint", BUTTON_SET_BREAKPOINT_EVENT_ID
    ), m_removeBreakpoint(
            GetLeftX() + 10, GetTopY() + 95, GetRightX() - 10, GetTopY() + 125,
            "Remove breakpoint", BUTTON_REMOVE_BREAKPOINT_EVENT_ID
    ), m_close(
            GetLeftX() + 10, GetTopY() + 130, GetRightX() - 10, GetTopY() + 160,
            "Close", BUTTON_CLOSE_EVENT_ID
    ) {
        AddElement(m_breakpointAddressLabel);
        AddElement(m_breakpointAddress);
        AddElement(m_setBreakpoint);
        AddElement(m_removeBreakpoint);
        AddElement(m_close);
    }

    /**
     * Returns the integer representation of the address the user entered.
     *
     * Accepts both uppercase and lowercase hexidecimal.
     *
     * @return The integer representation of the address the user entered, or 0
     * if the address was invalid in some way.
     */
    uint32_t GetBreakpointAddress() {
        const char *text = m_breakpointAddress.GetText();

        if (text == 0) {
            return 0;
        }

        char c;
        uint32_t address = 0;
        while ((c = *text++) != '\0') {
            uint32_t v;

            // Since 0-9, a-f and A-F are consecutive in the ASCII space, this
            // is an easy way to cheat the conversion from hex string -> number
            if (c >= '0' && c <= '9') {
                v = c - '0';
            } else if (c >= 'a' && c <= 'f') {
                v = c - 'a' + 0xA;
            } else if (c >= 'A' && c <= 'F') {
                v = c - 'A' + 0xA;
            } else {
                return 0;
            }

            address *= 0x10;
            address += v;
        }

        // Check if the address is within an actual region of memory
        // i.e 0x8000000-0x81500000 or 0x8C000000-0x8D000000
        if (
                !(address >= 0x80000000 && address <= 0x81500000) &&
                !(address >= 0x8C000000 && address <= 0x8D000000)
                ) {
            return 0;
        }

        return address;
    }

    int OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
        // Annoyingly, setting the text of the address textbox is the best
        // way I have at the moment of providing some feedback to the user.
        // Not elegant, but it gets the job done.
        // TODO: Add a label to indicate the status -> requires finding a
        // SetText function for labels.
        if (event->GetEventID() == BUTTON_SET_BREAKPOINT_EVENT_ID) {
            uint32_t address = GetBreakpointAddress();

            if (address != 0) {
                SetBreakpoint(address);

                m_breakpointAddress.SetText("Set!");
            } else {
                m_breakpointAddress.SetText("Invalid.");
            }

            Refresh();
            return 0;
        }

        if (event->GetEventID() == BUTTON_REMOVE_BREAKPOINT_EVENT_ID) {
            RemoveBreakpoint();

            m_breakpointAddress.SetText("Removed.");
            Refresh();
            return 0;
        }

        return GUIDialog::OnEvent(dialog, event);
    }

private:
    GUILabel m_breakpointAddressLabel;
    GUITextBox m_breakpointAddress;

    static const int BUTTON_SET_BREAKPOINT_EVENT_ID = 1;
    GUIButton m_setBreakpoint;

    static const int BUTTON_REMOVE_BREAKPOINT_EVENT_ID = 2;
    GUIButton m_removeBreakpoint;

    static const int BUTTON_CLOSE_EVENT_ID = GUIDialog::DialogResultCancel;
    GUIButton m_close;
};

extern "C"
#ifdef PC
int  main(){
#else
void main(){
#endif
    BreakpointDialog dialog;
    dialog.ShowDialog();

#ifdef PC
    return 0;
#endif
}

//
//extern "C"
//void main() {
//	calcInit(); //backup screen and init some variables
//
//	fillScreen(color(0,0,0));
//
//	float testVersion = 1.0;
//	//Example for Debug_Printf(x,y,invert_color,0,format_string) //(small text)
//
//	Debug_Printf(0,0,true,0,"TarrasK %.2f",testVersion);
//
////	Debug_Printf(10,4,true,0,"ceil %f",ceil(3.6));
//
//
//	LCD_Refresh();
//
//	//Example for getKey
//	while(true){
//		uint32_t key1, key2;	//First create variables
//		getKey(&key1, &key2);	//then read the keys
//		if(testKey(key1, key2, KEY_CLEAR)){ //Use testKey() to tests if a specific key is pressed
//			break;
//		}
//	}
//
//	calcEnd(); //restore screen and do stuff
//}
