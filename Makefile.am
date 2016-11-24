AUTOMAKE_OPTIONS = subdir-objects

noinst_LIBRARIES=libmimic.a
libmimic_a_SOURCES= \
    src/parsing/ByteConsumer.cpp

bin_PROGRAMS=mimic
mimic_LDADD=libmimic.a
mimic_SOURCES=src/Mimic.cpp

check_PROGRAMS=mimictest
mimictest_CPPFLAGS= -I$(top_srcdir)/src/test -I$(top_srcdir)/src
mimictest_LDFLAGS= -lpthread
mimictest_LDADD=libmimic.a
mimictest_SOURCES=src/test/MimicTest.cpp \
    src/test/gmock-gtest-all.cc \
    src/test/parsing/ByteConsumer_test.cpp

test: check
	$(top_srcdir)/mimictest

