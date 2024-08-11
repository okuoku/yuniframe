#include <yuniframe/yfrm.h>

#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

/* Event handling */
#define MIN_EVENT_SIZE 32 /* ??? */

static int32_t
fill_mousebuttonbitmap(uint8_t btn){
    int32_t r = 0;
    if(btn & SDL_BUTTON(1)){
        r |= 1;
    }
    if(btn & SDL_BUTTON(2)){
        r |= 2;
    }
    if(btn & SDL_BUTTON(3)){
        r |= 4;
    }
    if(btn & SDL_BUTTON(4)){
        r |= 8;
    }
    if(btn & SDL_BUTTON(5)){
        r |= 16;
    }
    return r;
}

static size_t
fill_mousemotionevent(int32_t* buf, size_t offs, SDL_Event* evt){
    const int32_t LEN_mousemotionevent = 7;
    /* 0 */ const int32_t len = LEN_mousemotionevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t x;
    /* 3 */ int32_t y;
    /* 4 */ int32_t xrel;
    /* 5 */ int32_t yrel;
    /* 6 */ int32_t buttons;
    type = 3;
    x = evt->motion.x;
    y = evt->motion.y;
    xrel = evt->motion.xrel;
    yrel = evt->motion.yrel;
    buttons = fill_mousebuttonbitmap(evt->motion.state);

    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = x;
    offs++;
    buf[offs] = y;
    offs++;
    buf[offs] = xrel;
    offs++;
    buf[offs] = yrel;
    offs++;
    buf[offs] = buttons;
    offs++;

    return offs;
}

static size_t
fill_mousewheelevent(int32_t* buf, size_t offs, SDL_Event* evt){
    const int32_t LEN_mousewheelevent = 4;
    /* 0 */ const int32_t len = LEN_mousewheelevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t dx;
    /* 3 */ int32_t dy;
    type = 2;
    dx = evt->wheel.x;
    dy = evt->wheel.y;

    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = dx;
    offs++;
    buf[offs] = dy;
    offs++;

    return offs;
}


static size_t
fill_mousebuttonevent(int32_t* buf, size_t offs, SDL_Event* evt){
    const int32_t LEN_mousebuttonevent = 6;
    /* 0 */ const int32_t len = LEN_mousebuttonevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t x;
    /* 3 */ int32_t y;
    /* 4 */ int32_t button;
    /* 5 */ int32_t buttons;
    if(evt->type == SDL_MOUSEBUTTONDOWN){
        type = 0;
    }else{
        type = 1;
    }
    x = evt->button.x;
    y = evt->button.y;
    button = fill_mousebuttonbitmap(evt->button.button);
    buttons = fill_mousebuttonbitmap(SDL_GetMouseState(NULL, NULL));
    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = x;
    offs++;
    buf[offs] = y;
    offs++;
    buf[offs] = button;
    offs++;
    buf[offs] = buttons;
    offs++;

    return offs;
}

static size_t
fill_controllerbuttonevent(int32_t* buf, size_t offs,SDL_Event* evt){
    const int32_t LEN_controllerbuttonevent = 4;
    /* 0 */ const int32_t len = LEN_controllerbuttonevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t ident;
    /* 3 */ int32_t button;

    if(evt->cbutton.state == SDL_PRESSED){
        type = 101;
    }else{
        type = 100;
    }

    ident = 1;
    /* GamePad API order: A B X Y L1 R1 L2 R2 SEL START L3 R3
     *                    UP DOWN LEFT RIGHT META */
    switch(evt->cbutton.button){
        case SDL_CONTROLLER_BUTTON_A:
            button = 0;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            button = 1;
            break;
        case SDL_CONTROLLER_BUTTON_X:
            button = 2;
            break;
        case SDL_CONTROLLER_BUTTON_Y:
            button = 3;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            button = 4;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            button = 5;
            break;
        /* Missing: L2 R2 */
        case SDL_CONTROLLER_BUTTON_BACK:
            button = 8;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            button = 9;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
            button = 10;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
            button = 11;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            button = 12;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            button = 13;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            button = 14;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            button = 15;
            break;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            button = 16;
            break;
        default:
            button = -1;
            break;
    }

    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = ident;
    offs++;
    buf[offs] = button;
    offs++;

    return offs;
}

static size_t
fill_controlleraxisevent(int32_t* buf, size_t offs,SDL_Event* evt){
    const int32_t LEN_controlleraxisevent = 6;
    /* 0 */ const int32_t len = LEN_controlleraxisevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t ident;
    /* 3 */ int32_t axis;
    /* 4 */ int32_t value;
    /* 5 */ int32_t frac;

    type = 102;
    ident = 1;
    switch(evt->caxis.axis){
        case SDL_CONTROLLER_AXIS_LEFTX:
            axis = 0;
            break;
        case SDL_CONTROLLER_AXIS_LEFTY:
            axis = 1;
            break;
        case SDL_CONTROLLER_AXIS_RIGHTX:
            axis = 2;
            break;
        case SDL_CONTROLLER_AXIS_RIGHTY:
            axis = 3;
            break;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            axis = 4;
            break;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            axis = 5;
            break;
        default:
            axis = -1;
            break;
    }
    value = evt->caxis.value;
    frac = 32768;

    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = ident;
    offs++;
    buf[offs] = axis;
    offs++;
    buf[offs] = value;
    offs++;
    buf[offs] = frac;
    offs++;

    return offs;
}

static size_t
fill_keyevent(int32_t* buf, size_t offs, SDL_Event* evt){
    const int32_t LEN_keyevent = 5;
    /* 0 */ const int32_t len = LEN_keyevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t keycode;
    /* 3 */ int32_t flags;
    /* 4 */ int32_t keyname; /* Unicode */
    if(evt->type == SDL_KEYDOWN){
        type = 200;
    }else{
        type = 201;
    }
    switch(evt->key.keysym.scancode){
#define KEYMAP(TO,FROM) case FROM: keycode = TO; break
        KEYMAP(3 , SDL_SCANCODE_CANCEL);
        KEYMAP(6 , SDL_SCANCODE_HELP);
        KEYMAP(8 , SDL_SCANCODE_BACKSPACE);
        KEYMAP(9 , SDL_SCANCODE_TAB);
        //KEYMAP(12 , SDL_SCANCODE_KP_5);
        KEYMAP(13 , SDL_SCANCODE_RETURN);
        KEYMAP(16 , SDL_SCANCODE_LSHIFT);
        KEYMAP(17 , SDL_SCANCODE_LCTRL);
        KEYMAP(18 , SDL_SCANCODE_LALT);
        KEYMAP(19 , SDL_SCANCODE_PAUSE);
        KEYMAP(20 , SDL_SCANCODE_CAPSLOCK);
        KEYMAP(27 , SDL_SCANCODE_ESCAPE);
        KEYMAP(32 , SDL_SCANCODE_SPACE);
        KEYMAP(33 , SDL_SCANCODE_PAGEUP);
        KEYMAP(34 , SDL_SCANCODE_PAGEDOWN);
        KEYMAP(35 , SDL_SCANCODE_END);
        KEYMAP(36 , SDL_SCANCODE_HOME);
        KEYMAP(37 , SDL_SCANCODE_LEFT);
        KEYMAP(38 , SDL_SCANCODE_UP);
        KEYMAP(39 , SDL_SCANCODE_RIGHT);
        KEYMAP(40 , SDL_SCANCODE_DOWN);
        KEYMAP(45 , SDL_SCANCODE_INSERT);
        KEYMAP(46 , SDL_SCANCODE_DELETE);
        KEYMAP(48 , SDL_SCANCODE_0);
        KEYMAP(49 , SDL_SCANCODE_1);
        KEYMAP(50 , SDL_SCANCODE_2);
        KEYMAP(51 , SDL_SCANCODE_3);
        KEYMAP(52 , SDL_SCANCODE_4);
        KEYMAP(53 , SDL_SCANCODE_5);
        KEYMAP(54 , SDL_SCANCODE_6);
        KEYMAP(55 , SDL_SCANCODE_7);
        KEYMAP(56 , SDL_SCANCODE_8);
        KEYMAP(57 , SDL_SCANCODE_9);
        KEYMAP(59 , SDL_SCANCODE_SEMICOLON);
        KEYMAP(60 , SDL_SCANCODE_NONUSBACKSLASH);
        KEYMAP(61 , SDL_SCANCODE_EQUALS);
        KEYMAP(63 , SDL_SCANCODE_MINUS);
        KEYMAP(65 , SDL_SCANCODE_A);
        KEYMAP(66 , SDL_SCANCODE_B);
        KEYMAP(67 , SDL_SCANCODE_C);
        KEYMAP(68 , SDL_SCANCODE_D);
        KEYMAP(69 , SDL_SCANCODE_E);
        KEYMAP(70 , SDL_SCANCODE_F);
        KEYMAP(71 , SDL_SCANCODE_G);
        KEYMAP(72 , SDL_SCANCODE_H);
        KEYMAP(73 , SDL_SCANCODE_I);
        KEYMAP(74 , SDL_SCANCODE_J);
        KEYMAP(75 , SDL_SCANCODE_K);
        KEYMAP(76 , SDL_SCANCODE_L);
        KEYMAP(77 , SDL_SCANCODE_M);
        KEYMAP(78 , SDL_SCANCODE_N);
        KEYMAP(79 , SDL_SCANCODE_O);
        KEYMAP(80 , SDL_SCANCODE_P);
        KEYMAP(81 , SDL_SCANCODE_Q);
        KEYMAP(82 , SDL_SCANCODE_R);
        KEYMAP(83 , SDL_SCANCODE_S);
        KEYMAP(84 , SDL_SCANCODE_T);
        KEYMAP(85 , SDL_SCANCODE_U);
        KEYMAP(86 , SDL_SCANCODE_V);
        KEYMAP(87 , SDL_SCANCODE_W);
        KEYMAP(88 , SDL_SCANCODE_X);
        KEYMAP(89 , SDL_SCANCODE_Y);
        KEYMAP(90 , SDL_SCANCODE_Z);
        KEYMAP(91 , SDL_SCANCODE_LGUI);
        KEYMAP(93 , SDL_SCANCODE_APPLICATION);
        KEYMAP(96 , SDL_SCANCODE_KP_0);
        KEYMAP(97 , SDL_SCANCODE_KP_1);
        KEYMAP(98 , SDL_SCANCODE_KP_2);
        KEYMAP(99 , SDL_SCANCODE_KP_3);
        KEYMAP(100 , SDL_SCANCODE_KP_4);
        KEYMAP(101 , SDL_SCANCODE_KP_5);
        KEYMAP(102 , SDL_SCANCODE_KP_6);
        KEYMAP(103 , SDL_SCANCODE_KP_7);
        KEYMAP(104 , SDL_SCANCODE_KP_8);
        KEYMAP(105 , SDL_SCANCODE_KP_9);
        KEYMAP(106 , SDL_SCANCODE_KP_MULTIPLY);
        KEYMAP(107 , SDL_SCANCODE_KP_PLUS);
        KEYMAP(109 , SDL_SCANCODE_KP_MINUS);
        KEYMAP(110 , SDL_SCANCODE_KP_PERIOD);
        KEYMAP(111 , SDL_SCANCODE_KP_DIVIDE);
        KEYMAP(112 , SDL_SCANCODE_F1);
        KEYMAP(113 , SDL_SCANCODE_F2);
        KEYMAP(114 , SDL_SCANCODE_F3);
        KEYMAP(115 , SDL_SCANCODE_F4);
        KEYMAP(116 , SDL_SCANCODE_F5);
        KEYMAP(117 , SDL_SCANCODE_F6);
        KEYMAP(118 , SDL_SCANCODE_F7);
        KEYMAP(119 , SDL_SCANCODE_F8);
        KEYMAP(120 , SDL_SCANCODE_F9);
        KEYMAP(121 , SDL_SCANCODE_F10);
        KEYMAP(122 , SDL_SCANCODE_F11);
        KEYMAP(123 , SDL_SCANCODE_F12);
        KEYMAP(124 , SDL_SCANCODE_F13);
        KEYMAP(125 , SDL_SCANCODE_F14);
        KEYMAP(126 , SDL_SCANCODE_F15);
        KEYMAP(127 , SDL_SCANCODE_F16);
        KEYMAP(128 , SDL_SCANCODE_F17);
        KEYMAP(129 , SDL_SCANCODE_F18);
        KEYMAP(130 , SDL_SCANCODE_F19);
        KEYMAP(131 , SDL_SCANCODE_F20);
        KEYMAP(132 , SDL_SCANCODE_F21);
        KEYMAP(133 , SDL_SCANCODE_F22);
        KEYMAP(134 , SDL_SCANCODE_F23);
        KEYMAP(135 , SDL_SCANCODE_F24);
        KEYMAP(144 , SDL_SCANCODE_NUMLOCKCLEAR);
        KEYMAP(145 , SDL_SCANCODE_SCROLLLOCK);
        KEYMAP(160 , SDL_SCANCODE_GRAVE);
        KEYMAP(163 , SDL_SCANCODE_KP_HASH);
        //KEYMAP(170 , SDL_SCANCODE_KP_MULTIPLY);
        KEYMAP(171 , SDL_SCANCODE_RIGHTBRACKET);
        //KEYMAP(173 , SDL_SCANCODE_MINUS);
        KEYMAP(174 , SDL_SCANCODE_VOLUMEDOWN);
        KEYMAP(175 , SDL_SCANCODE_VOLUMEUP);
        KEYMAP(176 , SDL_SCANCODE_AUDIONEXT);
        KEYMAP(177 , SDL_SCANCODE_AUDIOPREV);
        KEYMAP(179 , SDL_SCANCODE_AUDIOPLAY);
        KEYMAP(181 , SDL_SCANCODE_AUDIOMUTE);
        //KEYMAP(182 , SDL_SCANCODE_VOLUMEDOWN);
        //KEYMAP(183 , SDL_SCANCODE_VOLUMEUP);
        //KEYMAP(186 , SDL_SCANCODE_SEMICOLON);
        //KEYMAP(187 , SDL_SCANCODE_EQUALS);
        KEYMAP(188 , SDL_SCANCODE_COMMA);
        //KEYMAP(189 , SDL_SCANCODE_MINUS);
        KEYMAP(190 , SDL_SCANCODE_PERIOD);
        KEYMAP(191 , SDL_SCANCODE_SLASH);
        //KEYMAP(192 , SDL_SCANCODE_GRAVE);
        KEYMAP(219 , SDL_SCANCODE_LEFTBRACKET);
        KEYMAP(220 , SDL_SCANCODE_BACKSLASH);
        //KEYMAP(221 , SDL_SCANCODE_RIGHTBRACKET);
        KEYMAP(222 , SDL_SCANCODE_APOSTROPHE);
        default:
            keycode = evt->key.keysym.sym;
            break;
    }

    flags = 0;
    if(evt->key.repeat){
        flags |= 1;
    }
    if(evt->key.keysym.mod & KMOD_SHIFT){
        flags |= 2;
    }
    if(evt->key.keysym.mod & KMOD_CTRL){
        flags |= 4;
    }
    if(evt->key.keysym.mod & KMOD_ALT){
        flags |= 8;
    }
    if(evt->key.keysym.mod & KMOD_GUI){
        flags |= 16;
    }
    keyname = evt->key.keysym.sym < 0x100 ? evt->key.keysym.sym : 0;

    buf[offs] = len;
    offs++;
    buf[offs] = type;
    offs++;
    buf[offs] = keycode;
    offs++;
    buf[offs] = flags;
    offs++;
    buf[offs] = keyname;
    offs++;

    return offs;
}
static size_t
fill_windowevent(int32_t* buf, size_t offs, SDL_Event* evt){
    const int32_t LEN_windowresizeevent = 4;
    /* 0 */ const int32_t len_resize = LEN_windowresizeevent;
    /* 1 */ int32_t type;
    /* 2 */ int32_t x;
    /* 3 */ int32_t y;
    if(evt->window.event == SDL_WINDOWEVENT_RESIZED){
        x = evt->window.data1;
        y = evt->window.data2;
        buf[offs] = len_resize;
        offs++;
        buf[offs] = type;
        offs++;
        buf[offs] = x;
        offs++;
        buf[offs] = y;
        offs++;
    }
    return offs;
}


SDL_GameController* cur_controller = NULL;

YFRM_API int
yfrm_query0(int32_t slot, int32_t* buf, size_t buflen){
    SDL_Event evt;
    if(slot == 0 /* events */){
        size_t cur = 0;

        /* Init gamepad if we don't have one */
        if(! cur_controller){
            if(0 < SDL_NumJoysticks()){
                cur_controller = SDL_GameControllerOpen(0);
            }
        }

        while(((buflen - cur) > MIN_EVENT_SIZE)){
            if(!SDL_PollEvent(&evt)){
                break;
            }
            switch(evt.type){
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    cur = fill_mousebuttonevent(buf, cur, &evt);
                    break;
                case SDL_MOUSEMOTION:
                    cur = fill_mousemotionevent(buf, cur, &evt);
                    break;
                case SDL_MOUSEWHEEL:
                    cur = fill_mousewheelevent(buf, cur, &evt);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    cur = fill_controllerbuttonevent(buf, cur, &evt);
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    cur = fill_controlleraxisevent(buf, cur, &evt);
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    cur = fill_keyevent(buf, cur, &evt);
                    break;
                case SDL_WINDOWEVENT:
                    cur = fill_windowevent(buf, cur, &evt);
                    break;
                default:
                    /* Do nothing */
                    break;
            }
        }
        return cur;
    }else{
        return -1;
    }
}
