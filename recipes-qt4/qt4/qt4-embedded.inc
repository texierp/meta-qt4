SUMMARY = "Cross-platform UI toolkit and application framework (framebuffer version)"
DESCRIPTION = "Qt is a versatile cross-platform application framework -- this is the embedded (framebuffer) version."
SECTION = "libs"
HOMEPAGE = "http://qt-project.org/"

PACKAGECONFIG ??= '${@bb.utils.contains("MACHINE_FEATURES", "touchscreen", "tslib", "",d)}'
PACKAGECONFIG[tslib] = " -plugin-mouse-tslib, ,tslib"

QT4EDEPENDS = ""
QT_BASE_LIB  ?= "libqt-embedded"

# Set necessary variables in the profile
SRC_URI += "file://qte.sh \
            file://0033-configure-support-c-0x-standard-for-directfd.patch \
"

QT_EMBEDDED_FLAGS ?= " \
    -embedded $QT_ARCH \
    -qtlibinfix ${QT_LIBINFIX} \
"
QT_EMBEDDED_EXTRA_FLAGS ?= " \
    -plugin-gfx-transformed -plugin-gfx-qvfb -plugin-gfx-vnc \
    -qt-mouse-pc -qt-mouse-qvfb -qt-mouse-linuxinput \
    -qt-kbd-tty -qt-kbd-linuxinput ${PACKAGECONFIG_CONFARGS} \
"
QT_EMBEDDED_KEYPAD_FLAGS ?= " \
    -DQT_KEYPAD_NAVIGATION \
"

QT_CONFIG_FLAGS += "${QT_EMBEDDED_FLAGS} ${QT_EMBEDDED_EXTRA_FLAGS} ${QT_EMBEDDED_KEYPAD_FLAGS}"

require qt4.inc

do_install:append() {
    install -d ${D}/${libdir}/fonts
    touch ${D}/${libdir}/fonts/fontdir

    install -d ${D}${sysconfdir}/profile.d/
    install -m 0755 ${WORKDIR}/qte.sh ${D}${sysconfdir}/profile.d/
}

# We put the qte profile into it's own package as we don't want to install all qt stuff
# with depending on the global package qt4-embedded
PACKAGES += " ${PN}-conf"
FILES:${PN}-conf += " ${sysconfdir}/profile.d/qte.sh"
RRECOMMENDS:${PN} += " ${PN}-conf"

OTHER_PACKAGES += "\
             ${QT_BASE_NAME}-fonts \
             ${QT_BASE_NAME}-fonts-ttf-vera \
             ${QT_BASE_NAME}-fonts-ttf-dejavu \
             ${QT_BASE_NAME}-fonts-pfa \
             ${QT_BASE_NAME}-fonts-pfb \
             ${QT_BASE_NAME}-fonts-qpf"

RRECOMMENDS:${QT_BASE_NAME}-fonts = " \
             ${QT_BASE_NAME}-fonts-ttf-vera \
             ${QT_BASE_NAME}-fonts-ttf-dejavu \
             ${QT_BASE_NAME}-fonts-pfa \
             ${QT_BASE_NAME}-fonts-pfb \
             ${QT_BASE_NAME}-fonts-qpf"
RRECOMMENDS:${QT_BASE_NAME}-demos += " \
            ${QT_BASE_NAME}-fonts"

ALLOW_EMPTY:${QT_BASE_NAME}-fonts = "1"
PACKAGES_DYNAMIC += "^${QT_BASE_NAME}-fonts-.*"

FILES:${QT_BASE_NAME}-fonts-ttf-vera       = "${libdir}/fonts/Vera*.ttf"
FILES:${QT_BASE_NAME}-fonts-ttf-dejavu     = "${libdir}/fonts/DejaVu*.ttf"
FILES:${QT_BASE_NAME}-fonts-pfa            = "${libdir}/fonts/*.pfa"
FILES:${QT_BASE_NAME}-fonts-pfb            = "${libdir}/fonts/*.pfb"
FILES:${QT_BASE_NAME}-fonts-qpf            = "${libdir}/fonts/*.qpf*"
FILES:${QT_BASE_NAME}-fonts                = "${libdir}/fonts/README ${libdir}/fonts/fontdir"

inherit pkgconfig qt4e

