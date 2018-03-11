TEMPLATE = subdirs

CONFIG += ordered   # This tells Qt to compile the following SUBDIRS in order

SUBDIRS += src \
    3rdparty/fluid

PROJECT_NAME = RecordRack2
PROJECT_VERSION = 0.1.0

DISTNAME = $${PROJECT_NAME}-$${PROJECT_VERSION}
