#include "radapter/radapter.hpp"
#include "radapter/worker.hpp"
#include "radapter/config.hpp"
#include "X11/keysym.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

static int CodeToButton(QStringView code) {
    if (code == u"LMB") return Button1;
    else if (code == u"MMB") return Button2;
    else if (code == u"RMB") return Button3;
    else if (code == u"FMB") return Button4;
    else if (code == u"BMB") return Button5;
    else return 0;
}

static KeySym CodeToXSym(QStringView code, bool shift)
{
    if (code == u"LEFT") return XK_Left;
    else if (code == u"UP") return XK_Up;
    else if (code == u"RIGHT") return XK_Right;
    else if (code == u"DOWN") return XK_Down;

    else if (code == u"CTRL_L" || code == u"CTRL") return XK_Control_L;
    else if (code == u"CTRL_R") return XK_Control_R;
    else if (code == u"SHIFT_L") return XK_Shift_L;
    else if (code == u"SHIFT_R") return XK_Shift_R;
    else if (code == u"ALT_L") return XK_Alt_L;
    else if (code == u"ALT_R") return XK_Alt_R;

    else if (code == u"ENTER" || code == u"RETURN") return XK_Return;
    else if (code == u"SUPER") return XK_Super_L;
    else if (code == u"TAB") return XK_Tab;
    else if (code == u"CAPS_LOCK") return XK_Caps_Lock;
    else if (code == u"ESCAPE") return XK_Escape;
    else if (code == u"COMMA") return XK_comma;
    else if (code == u"SPACE") return XK_space;
    else if (code == u"FN") return 0;

    else if (code == u"A") return shift ? XK_A : XK_a;
    else if (code == u"B") return shift ? XK_B : XK_b;
    else if (code == u"C") return shift ? XK_C : XK_c;
    else if (code == u"D") return shift ? XK_D : XK_d;
    else if (code == u"E") return shift ? XK_E : XK_e;
    else if (code == u"F") return shift ? XK_F : XK_f;
    else if (code == u"G") return shift ? XK_G : XK_g;
    else if (code == u"H") return shift ? XK_H : XK_h;
    else if (code == u"I") return shift ? XK_I : XK_i;
    else if (code == u"J") return shift ? XK_J : XK_j;
    else if (code == u"L") return shift ? XK_L : XK_l;
    else if (code == u"M") return shift ? XK_M : XK_m;
    else if (code == u"N") return shift ? XK_N : XK_n;
    else if (code == u"O") return shift ? XK_O : XK_o;
    else if (code == u"P") return shift ? XK_P : XK_p;
    else if (code == u"K") return shift ? XK_K : XK_k;
    else if (code == u"Q") return shift ? XK_Q : XK_q;
    else if (code == u"R") return shift ? XK_R : XK_r;
    else if (code == u"S") return shift ? XK_S : XK_s;
    else if (code == u"T") return shift ? XK_T : XK_t;
    else if (code == u"U") return shift ? XK_U : XK_u;
    else if (code == u"V") return shift ? XK_V : XK_v;
    else if (code == u"W") return shift ? XK_W : XK_w;
    else if (code == u"X") return shift ? XK_X : XK_x;
    else if (code == u"Z") return shift ? XK_Z : XK_z;
    else if (code == u"Y") return shift ? XK_Y : XK_y;

    else if (code == u"[") return XK_braceleft;
    else if (code == u"]") return XK_braceright;
    else if (code == u"{") return XK_bracketleft;
    else if (code == u"}") return XK_bracketright;
    else if (code == u"BACKSPACE") return XK_BackSpace;

    else if (code == u"~") return XK_asciitilde;
    else if (code == u"!") return XK_exclam;
    else if (code == u"@") return XK_at;
    else if (code == u"%") return XK_percent;
    else if (code == u"^") return XK_ampersand;
    else if (code == u"*") return XK_asterisk;
    else if (code == u"#") return XK_numbersign;
    else if (code == u"/") return XK_slash;
    else if (code == u"\\") return XK_backslash;
    else if (code == u"+") return XK_plus;
    else if (code == u"-") return XK_minus;
    else if (code == u"=") return XK_equal;

    else if (code == u"1") return XK_1;
    else if (code == u"2") return XK_2;
    else if (code == u"3") return XK_3;
    else if (code == u"4") return XK_4;
    else if (code == u"5") return XK_5;
    else if (code == u"6") return XK_6;
    else if (code == u"7") return XK_7;
    else if (code == u"8") return XK_8;
    else if (code == u"9") return XK_9;
    else if (code == u"0") return XK_0;

    else if (code == u"F1") return XK_F1;
    else if (code == u"F2") return XK_F2;
    else if (code == u"F3") return XK_F3;
    else if (code == u"F4") return XK_F4;
    else if (code == u"F5") return XK_F5;
    else if (code == u"F6") return XK_F6;
    else if (code == u"F7") return XK_F7;
    else if (code == u"F8") return XK_F8;
    else if (code == u"F9") return XK_F9;
    else if (code == u"F10") return XK_F10;
    else if (code == u"F11") return XK_F11;
    else if (code == u"F12") return XK_F12;

    else if (code == u"Ё") return shift ? XK_Cyrillic_IO : XK_Cyrillic_io;
    else if (code == u"Й") return shift ? XK_Cyrillic_SHORTI : XK_Cyrillic_shorti;
    else if (code == u"Ц") return shift ? XK_Serbian_TSHE : XK_Serbian_tshe;
    else if (code == u"У") return shift ? XK_Cyrillic_U : XK_Cyrillic_u;
    else if (code == u"К") return shift ? XK_Cyrillic_KA : XK_Cyrillic_ka;
    else if (code == u"Е") return shift ? XK_Cyrillic_JE : XK_Cyrillic_je;
    else if (code == u"Н") return shift ? XK_Cyrillic_EN : XK_Cyrillic_en;
    else if (code == u"Г") return shift ? XK_Cyrillic_GHE : XK_Cyrillic_ghe;
    else if (code == u"Ш") return shift ? XK_Cyrillic_SHA : XK_Cyrillic_sha;
    else if (code == u"Щ") return shift ? XK_Cyrillic_SHCHA : XK_Cyrillic_shcha;
    else if (code == u"З") return shift ? XK_Cyrillic_ZE : XK_Cyrillic_ze;
    else if (code == u"Х") return shift ? XK_Cyrillic_HA : XK_Cyrillic_ha;
    else if (code == u"Ъ") return shift ? XK_Cyrillic_HARDSIGN : XK_Cyrillic_hardsign;
    else if (code == u"Ф") return shift ? XK_Cyrillic_EF : XK_Cyrillic_ef;
    else if (code == u"Ы") return shift ? XK_Ukrainian_YI : XK_Ukrainian_yi; //ы
    else if (code == u"В") return shift ? XK_Cyrillic_VE : XK_Cyrillic_ve;
    else if (code == u"А") return shift ? XK_Cyrillic_A : XK_Cyrillic_a;
    else if (code == u"П") return shift ? XK_Cyrillic_PE : XK_Cyrillic_pe;
    else if (code == u"Р") return shift ? XK_Cyrillic_ER : XK_Cyrillic_er;
    else if (code == u"О") return shift ? XK_Cyrillic_O : XK_Cyrillic_o;
    else if (code == u"Л") return shift ? XK_Cyrillic_EL : XK_Cyrillic_el;
    else if (code == u"Д") return shift ? XK_Cyrillic_DE : XK_Cyrillic_de;
    else if (code == u"Ж") return shift ? XK_Cyrillic_DZHE : XK_Cyrillic_dzhe;
    else if (code == u"Э") return shift ? XK_Cyrillic_E : XK_Cyrillic_e; //э
    else if (code == u"Я") return shift ? XK_Cyrillic_YA : XK_Cyrillic_ya;
    else if (code == u"Ч") return shift ? XK_Cyrillic_CHE : XK_Cyrillic_che;
    else if (code == u"С") return shift ? XK_Cyrillic_ES : XK_Cyrillic_es;
    else if (code == u"М") return shift ? XK_Cyrillic_EM : XK_Cyrillic_em;
    else if (code == u"И") return shift ? XK_Cyrillic_I : XK_Cyrillic_i;
    else if (code == u"Т") return shift ? XK_Cyrillic_TE : XK_Cyrillic_te;
    else if (code == u"Ь") return shift ? XK_Cyrillic_SOFTSIGN : XK_Cyrillic_softsign;
    else if (code == u"Б") return shift ? XK_Cyrillic_BE : XK_Cyrillic_be;
    else if (code == u"Ю") return shift ? XK_Cyrillic_YU : XK_Cyrillic_yu;

    else if (code == u".") return XK_period;
    else if (code == u";") return XK_semicolon;
    else if (code == u":") return XK_colon;
    else if (code == u"\"") return XK_quotedbl;
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
            MapPress(kb->key, kb->press);
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
        auto upper = code.toUpper();
        bool shift = upper == code;
        if (upper.endsWith("MB")) {
            auto but = CodeToButton(upper);
            if (!but) {
                Warn("Invalid button: {}", code);
                return;
            }
            auto ok = XTestFakeButtonEvent(disp, but, xpress, CurrentTime);
            check(ok, "Mouse click");
        } else {
            auto sym = CodeToXSym(upper, shift);
            if (!sym) {
                Warn("Invalid symbol: {}", code);
                return;
            }
            auto ok = XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, sym), xpress, CurrentTime);
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
