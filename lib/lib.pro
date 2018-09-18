TARGET = qgeoview
TEMPLATE = lib

include(lib.pri)

target.path = /lib
INSTALLS += target

header_files.files = $$HEADERS
header_files.path = /include/QGeoView
INSTALLS += header_files
