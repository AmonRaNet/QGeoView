TARGET = qgeoview
TEMPLATE = lib

include(lib.pri)

DESTDIR = $$(QGV_DESTDIR)

target.path = $${DESTDIR}/lib
target.CONFIG = no_check_exist
INSTALLS += target

header_files.files = $$HEADERS
header_files.path = $${DESTDIR}/include/QGeoView
header_files.CONFIG = no_check_exist
INSTALLS += header_files
