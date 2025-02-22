#include "radapter/radapter.hpp"
#include "radapter/worker.hpp"
#include "radapter/config.hpp"
#include "X11/keysym.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

static int CodeToButton(QString const& code) {
    if (code == "LMB") return Button1;
    else if (code == "MMB") return Button2;
    else if (code == "RMB") return Button3;
    else if (code == "FMB") return Button4;
    else if (code == "BMB") return Button5;
    else return -1;
}

// todo: directly translate: without shift
static KeySym CodeToXSym(QString const& code, bool shift)
{
    if (code == "LEFT") return XK_Left;
    else if (code == "UP") return XK_Up;
    else if (code == "RIGHT") return XK_Right;
    else if (code == "DOWN") return XK_Down;

    else if (code == "CTRL_L") return XK_Control_L;
    else if (code == "CTRL_R") return XK_Control_R;
    else if (code == "SHIFT_L") return XK_Shift_L;
    else if (code == "SHIFT_R") return XK_Shift_R;
    else if (code == "ALT_L") return XK_Alt_L;
    else if (code == "ALT_R") return XK_Alt_R;

    else if (code == "ENTER") return XK_Return;
    else if (code == "SUPER") return XK_Super_L;
    else if (code == "TAB") return XK_Tab;
    else if (code == "CAPS_LOCK") return XK_Caps_Lock;
    else if (code == "ESCAPE") return XK_Escape;
    else if (code == "COMMA") return XK_comma;
    else if (code == "SPACE") return XK_space;
    else if (code == "FN") return 0;

    else if (code == "A") return shift ? XK_A : XK_a;
    else if (code == "B") return shift ? XK_B : XK_b;
    else if (code == "C") return shift ? XK_C : XK_c;
    else if (code == "D") return shift ? XK_D : XK_d;
    else if (code == "E") return shift ? XK_E : XK_e;
    else if (code == "F") return shift ? XK_F : XK_f;
    else if (code == "G") return shift ? XK_G : XK_g;
    else if (code == "H") return shift ? XK_H : XK_h;
    else if (code == "I") return shift ? XK_I : XK_i;
    else if (code == "J") return shift ? XK_J : XK_j;
    else if (code == "L") return shift ? XK_L : XK_l;
    else if (code == "M") return shift ? XK_M : XK_m;
    else if (code == "N") return shift ? XK_N : XK_n;
    else if (code == "O") return shift ? XK_O : XK_o;
    else if (code == "P") return shift ? XK_P : XK_p;
    else if (code == "K") return shift ? XK_K : XK_k;
    else if (code == "Q") return shift ? XK_Q : XK_q;
    else if (code == "R") return shift ? XK_R : XK_r;
    else if (code == "S") return shift ? XK_S : XK_s;
    else if (code == "T") return shift ? XK_T : XK_t;
    else if (code == "U") return shift ? XK_U : XK_u;
    else if (code == "V") return shift ? XK_V : XK_v;
    else if (code == "W") return shift ? XK_W : XK_w;
    else if (code == "X") return shift ? XK_X : XK_x;
    else if (code == "Z") return shift ? XK_Z : XK_z;
    else if (code == "Y") return shift ? XK_Y : XK_y;

    else if (code == "[") return XK_braceleft;
    else if (code == "]") return XK_braceright;
    else if (code == "{") return XK_bracketleft;
    else if (code == "}") return XK_bracketright;
    else if (code == "BACKSPACE") return XK_BackSpace;

    else if (code == "~") return XK_asciitilde;
    else if (code == "!") return XK_exclam;
    else if (code == "@") return XK_at;
    else if (code == "%") return XK_percent;
    else if (code == "^") return XK_ampersand;
    else if (code == "*") return XK_asterisk;
    else if (code == "#") return XK_numbersign;
    else if (code == "/") return XK_slash;
    else if (code == "\\") return XK_backslash;
    else if (code == "+") return XK_plus;
    else if (code == "-") return XK_minus;
    else if (code == "=") return XK_equal;

    else if (code == "1") return XK_1;
    else if (code == "2") return XK_2;
    else if (code == "3") return XK_3;
    else if (code == "4") return XK_4;
    else if (code == "5") return XK_5;
    else if (code == "6") return XK_6;
    else if (code == "7") return XK_7;
    else if (code == "8") return XK_8;
    else if (code == "9") return XK_9;
    else if (code == "0") return XK_0;

    else if (code == "F1") return XK_F1;
    else if (code == "F2") return XK_F2;
    else if (code == "F3") return XK_F3;
    else if (code == "F4") return XK_F4;
    else if (code == "F5") return XK_F5;
    else if (code == "F6") return XK_F6;
    else if (code == "F7") return XK_F7;
    else if (code == "F8") return XK_F8;
    else if (code == "F9") return XK_F9;
    else if (code == "F10") return XK_F10;
    else if (code == "F11") return XK_F11;
    else if (code == "F12") return XK_F12;

    else if (code == u"ё") return shift ? XK_Cyrillic_IO : XK_Cyrillic_io;
    else if (code == u"й") return shift ? XK_Cyrillic_SHORTI : XK_Cyrillic_shorti;
    else if (code == u"ц") return shift ? XK_Serbian_TSHE : XK_Serbian_tshe;
    else if (code == u"у") return shift ? XK_Cyrillic_U : XK_Cyrillic_u;
    else if (code == u"к") return shift ? XK_Cyrillic_KA : XK_Cyrillic_ka;
    else if (code == u"е") return shift ? XK_Cyrillic_JE : XK_Cyrillic_je;
    else if (code == u"н") return shift ? XK_Cyrillic_EN : XK_Cyrillic_en;
    else if (code == u"г") return shift ? XK_Cyrillic_GHE : XK_Cyrillic_ghe;
    else if (code == u"ш") return shift ? XK_Cyrillic_SHA : XK_Cyrillic_sha;
    else if (code == u"щ") return shift ? XK_Cyrillic_SHCHA : XK_Cyrillic_shcha;
    else if (code == u"з") return shift ? XK_Cyrillic_ZE : XK_Cyrillic_ze;
    else if (code == u"х") return shift ? XK_Cyrillic_HA : XK_Cyrillic_ha;
    else if (code == u"ъ") return shift ? XK_Cyrillic_HARDSIGN : XK_Cyrillic_hardsign;
    else if (code == u"ф") return shift ? XK_Cyrillic_EF : XK_Cyrillic_ef;
    else if (code == u"ы") return shift ? XK_Ukrainian_YI : XK_Ukrainian_yi; //ы
    else if (code == u"в") return shift ? XK_Cyrillic_VE : XK_Cyrillic_ve;
    else if (code == u"а") return shift ? XK_Cyrillic_A : XK_Cyrillic_a;
    else if (code == u"п") return shift ? XK_Cyrillic_PE : XK_Cyrillic_pe;
    else if (code == u"р") return shift ? XK_Cyrillic_ER : XK_Cyrillic_er;
    else if (code == u"о") return shift ? XK_Cyrillic_O : XK_Cyrillic_o;
    else if (code == u"л") return shift ? XK_Cyrillic_EL : XK_Cyrillic_el;
    else if (code == u"д") return shift ? XK_Cyrillic_DE : XK_Cyrillic_de;
    else if (code == u"ж") return shift ? XK_Cyrillic_DZHE : XK_Cyrillic_dzhe;
    else if (code == u"э") return shift ? XK_Cyrillic_E : XK_Cyrillic_e; //э
    else if (code == u"я") return shift ? XK_Cyrillic_YA : XK_Cyrillic_ya;
    else if (code == u"ч") return shift ? XK_Cyrillic_CHE : XK_Cyrillic_che;
    else if (code == u"с") return shift ? XK_Cyrillic_ES : XK_Cyrillic_es;
    else if (code == u"м") return shift ? XK_Cyrillic_EM : XK_Cyrillic_em;
    else if (code == u"и") return shift ? XK_Cyrillic_I : XK_Cyrillic_i;
    else if (code == u"т") return shift ? XK_Cyrillic_TE : XK_Cyrillic_te;
    else if (code == u"ь") return shift ? XK_Cyrillic_SOFTSIGN : XK_Cyrillic_softsign;
    else if (code == u"б") return shift ? XK_Cyrillic_BE : XK_Cyrillic_be;
    else if (code == u"ю") return shift ? XK_Cyrillic_YU : XK_Cyrillic_yu;

    else if (code == ".") return XK_period;
    else if (code == ";") return XK_semicolon;
    else if (code == ":") return XK_colon;
    else if (code == "\"") return XK_quotedbl;
    return 0;
}

using namespace radapter;

struct XorgKBMessage {
    QString key;
    bool press;
};
RAD_DESCRIBE(XorgKBMessage) {
    RAD_MEMBER(key);
    RAD_MEMBER(press);
}

struct XorgMouseMessage {
    float x;
    float y;
    float scroll;
};
RAD_DESCRIBE(XorgMouseMessage) {
    RAD_MEMBER(x);
    RAD_MEMBER(y);
    RAD_MEMBER(scroll);
}

struct XorgMessage {
    optional<XorgMouseMessage> mouse;
    optional<XorgKBMessage> kb;
};
RAD_DESCRIBE(XorgMessage) {
    RAD_MEMBER(mouse);
    RAD_MEMBER(kb);
}

class XorgPlugin final : public radapter::Worker
{
    Q_OBJECT
public:
    struct {
        uint16_t x = 0;
        uint16_t y = 0;
        unsigned modifs = 0;
    } input;
    Display* disp = {};
    Window root = {};

    XorgPlugin(QVariantList const& args, radapter::Instance* inst) : Worker(inst, "xorg") {
        disp = XOpenDisplay(nullptr);
        if (!disp) {
            throw Err("Could not open display");
        }
        root = XDefaultRootWindow(disp);
        if (!root) {
            throw Err("Could not get root window");
        }
    }
    void OnMsg(QVariant const& msg) override {
        XorgMessage parsed;
        Parse(parsed, msg);
        if (auto& mouse = parsed.mouse) {
            MapMouse(mouse->x, mouse->y, mouse->scroll);
        }
        if (auto& kb = parsed.kb) {
            MapPress(kb->key.toUpper(), kb->press);
        }
    }
    void check(int status, const char* ev) {
        if (!status) {
            Warn("Could not send: {}", ev);
        }
    }
    void MapMouse(uint16_t x, uint16_t y, float scroll) {
        auto ok = XTestFakeMotionEvent(disp, -1, x, y, CurrentTime);
        check(ok, "Mouse pos");
        if (std::abs(scroll) > std::numeric_limits<float>::epsilon()) {
            ok = XTestFakeButtonEvent(disp, scroll < 0 ? Button4 : Button5, True, CurrentTime);
            check(ok, "Mouse scroll");
        }
        XFlush(disp);
    }
    void MapPress(QString const& code, bool press) {
        auto xpress = press ? True : False;
        if (code == "LMB" || code == "RMB" || code == "MMB") {
            auto ok = XTestFakeButtonEvent(disp, CodeToButton(code), xpress, CurrentTime);
            check(ok, "Mouse click");
        } else {
            auto ok = XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, CodeToXSym(code, false)), xpress, CurrentTime);
            check(ok, "Key click");
        }
        XFlush(disp);
    }
    ~XorgPlugin() override {
        XCloseDisplay(disp);
    }
};

RADAPTER_PLUGIN("radapter.plugins.xorg") {
    radapter->RegisterWorker<XorgPlugin>("XorgPlugin");
}

#include "xorg.moc"
