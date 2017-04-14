AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES=libmimic.a
libmimic_a_CPPFLAGS= -I$(top_srcdir)/src
libmimic_a_SOURCES= \
    src/ConstantPool.cpp \
    src/ClassFile.cpp \
    src/ClassValidator.cpp \
    src/FieldDescriptor.cpp \
    src/JUtf8String.cpp \
    src/parsing/ByteConsumer.cpp

bin_PROGRAMS=mimic
mimic_LDFLAGS= -lpthread
mimic_LDADD=libmimic.a
mimic_SOURCES=src/Mimic.cpp

check_PROGRAMS=mimictest
mimictest_CPPFLAGS= -I$(top_srcdir)/src
mimictest_LDFLAGS= -lpthread
mimictest_LDADD=libmimic.a
mimictest_SOURCES=src/test/MimicTest.cpp \
    src/test/gmock-gtest-all.cc \
    src/test/ClassFile_test.cpp \
    src/test/ClassValidator_test.cpp \
    src/test/FieldDescriptor_test.cpp \
    src/test/JUtf8String_test.cpp \
    src/test/parsing/ByteConsumer_test.cpp

test: check
	$(top_srcdir)/mimictest

