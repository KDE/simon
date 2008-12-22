======================================================================
                  Large Vocabulary Continuous Speech
                          Recognition Engine

                                Julius

                                                (Rev 4.1   2008/10/03)
                                                (Rev 4.0.2 2008/05/27)
                                                (Rev 4.0   2007/12/19)
                                                (Rev 3.5.3 2006/12/29)
                                                (Rev 3.4.2 2004/04/30)
                                                (Rev 2.0   1999/02/20)
                                                (Rev 1.0   1998/02/20)

 Copyright (c) 1991-2008 Kawahara Lab., Kyoto University
 Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 Copyright (c) 2005-2008 Julius project team, Nagoya Institute of Technology
 All rights reserved
======================================================================

About Julius
=============

"Julius" is an open-source high-performance large vocabulary
continuous speech recognition (LVCSR) decoder software for
speech-related researchers and developers.  Based on word N-gram and
triphone context-dependent HMM, it can perform almost real-time
decoding on most current PCs with small amount of memory.

It also has high vesatility. The acoustic models and language models
are pluggable, and you can build various types of speech recognition
system by building your own models and modules to be suitable for your
task.  It also adopts standard formats to cope with other toolkit such
as HTK, CMU-Cam SLM toolkit, etc.

The core engine is implemented as embeddable library, to aim to offer
speech recognition capability to various applications.  The recent
version supports plug-in capability so that the engine can be extended
by user.

The main platform is Linux and other Unix workstations, and also works
on Windows (SAPI/console). Julius is distributed with open license
together with source codes.


What's new in Julius-4.1
==========================

From 4.0 to 4.0.2, many bugs are fixed and small improvements were
done.  New options "-fallback1pass" and "-usepower" were added.  The
default audio API is changed from "oss" to "alsa" on Linux.

From 4.0.2 to 4.1, multi-stream AM, MSD-HMM, CVN, frequency warping
for VTLN are all supported.  "jclient-perl", a perl version of module
mode client, is newly added.

Plug-in support is newly implemented on rev. 4.1.  Using dynamic
object linking, Julius now can load extensions at run time, and extend
its ability easily without modifying the original engine source code.
The directory "plugin" contains several example source codes and ready
to compile and test them.  The source codes also contain all function
specification documents.

From this release, you can get the total documentation of Julius
called "Juliusbook".  It is written in Docbook XML, and PDF version
and HTML version is available.  At this time English version has only
the option descriptions and online manuals.  Full version that
explains all aspects and specification of Julius (models, input,
decoding, etc...) will be released within a short term.


Contents of Julius-4.1
========================

	(Documents with suffix "ja" are written in Japanese)

	00readme.txt		ReadMe (This file)
	LICENSE.txt		Terms and conditions of use
	Release.txt		Release note / ChangeLog
	configure		configure script
	configure.in		
	Sample.jconf		Sample configuration file for Julius-3.5.3
	julius/			Julius/Julian 3.5.3 sources
	libsent/		Julius/Julian 3.5.3 library sources
	adinrec/		Record one sentence utterance to a file
	adintool/		Record/split/send/receive speech data
	generate-ngram/		Tool to generate random sentences from N-gram
	gramtools/		Tools to build and test recognition grammar
	jcontrol/		A sample network client module 
	mkbingram/		Convert N-gram to binary format
	mkbinhmm/		Convert ascii hmmdefs to binary format
	mkgshmm/		Model conversion for Gaussian Mixture Selection
	mkss/			Estimate noise spectrum from mic input
	support/		some tools to compile julius/julian from source
	jclient-perl/		A simple perl version of module mode client
	plugin/			Several plugin source codes and documentation
	man/			Unix online manuals
	olddoc/			ChangeLogs before 3.2


Documentation
===============

The up-to-date documentations are available at the Julius Web site:

    http://julius.sourceforge.jp/en/

For QA, discussion and development information, please see and join
the Julius web forum at:

    http://julius.sourceforge.jp/forum/


License
========

Julius is an open-source software provided as is.  For more
information about the license, please refer to the "LICENSE.txt" file
included in this archive.


Contact Us
===========

The contact address of Julius/Julian development team is:
(please replace 'at' with '@')

      "julius-info at lists.sourceforge.jp"


EOF
