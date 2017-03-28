Please compile Simon with the provided build.sh script.

There are the following dependencies.

Required build-time dependency:
1. KDE libraries and workspace (4.8 or newer)

Optional build-time dependencies:
1. KDE Pim Libs . Enables the optional command plugin "Akonadi" that allows
Simon to trigger commands at certain times and to use Simon dialogs as
calendar reminders.
2. LibSampleRate (http://www.mega-nerd.com/SRC/). If you build Simon with LibSampleRate support, it can
automatically resample audio in- and output for devices that don't support the
required configuration.
3. qaccessibilityclient (http://cmusphinx.sf.net://projects.kde.org/projects/playground/accessibility/libkdeaccessibilityclient). Enables the experimental ATSPI integration.
4. Sphinxbase, PocketSphinx, SphinxTrain (http://cmusphinx.sf.net). Together, they enable SPHINX support in Simon
(model compilation and recognition).
5. OpenCV (http://opencv.org). Enables the face detection and lip detection context plugins.

Optional runtime dependencies:
Julius (http://julius.sourceforge.jp/en_index.php) and the HTK
(http://htk.eng.cam.ac.uk) enable the use of HTK models when installed.


Please refer to the manual for more information.
