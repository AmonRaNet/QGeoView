TEMPLATE = subdirs
SUBDIRS = \
    lib \
    samples/shared \
    samples/basic \
    samples/background \
    samples/widgets \
    samples/flags \
    samples/layers \
    samples/fun \
    samples/10000 \
    samples/debug \
    samples/custom-tiles \
    samples/moving-objects \
    samples/mouse-actions \
    samples/camera-actions

samples.shared.depends = lib
samples.basic.depends = lib samples.shared
samples.background.depends = lib samples.shared
samples.widgets.depends = lib samples.shared
samples.flags.depends = lib samples.shared
samples.layers.depends = lib samples.shared
samples.fun.depends = lib samples.shared
samples.10000.depends = lib samples.shared
samples.debug.depends = lib samples.shared
samples.custom-tiles.depends = lib samples.shared
samples.moving-objects.depends = lib samples.shared
samples.mouse-actions.depends = lib samples.shared
samples.camera-actions.depends = lib samples.shared
