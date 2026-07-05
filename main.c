#include <stdio.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>

// Global variables for tracking the menu state
int current_selection = 0; // 0 = PLAY, 1 = LOAD, 2 = OPTIONS
#define MENU_MAX 3

// Alignment buffers required by the PS2 hardware for controller data
static char padBuf[256] __attribute__((aligned(64)));

void init_controllers() {
    // 1. Reset SifRPC (System Interface) and load the controller driver modules
    sbv_del_init();
    SifInitRpc(0);
    
    // Load the Input/Output Processor pad drivers
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);
    
    // 2. Initialize the pad library and open Port 0, Slot 0
    padInit(0);
    padPortOpen(0, 0, padBuf);
}

void check_inputs() {
    struct padButtonStatus buttons;
    u32 paddata;
    u32 old_disabled_buttons;

    // Read the current state of Port 0, Slot 0
    if (padGetState(0, 0) == PAD_STATE_STABLE) {
        int ret = padRead(0, 0, &buttons);
        if (ret != 0) {
            // Convert button struct to bitmask data
            paddata = 0xffff ^ buttons.btns;
            
            // Check if D-PAD DOWN is pressed
            if (paddata & PAD_DOWN) {
                current_selection++;
                if (current_selection >= MENU_MAX) current_selection = 0;
                printf("Moved Menu Down. Current Option: %d\n", current_selection);
                
                // Small delay to prevent ultra-fast scrolling
                int i; for(i=0; i<2000000; i++) { __asm__("nop"); } 
            }
            
            // Check if D-PAD UP is pressed
            if (paddata & PAD_UP) {
                current_selection--;
                if (current_selection < 0) current_selection = MENU_MAX - 1;
                printf("Moved Menu Up. Current Option: %d\n", current_selection);
                
                int i; for(i=0; i<2000000; i++) { __asm__("nop"); }
            }
            
            // Check if X (Cross) button is pressed to select
            if (paddata & PAD_CROSS) {
                printf("Selected option %d!\n", current_selection);
                
                int i; for(i=0; i<5000000; i++) { __asm__("nop"); }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    printf("Initializing GTA-Style Menu System...\n");
    init_controllers();
    
    printf("Controller ready! Use Up/Down D-Pad. Press X to select.\n");
    
    // Main Game/Menu Loop
    while(1) {
        check_inputs();
    }
    
    return 0;
}
