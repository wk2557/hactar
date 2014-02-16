AUTOMAKE_OPTIONS=foreign subdir-objects no-define
ACLOCAL_AMFLAGS=-I m4 ${ACLOCAL_FLAGS}

AM_CXXFLAGS=-Wall -fno-exceptions -fno-rtti -I${top_srcdir}/base

lib_LTLIBRARIES=libhactar.la
libhactar_la_SOURCES=base/base_test.cc
libhactar_la_CPPFLAGS= -Wall $(C_FLAGS)
libhactar_la_LFLAGS= -pthread $(L_FLAGS)
libhactar_la_LDFLAGS= -version-info 0:1:0
libhactar_includedir=$(includedir)/hactar
libhactar_include_HEADERS=base/const_ptr.hh base/mutable_ptr.hh base/const_queue.hh base/action.hh base/wrap_action.hh base/offer_action.hh base/loop_action.hh base/hactar.hh

check_PROGRAMS=hactar_test
hactar_test_SOURCES=hactar_test.cc base/base_test.cc
hactar_test_LDADD=-L. -lhactar

TESTS: hactar_test

test: hactar_test
	${top_srcdir}/hactar_test
.PHONY: test

html:
	find ${top_srcdir} -name "*.adoc" -exec asciidoc -a icons {} \;
	find ${top_srcdir} -name "*.h"  -o -name "*.hh" -exec asciidoc -a icons  {} \;
.PHONY: html

index:
	find ${top_srcdir} -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hh" > ${top_srcdir}/index
.PHONY: index

beautify: index
	uncrustify -lCPP -c ${top_srcdir}/uncrustify.cfg --mtime --no-backup `cat ${top_srcdir}/index`
.PHONY: beautify

cscope: index
	cscope -q -R -b -i ${top_srcdir}/index
	export CSCOPE_DB=${top_srcdir}/cscope.out
.PHONY: cscope

cleanup:
	rm -frv ${top_srcdir}/*~ ${top_srcdir}/**/*~ ${top_srcdir}/.DS_Store ${top_srcdir}/**/.DS_Store ${top_srcdir}/Makefile.in ${top_srcdir}/aclocal.m4 ${top_srcdir}/config.* ${top_srcdir}/confdefs.h ${top_srcdir}/configure ${top_srcdir}/install-sh ${top_srcdir}/ltmain.sh ${top_srcdir}/compile ${top_srcdir}/missing ${top_srcdir}/libtool ${top_srcdir}/depcomp ${top_srcdir}/stamp-h1 ${top_srcdir}/m4/ ${top_srcdir}/.libs/ ${top_srcdir}/**/.libs ${top_srcdir}/autom4te.cache/ ${top_srcdir}/**/.dirstamp ${top_srcdir}/.deps ${top_srcdir}/**/.deps ${top_srcdir}/libhactar.la ${top_srcdir}/*.o ${top_srcdir}/**/*.o ${top_srcdir}/**.lo ${top_srcdir}/**/*.lo ${top_srcdir}/hactar_shell ${top_srcdir}/hactar_test  ${top_srcdir}/cscope.* ${top_srcdir}/*.html ${top_srcdir}/**/*.html ${top_srcdir}/index ${top_srcdir}/Makefile
.PHONY: cleanup
