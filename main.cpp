#include "cpu.hpp"
using namespace std;
int main()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL2 was Unable to Initialize!");
        return 1;
    }
    nopNESwindow = SDL_CreateWindow("nopNES Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(nopNESwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetScale(renderer,2,2);
    if(beginning() == false)
    {
        printf("File Not Found!\n");
        return 0;
    }
    currentFrame = 0;
    nesPowerOn();
    NESOB.scanline = 0;
    pal = fopen("pal.pal", "rb");
    fread(pallete,192,1,pal);
    fclose(pal);
    thread graphic (handleGraphicsBASIC);
    NESOB.opcode = NESOB.memory[NESOB.pc];
    while(NESOB.dontSetTrue == false) // Begin Loop
    {
        while( SDL_PollEvent( &SDL_EVENT_HANDLING)) // While Event to handle Random Stuff
        {
            if (SDL_EVENT_HANDLING.type == SDL_QUIT) // If the SDL Window is Closed, close the program.
            {
                NESOB.closeProgram = true;
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYDOWN) // If a key is being pressed, handle controls.
            {   // Handle Controls
                handleControls();
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYUP)
            {
                handleControlsr();
            }
        }
        NESOB.opcode = NESOB.memory[NESOB.pc];
        prevScanlineTimer = NESOB.cycles % cycleModulo;
        //printf("Opcode: 0x%X\n",NESOB.opcode);
        //cout<<newOpcode2<<endl;
        //printf("PC: 0x%X\n",NESOB.pc);
        doOpcode();
        handleOther();
        handleInterrupts();
        handleScanlineStuff();
        //printRegs();
        //handleLog();
        //printRegs();
        if(NESOB.pc == 0xF50E)
        {
            //printf("Scanline: 0x%i\n",NESOB.scanline);
            //breakpoint = true;
        }
        if(NESOB.memory[0x0001] == 0x3F)
        {
            //breakpoint = true;
        }
        if(breakpoint == true)
        {
            printRegs();
            printf("Continue?\n");
            cin>>options;
            if(options == 'm')
            {
                memDump();
            }
            if(options == 'o')
            {
                breakpoint = false;
            }
            if(options == 'r')
            {
                NESOB.closeProgram = true;
            }
            if(options == 'i')
            {
                NESOB.pc++;
            }
        }
        if(restartNopNES == true)
        {
            main();
        }
        if(NESOB.closeProgram == true)
        {
            memDump();
            exitLoop = true;
            //fclose (logfile);
            return 0;
        }

    }
    return 0;
}
