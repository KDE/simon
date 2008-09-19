<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>RtAudio.h</name>
    <path>/home/bedahr/Daten/simon/source/trunk/</path>
    <filename>RtAudio_8h</filename>
    <class kind="class">RtAudio</class>
    <class kind="struct">RtAudio::DeviceInfo</class>
    <class kind="struct">RtAudio::StreamParameters</class>
    <class kind="struct">RtAudio::StreamOptions</class>
    <member kind="typedef">
      <type>unsigned long</type>
      <name>RtAudioFormat</name>
      <anchorfile>RtAudio_8h.html</anchorfile>
      <anchor>afca92882d25915560018873221e44b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>RtAudioStreamFlags</name>
      <anchorfile>RtAudio_8h.html</anchorfile>
      <anchor>2c0f59b200dcec4d4e760f9166b29c41</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>RtAudioStreamStatus</name>
      <anchorfile>RtAudio_8h.html</anchorfile>
      <anchor>80e306d363583da3b0a1b65d9b57c806</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>RtAudioCallback</name>
      <anchorfile>RtAudio_8h.html</anchorfile>
      <anchor>112c7b7e25a974977f6fc094cef1a31f</anchor>
      <arglist>)(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>$CLASSNAME</name>
    <filename>class$CLASSNAME.html</filename>
  </compound>
  <compound kind="class">
    <name>AddWordIntroPage</name>
    <filename>classAddWordIntroPage.html</filename>
    <member kind="function">
      <type></type>
      <name>AddWordIntroPage</name>
      <anchorfile>classAddWordIntroPage.html</anchorfile>
      <anchor>3190e28be08c286e194de8c5159fb508</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AddWordRecordPage</name>
    <filename>classAddWordRecordPage.html</filename>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classAddWordRecordPage.html</anchorfile>
      <anchor>760a1e1aecc7b98319557be140ca45c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AddWordRecordPage</name>
      <anchorfile>classAddWordRecordPage.html</anchorfile>
      <anchor>e1aa711444138a60d51c6eabeccb18f2</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~AddWordRecordPage</name>
      <anchorfile>classAddWordRecordPage.html</anchorfile>
      <anchor>a5497b97dfe8b30b2f8bd882d18f2c99</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AddWordResolvePage</name>
    <filename>classAddWordResolvePage.html</filename>
    <member kind="function">
      <type></type>
      <name>AddWordResolvePage</name>
      <anchorfile>classAddWordResolvePage.html</anchorfile>
      <anchor>432c74d465e09ededf7f84e340705b69</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classAddWordResolvePage.html</anchorfile>
      <anchor>4290a991ad97cd1b68d88569cbf8c985</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>displayWords</name>
      <anchorfile>classAddWordResolvePage.html</anchorfile>
      <anchor>efeee80550a3b0fa2ad12b6e366472ab</anchor>
      <arglist>(WordList *words)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AddWordView</name>
    <filename>classAddWordView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>5833706eea47be559a736e375823a74c</anchor>
      <arglist>(int done)</arglist>
    </member>
    <member kind="slot">
      <type>AddWordIntroPage *</type>
      <name>createWelcomePage</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>625f708863622619c4c9ee540ec6763e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>QWizardPage *</type>
      <name>createRecordPage</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>0cd99dc736772191982e4bd66b20b934</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>AddWordResolvePage *</type>
      <name>createResolvePage</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>56b63706c637c0c8335973d5e8b9b8a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>QWizardPage *</type>
      <name>createFinishedPage</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>64386d190454b2813b6dd65f02441d75</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>createWord</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>09ec0394654c72382b26a2a40d62ceed</anchor>
      <arglist>(QString word)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>AddWordView</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>4d4083b44e6a27bc5b589d4af072e313</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ATBackend</name>
    <filename>classATBackend.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>objectFound</name>
      <anchorfile>classATBackend.html</anchorfile>
      <anchor>6385b6c9871d6e003052f7c031a693be</anchor>
      <arglist>(ATObject *widgets)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ATObject</name>
    <filename>classATObject.html</filename>
  </compound>
  <compound kind="class">
    <name>ATSettings</name>
    <filename>classATSettings.html</filename>
    <base>SystemWidget</base>
  </compound>
  <compound kind="class">
    <name>ATWatcher</name>
    <filename>classATWatcher.html</filename>
    <member kind="slot">
      <type>bool</type>
      <name>trigger</name>
      <anchorfile>classATWatcher.html</anchorfile>
      <anchor>7b241330332469254637023e70918dc5</anchor>
      <arglist>(QString word)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applySettings</name>
      <anchorfile>classATWatcher.html</anchorfile>
      <anchor>7c56c330c36752d2a9d27f4b3010e19f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>BOMPDict</name>
    <filename>classBOMPDict.html</filename>
    <base>Dict</base>
    <member kind="function">
      <type></type>
      <name>BOMPDict</name>
      <anchorfile>classBOMPDict.html</anchorfile>
      <anchor>c9cf34ac89e684316101fb4b44af7fbb</anchor>
      <arglist>(QString path=&quot;&quot;, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classBOMPDict.html</anchorfile>
      <anchor>3008e0044b4fac37e1df42a13046ed3e</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Bunzip</name>
    <filename>classBunzip.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>cancel</name>
      <anchorfile>classBunzip.html</anchorfile>
      <anchor>c0e6f112d9ccc9399bd813cb1f2517c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classBunzip.html</anchorfile>
      <anchor>220489c327fe0c3dba31f630a9fb6fa0</anchor>
      <arglist>(QString filename)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>extractingFinishing</name>
      <anchorfile>classBunzip.html</anchorfile>
      <anchor>436041316328df8a1594afa66ae95e3c</anchor>
      <arglist>(int exitCode)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bunzip</name>
      <anchorfile>classBunzip.html</anchorfile>
      <anchor>b5138b4ceea44f6de0c8741e3548bed8</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CategoryXMLReader</name>
    <filename>classCategoryXMLReader.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>CategoryXMLReader</name>
      <anchorfile>classCategoryXMLReader.html</anchorfile>
      <anchor>f7e9335ed571250b187691c6d7b2cf54</anchor>
      <arglist>(QString path, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classCategoryXMLReader.html</anchorfile>
      <anchor>6107501cef6c6b11128d46c6810e1e15</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Command</name>
    <filename>classCommand.html</filename>
    <member kind="function">
      <type></type>
      <name>Command</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>4b6075a30bc4903a14b96e87ef6b10bc</anchor>
      <arglist>(QString name, CommandType type, QString value, QString iconPath=&quot;&quot;, QString workingDirectory=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getName</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>571fdd54480d238527d7d933f5f90c86</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>CommandType</type>
      <name>getType</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>88fd4effe76229b40f3021e77fcae86e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getValue</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>d0a1262d53934bf661805596575e423a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getWorkingDirectory</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>48bf11e371ed3b1a60cad032a42f8b3d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getIconPath</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>10420593314681f56caeca771e023d02</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CommandSettings</name>
    <filename>classCommandSettings.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>insertCommand</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>1d6c55a77d192d3d4aa6ec90aac5b802</anchor>
      <arglist>(Command *command)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setWidgetsDisabled</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>ab5e957a04f04e86fc836ec7c1599294</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>checkValuesAfterReturnPressed</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>5cbda30a4692ad5140da1416304a3077</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CommandSettings</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>76c4a1e6bd6753d043e7fa11a719bbf8</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~CommandSettings</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>7fc4e7a495f4aff326320da92fd129c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>bf3b4356ff26bd11cd1e25ff23a5c735</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>apply</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>4428f9209043fbcb2ada649f62038060</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>reset</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>d2a45376d9435269f4187f458d3d80f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>ee42a477ed3c4be503f0ac671cc5ab4d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deactivateAllCbs</name>
      <anchorfile>classCommandSettings.html</anchorfile>
      <anchor>66a0ae0a0d3527f7da51085645834a4f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ConfigureProgramPage</name>
    <filename>classConfigureProgramPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ConfigureProgramPage</name>
      <anchorfile>classConfigureProgramPage.html</anchorfile>
      <anchor>43375b7ca7df2a252ec3ed0b00d8a5ca</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ConfigureProgramPage</name>
      <anchorfile>classConfigureProgramPage.html</anchorfile>
      <anchor>d28e1bbe9ff472fe3eb93a468198421b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const QString</type>
      <name>getIcon</name>
      <anchorfile>classConfigureProgramPage.html</anchorfile>
      <anchor>3fa28283aa9e80d5290078669b67338a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init</name>
      <anchorfile>classConfigureProgramPage.html</anchorfile>
      <anchor>a698e1a473070f14b2e1d94571c7f536</anchor>
      <arglist>(QString iconsrc, QString name, QString exec, QString workingDirectory)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ConfigureTriggersPage</name>
    <filename>classConfigureTriggersPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ConfigureTriggersPage</name>
      <anchorfile>classConfigureTriggersPage.html</anchorfile>
      <anchor>334efc95fd180e5913ee3b37e3a6f1eb</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classConfigureTriggersPage.html</anchorfile>
      <anchor>e8b3097b3314d5ff1761f59c5d9bf12a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classConfigureTriggersPage.html</anchorfile>
      <anchor>39c2a61a08a6fdfb4310782f909a4be6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validatePage</name>
      <anchorfile>classConfigureTriggersPage.html</anchorfile>
      <anchor>cc9839584bbbf210dcd675fcd214eadc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CoreEvents</name>
    <filename>classCoreEvents.html</filename>
  </compound>
  <compound kind="class">
    <name>DBusBackend</name>
    <filename>classDBusBackend.html</filename>
    <base>ATBackend</base>
    <member kind="function">
      <type>ATObject *</type>
      <name>readGui</name>
      <anchorfile>classDBusBackend.html</anchorfile>
      <anchor>d74e5c0b80ac99b061d6b9d3ce9842d3</anchor>
      <arglist>(QString service)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DefineDefaultValuesPage</name>
    <filename>classDefineDefaultValuesPage.html</filename>
    <member kind="function">
      <type></type>
      <name>DefineDefaultValuesPage</name>
      <anchorfile>classDefineDefaultValuesPage.html</anchorfile>
      <anchor>d46bca97ab0eff0d9297528f3175ffe7</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkAndSet</name>
      <anchorfile>classDefineDefaultValuesPage.html</anchorfile>
      <anchor>6a2ef04642b8fe898efac4ff0fe8b3c5</anchor>
      <arglist>(QString option, QVariant value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classDefineDefaultValuesPage.html</anchorfile>
      <anchor>ec5f5dc638a98765467b5fea71c6fa07</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classDefineDefaultValuesPage.html</anchorfile>
      <anchor>2369ed35b7fb9e04afd54fdddf6ec75a</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DeleteWordDialog</name>
    <filename>classDeleteWordDialog.html</filename>
    <member kind="function">
      <type></type>
      <name>DeleteWordDialog</name>
      <anchorfile>classDeleteWordDialog.html</anchorfile>
      <anchor>8334df64fbe199337fc3b31873915a43</anchor>
      <arglist>(QWidget *parent=0, Qt::WindowFlags f=0)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>exec</name>
      <anchorfile>classDeleteWordDialog.html</anchorfile>
      <anchor>38d352bd9a53d96396dcab9a3957bf89</anchor>
      <arglist>(Word word, bool isShadowed)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DesktopGridConfiguration</name>
    <filename>classDesktopGridConfiguration.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>bool</type>
      <name>apply</name>
      <anchorfile>classDesktopGridConfiguration.html</anchorfile>
      <anchor>bd300f263649a2503c95fd1569a53273</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>reset</name>
      <anchorfile>classDesktopGridConfiguration.html</anchorfile>
      <anchor>c473c3ed400c1579f9654a05ae40b804</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classDesktopGridConfiguration.html</anchorfile>
      <anchor>3aa474816fb4f913e5b87ee58b45b733</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DesktopGridConfiguration</name>
      <anchorfile>classDesktopGridConfiguration.html</anchorfile>
      <anchor>7cc7cbec44daa5bbfaba29bcdeaacff2</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classDesktopGridConfiguration.html</anchorfile>
      <anchor>83738baa44763f6a1d0f63f91b081abc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Dict</name>
    <filename>classDict.html</filename>
    <member kind="function">
      <type></type>
      <name>Dict</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>e3a54e32280a37b5152c7728795c078b</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>ipaToXSampa</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>576fb771620a5e0f87dc9c6ec82af654</anchor>
      <arglist>(QString ipa)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Dict</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>46561fe694c0bba6e7ed3703c47bf43f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DragTableWidget</name>
    <filename>classDragTableWidget.html</filename>
    <member kind="function">
      <type></type>
      <name>DragTableWidget</name>
      <anchorfile>classDragTableWidget.html</anchorfile>
      <anchor>bb52309070397e5fa99f971dde2f9de4</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~DragTableWidget</name>
      <anchorfile>classDragTableWidget.html</anchorfile>
      <anchor>3ca107ddfe5bd47cdad1481dc26145bf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>mousePressEvent</name>
      <anchorfile>classDragTableWidget.html</anchorfile>
      <anchor>8c3a89da8a434a608255bfcf262612ff</anchor>
      <arglist>(QMouseEvent *event)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>mouseMoveEvent</name>
      <anchorfile>classDragTableWidget.html</anchorfile>
      <anchor>b1bbe500db4be90a9e085cda272bbe9b</anchor>
      <arglist>(QMouseEvent *event)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>startDrag</name>
      <anchorfile>classDragTableWidget.html</anchorfile>
      <anchor>05f3d9eff2af72082e06921399688917</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DropListWidget</name>
    <filename>classDropListWidget.html</filename>
    <member kind="function">
      <type></type>
      <name>DropListWidget</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>7ce3159db5c8eb5086181c4c0c728c6b</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~DropListWidget</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>747b394b3198a807c117490687febd9b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>dragEnterEvent</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>e2722c0fb4e3307eaaa35c046d537b30</anchor>
      <arglist>(QDragEnterEvent *event)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>dropEvent</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>7f1814f1971d18d330ec95f08129765a</anchor>
      <arglist>(QDropEvent *event)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>dragMoveEvent</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>14a8faa30126eda8b307845b6257fd15</anchor>
      <arglist>(QDragMoveEvent *event)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>EventHandler</name>
    <filename>classEventHandler.html</filename>
    <member kind="function">
      <type>void</type>
      <name>click</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>99714cb7f2c499aefdb08a21b88772cf</anchor>
      <arglist>(int x, int y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendShortcut</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>7ae6561cacbd3d58e810c2925bc0bd23</anchor>
      <arglist>(Shortcut *shortcut)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendWord</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>f660a2c5d8f972fbaa0d2e12dd4c6df9</anchor>
      <arglist>(QString word)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendKey</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>2bb707650595308c9580b490ce5a8a68</anchor>
      <arglist>(QChar key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>EventHandler</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>8fe27b69582cce5c6a89a0b134bc8158</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ExternalProgramManager</name>
    <filename>classExternalProgramManager.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>bool</type>
      <name>apply</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>6d063196b3fc333fb550244fafa71c33</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>reset</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>292e1967b95183122d770cc7c3ad4def</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>78964ea5df359171b5600fe96835520c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ExternalProgramManager</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>130209ddef871bac532799fed8614a80</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>2935f3b5b1a4689b4edfaae5ed62190f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ExternalProgramManager</name>
      <anchorfile>classExternalProgramManager.html</anchorfile>
      <anchor>3aa85f488fbd147093979dc03470d2d9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunCreateDictionaryPage</name>
    <filename>classFirstRunCreateDictionaryPage.html</filename>
    <member kind="function">
      <type></type>
      <name>FirstRunCreateDictionaryPage</name>
      <anchorfile>classFirstRunCreateDictionaryPage.html</anchorfile>
      <anchor>e00d952217f9e8b90709b906a63076ee</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classFirstRunCreateDictionaryPage.html</anchorfile>
      <anchor>d04cd094ee7a3eb56a21a759b8807ea5</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunImportBOMPPage</name>
    <filename>classFirstRunImportBOMPPage.html</filename>
    <base>ImportBOMPPage</base>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classFirstRunImportBOMPPage.html</anchorfile>
      <anchor>3664975a2c62e5f59fe18859681846b8</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunImportDictSelectSourcePage</name>
    <filename>classFirstRunImportDictSelectSourcePage.html</filename>
    <base>ImportDictSelectSourcePage</base>
    <member kind="function">
      <type></type>
      <name>FirstRunImportDictSelectSourcePage</name>
      <anchorfile>classFirstRunImportDictSelectSourcePage.html</anchorfile>
      <anchor>ea50bc665b0c2597b21f680e2d7190fa</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classFirstRunImportDictSelectSourcePage.html</anchorfile>
      <anchor>03444c12af8ed2cdb0d4a2362a0ed78f</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunImportDictWiktionaryPage</name>
    <filename>classFirstRunImportDictWiktionaryPage.html</filename>
    <base>ImportDictWiktionaryPage</base>
    <member kind="function">
      <type></type>
      <name>FirstRunImportDictWiktionaryPage</name>
      <anchorfile>classFirstRunImportDictWiktionaryPage.html</anchorfile>
      <anchor>686cbb7f98ed8444cb52ab1a7c5ba09e</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classFirstRunImportDictWiktionaryPage.html</anchorfile>
      <anchor>6b1a51123ef6aca6ad166da9275ede5a</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunImportGrammarDescriptionPage</name>
    <filename>classFirstRunImportGrammarDescriptionPage.html</filename>
  </compound>
  <compound kind="class">
    <name>FirstRunImportGrammarWorkingPage</name>
    <filename>classFirstRunImportGrammarWorkingPage.html</filename>
    <member kind="function">
      <type></type>
      <name>FirstRunImportGrammarWorkingPage</name>
      <anchorfile>classFirstRunImportGrammarWorkingPage.html</anchorfile>
      <anchor>78766831d745a7dd5e3a2dcecc9f847a</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunModelSettings</name>
    <filename>classFirstRunModelSettings.html</filename>
    <member kind="function">
      <type></type>
      <name>FirstRunModelSettings</name>
      <anchorfile>classFirstRunModelSettings.html</anchorfile>
      <anchor>3f1466ae4a52e96ae91bbb330620b7f6</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classFirstRunModelSettings.html</anchorfile>
      <anchor>ac21b6bcbbcb62ed1d976c24dcca4aab</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FirstRunWizard</name>
    <filename>classFirstRunWizard.html</filename>
    <member kind="function">
      <type></type>
      <name>FirstRunWizard</name>
      <anchorfile>classFirstRunWizard.html</anchorfile>
      <anchor>3ba0550d90dad2b8b530cf2f0fa1067c</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FirstRunWizard</name>
      <anchorfile>classFirstRunWizard.html</anchorfile>
      <anchor>d8584de6c47b626fdd4d5c68cbc43e96</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GeneralSettings</name>
    <filename>classGeneralSettings.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>bool</type>
      <name>apply</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>83f2d16fb1ecf95e82cf53600842282f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>c204d7c1251d1e7532581c535e18fe8e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>reset</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>643215e2b4ef6efbbf1b1f52c9357457</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GeneralSettings</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>1ae4e989c8d256df4a3f29e733735721</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>8c83ea2193967ad6d454d483c769c70f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~GeneralSettings</name>
      <anchorfile>classGeneralSettings.html</anchorfile>
      <anchor>5de8d95efec990cc8c6dfa6bbaca19d5</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>IconButton</name>
    <filename>classIconButton.html</filename>
    <member kind="function">
      <type></type>
      <name>IconButton</name>
      <anchorfile>classIconButton.html</anchorfile>
      <anchor>8edc1fd1f74f0d0e92d8f687e4284a37</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>IconDialog</name>
    <filename>classIconDialog.html</filename>
    <member kind="function">
      <type></type>
      <name>IconDialog</name>
      <anchorfile>classIconDialog.html</anchorfile>
      <anchor>528181f7bc629bca9408c74ea6d7f487</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showIcons</name>
      <anchorfile>classIconDialog.html</anchorfile>
      <anchor>e7332d6adfcb85743cb34a6bb15aefc3</anchor>
      <arglist>(QString currentIcon=&quot;&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportBOMPPage</name>
    <filename>classImportBOMPPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportBOMPPage</name>
      <anchorfile>classImportBOMPPage.html</anchorfile>
      <anchor>9a6b7482df49af2e1dda5a4981b90f56</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportBOMPPage</name>
      <anchorfile>classImportBOMPPage.html</anchorfile>
      <anchor>89f991dd2b7d444fff6bf298c8531d92</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDict</name>
    <filename>classImportDict.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>deleteDict</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>db784b7c3f861b074d7729f79a824014</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportDict</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>7a6605ddcc915672aab493536954ab7d</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>6427916caeed58936c5f7484e67fe1e4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>parseWordList</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>03086eeebfa41bd588d0723734c7e372</anchor>
      <arglist>(QString pathToDict, int type, bool deleteFileWhenDone=false)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportDict</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>fd99353fa09bcdb4c56cc43393e0af34</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDictSelectSourcePage</name>
    <filename>classImportDictSelectSourcePage.html</filename>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classImportDictSelectSourcePage.html</anchorfile>
      <anchor>6b95e249cf9f23a7f0a45fbf1535d551</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportDictSelectSourcePage</name>
      <anchorfile>classImportDictSelectSourcePage.html</anchorfile>
      <anchor>4f3da13000a04180f13b8acfa678809a</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportDictSelectSourcePage</name>
      <anchorfile>classImportDictSelectSourcePage.html</anchorfile>
      <anchor>2bcc4db4e7395ab2cf2d3fdf5830d28a</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDictView</name>
    <filename>classImportDictView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>show</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>666ed5d3d362b52f93da66c7d3a349e8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>QWizardPage *</type>
      <name>createIntroPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>d62bd698aa96395637002d33b0a4544b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>ImportDictSelectSourcePage *</type>
      <name>createSelectSourcePage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>873293edc0dc88caff3eeac07c679fc7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>ImportBOMPPage *</type>
      <name>createImportBOMPPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>b590575e965f64c4d2911503e93f5f45</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>ImportDictWiktionaryPage *</type>
      <name>createImportWiktionaryPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>ae0a95d900540b7349b83161d1b0b4cd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>QWizardPage *</type>
      <name>createImportLexiconPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>a717a04a683804fe0e8a6ee856cb3f12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>ImportDictWorkingPage *</type>
      <name>createImportDictWorkingPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>f87de225341f61facab24a95c9ef4d57</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>QWizardPage *</type>
      <name>createFinishedPage</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>5d0f8de8d9be2cdd63af1accf2a1496d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportDictView</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>7fc799478e8fc1e41cee3eb94d781075</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportDictView</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>7a40679efb522252e8f947bfccbf4294</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDictWiktionaryPage</name>
    <filename>classImportDictWiktionaryPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportDictWiktionaryPage</name>
      <anchorfile>classImportDictWiktionaryPage.html</anchorfile>
      <anchor>71cdcc7ad921b447c71c4d5b68121ca7</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classImportDictWiktionaryPage.html</anchorfile>
      <anchor>b0e0a83b224f0382d736b60f2fe338fc</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportDictWiktionaryPage</name>
      <anchorfile>classImportDictWiktionaryPage.html</anchorfile>
      <anchor>f1e16a4c8f8cbe7f1a8c7c89d73a16d8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDictWorkingPage</name>
    <filename>classImportDictWorkingPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>abort</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>b69462487ef3a50c5d9fe8506b678d4b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayStatus</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>b197f84a18ef74f2f986c33303af8d0d</anchor>
      <arglist>(QString status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayProgress</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>b9c08d384d5eaecd1aed5addb40ae077</anchor>
      <arglist>(int progress)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importHADIFIX</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>5850f222c2c5ecd3834ed76d021e8bde</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importWiktionary</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>4cc83210031dfe9eef545b2c2d28a069</anchor>
      <arglist>(QString url)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importLexicon</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>1a4eec272b72b16c6de2ee5aa7bd14ff</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>unpackWikiIfNecessary</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>84cbf3631efd2e161fb45d23d9d3e50e</anchor>
      <arglist>(QString file)</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>a75b97f95fc963959b5d5c26701ed726</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>2134ce4bbb1f9e381d646ae7dc671428</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportDictWorkingPage</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>25d7a2db8b06de1ee4cbe207bcd5f099</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportDictWorkingPage</name>
      <anchorfile>classImportDictWorkingPage.html</anchorfile>
      <anchor>42c7fd1e94632520be6a06da7e31d141</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportGrammar</name>
    <filename>classImportGrammar.html</filename>
  </compound>
  <compound kind="class">
    <name>ImportGrammarSelectFilesPage</name>
    <filename>classImportGrammarSelectFilesPage.html</filename>
  </compound>
  <compound kind="class">
    <name>ImportGrammarWizard</name>
    <filename>classImportGrammarWizard.html</filename>
  </compound>
  <compound kind="class">
    <name>ImportLexiconPage</name>
    <filename>classImportLexiconPage.html</filename>
  </compound>
  <compound kind="class">
    <name>ImportPlacePage</name>
    <filename>classImportPlacePage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportPlacePage</name>
      <anchorfile>classImportPlacePage.html</anchorfile>
      <anchor>c9af6befa797591fedcafd6dffeb05ef</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportPlacePage</name>
      <anchorfile>classImportPlacePage.html</anchorfile>
      <anchor>6c31c92a2b3815ac3659553a5f3236f8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classImportPlacePage.html</anchorfile>
      <anchor>91936d1e0a59f129f7d7fc58ccb5624e</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>createCommand</name>
      <anchorfile>classImportPlacePage.html</anchorfile>
      <anchor>2b249e366319894e3cd26bc21725fd82</anchor>
      <arglist>(QString name, QString value)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportPlaceWizard</name>
    <filename>classImportPlaceWizard.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportPlaceWizard</name>
      <anchorfile>classImportPlaceWizard.html</anchorfile>
      <anchor>839df7469e80522aa895660b4ae5c90e</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportPlaceWizard</name>
      <anchorfile>classImportPlaceWizard.html</anchorfile>
      <anchor>336840dff95d65a0c951b9d065241d2e</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportProgramPage</name>
    <filename>classImportProgramPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportProgramPage</name>
      <anchorfile>classImportProgramPage.html</anchorfile>
      <anchor>17ba36721de416c40e97a81c94d0b9e5</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportProgramPage</name>
      <anchorfile>classImportProgramPage.html</anchorfile>
      <anchor>1fda02fa7427dc9bb1e70175a1745e13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>createCommand</name>
      <anchorfile>classImportProgramPage.html</anchorfile>
      <anchor>fa9b68ab8c10696c7f46a968bc2cf1ab</anchor>
      <arglist>(QString icon, QString name, QString value, QString workingDir)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportProgramWizard</name>
    <filename>classImportProgramWizard.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportProgramWizard</name>
      <anchorfile>classImportProgramWizard.html</anchorfile>
      <anchor>a6163cd1bf754274d557b7494742a00b</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportProgramWizard</name>
      <anchorfile>classImportProgramWizard.html</anchorfile>
      <anchor>cfa0424f5e1b95b3337e3395625ae1f8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingDirectory</name>
    <filename>classImportTrainingDirectory.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportTrainingDirectory</name>
      <anchorfile>classImportTrainingDirectory.html</anchorfile>
      <anchor>e0a329ab84ed2dd00c0565b08de5d69c</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingDirectoryIntroPage</name>
    <filename>classImportTrainingDirectoryIntroPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setDir</name>
      <anchorfile>classImportTrainingDirectoryIntroPage.html</anchorfile>
      <anchor>c25a95415d1ac0d684653a825955e513</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportTrainingDirectoryIntroPage</name>
      <anchorfile>classImportTrainingDirectoryIntroPage.html</anchorfile>
      <anchor>ca485d23357ace67652c1b4c0316ef08</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingDirectoryWorkingPage</name>
    <filename>classImportTrainingDirectoryWorkingPage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportTrainingDirectoryWorkingPage</name>
      <anchorfile>classImportTrainingDirectoryWorkingPage.html</anchorfile>
      <anchor>6c8b3da604221b28c1545313ef38cbbb</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classImportTrainingDirectoryWorkingPage.html</anchorfile>
      <anchor>17c926b62557dc268d56e55ea4f1376d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTextLocalPage</name>
    <filename>classImportTrainingTextLocalPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setLocalSourceFile</name>
      <anchorfile>classImportTrainingTextLocalPage.html</anchorfile>
      <anchor>962c96ac087c6a36eec02b476149d880</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportTrainingTextLocalPage</name>
      <anchorfile>classImportTrainingTextLocalPage.html</anchorfile>
      <anchor>0a503215ef82aefd9191e8dcd83f7c3d</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTextRemotePage</name>
    <filename>classImportTrainingTextRemotePage.html</filename>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classImportTrainingTextRemotePage.html</anchorfile>
      <anchor>b78d046f7dbfe762eff8c3604d424070</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportTrainingTextRemotePage</name>
      <anchorfile>classImportTrainingTextRemotePage.html</anchorfile>
      <anchor>3c06c7aaf6b17f47f23c343c718be16b</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTexts</name>
    <filename>classImportTrainingTexts.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportTrainingTexts</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>3403ae812360b0c8a3036206fec5e2d4</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>9be6176ef8495d656dc59b931eda62ea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createIntroPage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>2fae49d9f9bee7ef4e25b55d0e13d24a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createSourcePage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>ffe330d1019d3f5c31d9f635adf14cd3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createRemoteImportPage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>dc747b57ccd245a735b0e578ab0c52f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createLocalImportPage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>b7985080000ee8c833fe0940f09f3b03</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createWorkingPage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>c5d28c62a4d4941c8e8df3bc2522a179</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QWizardPage *</type>
      <name>createFinishedPage</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>ae0ea91baf751a517b7d207b33f68f27</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ImportTrainingTexts</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>4abd237cd0af6fff1803da20023815b2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTextSelectSourcePage</name>
    <filename>classImportTrainingTextSelectSourcePage.html</filename>
    <member kind="function">
      <type></type>
      <name>ImportTrainingTextSelectSourcePage</name>
      <anchorfile>classImportTrainingTextSelectSourcePage.html</anchorfile>
      <anchor>6b05fa80bb77b03887d20c82b7d2566c</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classImportTrainingTextSelectSourcePage.html</anchorfile>
      <anchor>49753a9c6f7fe8f5caa51f07f0faeb29</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTextWorkingPage</name>
    <filename>classImportTrainingTextWorkingPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>startImport</name>
      <anchorfile>classImportTrainingTextWorkingPage.html</anchorfile>
      <anchor>b94e5b6b82189194b1c2f8736e0ecb85</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>processText</name>
      <anchorfile>classImportTrainingTextWorkingPage.html</anchorfile>
      <anchor>c106db3a56bbc9bb5e9ae760bf6f9413</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>parseFile</name>
      <anchorfile>classImportTrainingTextWorkingPage.html</anchorfile>
      <anchor>577f50b0c7524aa921632d9ea5b6f43f</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportTrainingTextWorkingPage</name>
      <anchorfile>classImportTrainingTextWorkingPage.html</anchorfile>
      <anchor>c0c823ce4655ca0a273ee8d228d55f54</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>IniReader</name>
    <filename>classIniReader.html</filename>
  </compound>
  <compound kind="class">
    <name>InlineWidget</name>
    <filename>classInlineWidget.html</filename>
    <base>SimonWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>accept</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>9b9e4578b8c8d3e38d416461b1fc7b0c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>reject</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>dc8d260438226c59f1b439ce9c65289b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>close</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>959c1d289f4faa9bc5bca55403c55970</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>exec</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>5a14f7eaa43a2edf5ce33493966d93aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>InlineWidget</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>14a13151a454840f88d0f2a1fd5b7ce4</anchor>
      <arglist>(QString title, QIcon icon, QString desc, QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~InlineWidget</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>37f7f9ede99d538fa40855157e7163d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setVisible</name>
      <anchorfile>classInlineWidget.html</anchorfile>
      <anchor>e5835abfe371f5aae07dbbc2173759e7</anchor>
      <arglist>(bool visible)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InlineWidgetView</name>
    <filename>classInlineWidgetView.html</filename>
    <member kind="function">
      <type></type>
      <name>InlineWidgetView</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>0cd649946e46fb53fb50f8329fec1693</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerPage</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>5950a1666002adfb2ad52a3ccf926e41</anchor>
      <arglist>(InlineWidget *page)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyPressEvent</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>53890a2351c813861ff9c8c516915acb</anchor>
      <arglist>(QKeyEvent *event)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unRegisterPage</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>f2341f440ad85446602bae13cf571b01</anchor>
      <arglist>(InlineWidget *page)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unRegisterCurrentPage</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>6073cf4b74e9c512aeeffbbd4cbe485b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~InlineWidgetView</name>
      <anchorfile>classInlineWidgetView.html</anchorfile>
      <anchor>3ccf0ba65aea75b94c17463a5cc24a60</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InternetExtensionSettings</name>
    <filename>classInternetExtensionSettings.html</filename>
    <base>SystemWidget</base>
  </compound>
  <compound kind="class">
    <name>IntroPlacePage</name>
    <filename>classIntroPlacePage.html</filename>
    <member kind="function">
      <type></type>
      <name>IntroPlacePage</name>
      <anchorfile>classIntroPlacePage.html</anchorfile>
      <anchor>0d006f5aad5afa0af3670096d2a25cea</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~IntroPlacePage</name>
      <anchorfile>classIntroPlacePage.html</anchorfile>
      <anchor>d0bd4bf5861159982ed4787881166e7d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>JuliusControl</name>
    <filename>classJuliusControl.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>messageReceived</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>c50fb2294bba35575f34c3f96b45039d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connectTo</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>a4872155aecef219071f13d36a18ba5b</anchor>
      <arglist>(QString server=&quot;127.0.0.1&quot;, quint16 port=4444)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connectionLost</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>f2b6e8d023b35ecd976944d90141c873</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connectedTo</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>ed2664e63cc4750df35181d0556e5693</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>7730040fb8ad3d8feb7d808e5a614489</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disconnectFromServer</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>17fd87b5988fc00cb35d547d1b555be6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>JuliusControl</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>f3b62c3a2fb6d599dc40832569d0b0a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~JuliusControl</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>26596ce674dc5c430de5920b7c72ed76</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>KDEProgramManager</name>
    <filename>classKDEProgramManager.html</filename>
    <base>ProgramManager</base>
  </compound>
  <compound kind="class">
    <name>LexiconDict</name>
    <filename>classLexiconDict.html</filename>
    <base>Dict</base>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classLexiconDict.html</anchorfile>
      <anchor>a8f84cd2803cd06695f0806e96bc719f</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LocalPlacePage</name>
    <filename>classLocalPlacePage.html</filename>
    <member kind="function">
      <type></type>
      <name>LocalPlacePage</name>
      <anchorfile>classLocalPlacePage.html</anchorfile>
      <anchor>372e08f56ff32d4218072db25241e205</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~LocalPlacePage</name>
      <anchorfile>classLocalPlacePage.html</anchorfile>
      <anchor>06424c23de66d221e740071e46d14fd6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPlacePath</name>
      <anchorfile>classLocalPlacePage.html</anchorfile>
      <anchor>2c8985179ee903f934e9640e6acfe8ed</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LogEntry</name>
    <filename>classLogEntry.html</filename>
  </compound>
  <compound kind="class">
    <name>Logger</name>
    <filename>classLogger.html</filename>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>init</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a0fc99c33212ae9fab0027989b6b4c99</anchor>
      <arglist>(QString path=&quot;log/simon.log&quot;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>f0f77ab835abcd9b68c6877c5d01ba90</anchor>
      <arglist>(QString message)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>close</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>fee2bab560c2db0190c980884d33868c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LogManager</name>
    <filename>classLogManager.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>getDateList</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>388e2b3b8a493c2d04215e5aead96dc0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>98812d3a4a1c68681c004e26f02dd813</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>getDay</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>01b20173707f83083d5945511155aeda</anchor>
      <arglist>(QDate day=QDate())</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>getAll</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>cf3a7acedd78f87f0952e8c353a9a426</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LogManager</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>edab1387b9084cd125446f3b8d82861f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~LogManager</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>7e6291dd344ad467c049c39f65f6897c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasFinishedReading</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>9baeff252a2ed7c7f675ada1655bb9c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classLogManager.html</anchorfile>
      <anchor>4fd6e566ce63b6505ae7a13e606322f0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LogView</name>
    <filename>classLogView.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>9ef31c304dab37860d69779b427102c6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>logReadFinished</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>d98f161482800126c0e4721dd4be4af8</anchor>
      <arglist>(int exitCode)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>readLog</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>022059cd48d550ff0b7e6b31ce2fee25</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>startLogLoad</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>36dfb616205effb9ecdab3bd80bbf867</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>viewAll</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>30194f3b3c8360cb3e24727f4397f1df</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>viewDay</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>144b5bc76b86ebc45c2e25db04641107</anchor>
      <arglist>(QDate day)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>abort</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>992b1553eef17f99b325c9fdf0d37caa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>reload</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>1114b6a0a16723493063d1c64d6f07c8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayCancel</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>d217bc635ffcce6dccfa62419ac2bb45</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayReload</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>c430aca407f0eaea8c678f2fb6586e1f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>enableWidgets</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>7676a88544f36477dc32ce5d19c4ad11</anchor>
      <arglist>(bool enabled=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setStatus</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>f448eee9ea86e08a119b386d140ce6ce</anchor>
      <arglist>(QString status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setProgress</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>1636c66ac7da36717ab2a69296a57731</anchor>
      <arglist>(int progress)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>selectDay</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>9c200c540a00cbde3f357f632ed94448</anchor>
      <arglist>(QDate day)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayDay</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>6d8a60e1c3687dd1702ad165451a2be8</anchor>
      <arglist>(QDate day)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayDay</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>106d9421574afa0b29da8987d276c9d4</anchor>
      <arglist>(QTreeWidgetItem *item)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayDates</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>8ae32ffc7ae1ad025e62d4af6004af8d</anchor>
      <arglist>(Dates daysAvailable)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>completeDates</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>dfa1aacfcf5baf5c66aeec9bfe70b93a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>processEntries</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>ad3e0442c504c0a108682ca04ca10768</anchor>
      <arglist>(LogEntryList *entries, bool copy=false)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>processDates</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>fbb2feea56ecf62155b1a7e54f3c80e7</anchor>
      <arglist>(Dates daysAvailable)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>insertEntries</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>ade89b0577561d9ec658abb885974fdf</anchor>
      <arglist>(LogEntryList *entries)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>clean</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>e7d465b51e91712acfeaec3e5be857c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>cleanAllChilds</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>7f53159f396f9b3043c2b2464cd1e496</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteItem</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>6070df70705aa849a1c450706863463f</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteItem</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>2ad7a14ab77bf433e6e5410448ea6956</anchor>
      <arglist>(QTreeWidgetItem *item)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteChilds</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>0cc734026e625d9979f67a72e630fd68</anchor>
      <arglist>(QTreeWidgetItem *item)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteChilds</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>902e27f24c6779dd0579515be0141803</anchor>
      <arglist>(int parentIndex)</arglist>
    </member>
    <member kind="slot">
      <type>LogEntryList *</type>
      <name>filterFor</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>d926df4104d381210614f760ff5429db</anchor>
      <arglist>(LogEntryList *log, bool copy, const QString filter, const short type)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LogView</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>806a7e4ca3f038286152869b018b924a</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classLogView.html</anchorfile>
      <anchor>0c7a5fcb5215221224eaeb4ff80ce64a</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MergeTerminalsSelectTerminalsPage</name>
    <filename>classMergeTerminalsSelectTerminalsPage.html</filename>
  </compound>
  <compound kind="class">
    <name>ModelManager</name>
    <filename>classModelManager.html</filename>
  </compound>
  <compound kind="class">
    <name>ModelSettings</name>
    <filename>classModelSettings.html</filename>
    <base>SystemWidget</base>
  </compound>
  <compound kind="class">
    <name>MSAABackend</name>
    <filename>classMSAABackend.html</filename>
    <base>ATBackend</base>
    <member kind="function">
      <type>void</type>
      <name>startMonitoring</name>
      <anchorfile>classMSAABackend.html</anchorfile>
      <anchor>8a9f74f4f4777e425244d52a1c08a010</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NetworkSettings</name>
    <filename>classNetworkSettings.html</filename>
    <base>SystemWidget</base>
    <member kind="slot">
      <type>bool</type>
      <name>apply</name>
      <anchorfile>classNetworkSettings.html</anchorfile>
      <anchor>825cf57f34e83c5eab56e2b47577c40d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>reset</name>
      <anchorfile>classNetworkSettings.html</anchorfile>
      <anchor>48e9d4b30653631947f47f0b8afe1c8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classNetworkSettings.html</anchorfile>
      <anchor>74e9971ea310f1ccac0c61f256fd1908</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NetworkSettings</name>
      <anchorfile>classNetworkSettings.html</anchorfile>
      <anchor>a5a52e45936759aade4421612fa592bb</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isComplete</name>
      <anchorfile>classNetworkSettings.html</anchorfile>
      <anchor>9825886f6fca65ea65562015af5614cf</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>OSD</name>
    <filename>classOSD.html</filename>
    <member kind="function">
      <type></type>
      <name>OSD</name>
      <anchorfile>classOSD.html</anchorfile>
      <anchor>3f65de1a8c2d0c22ec29690d039aa220</anchor>
      <arglist>(QString message, int timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>paintEvent</name>
      <anchorfile>classOSD.html</anchorfile>
      <anchor>8dee6d22b07f172a23775b90ab5d05ac</anchor>
      <arglist>(QPaintEvent *e)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~OSD</name>
      <anchorfile>classOSD.html</anchorfile>
      <anchor>93fde13b38ef9b4d34d09a34af260c75</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PasswordDlg</name>
    <filename>classPasswordDlg.html</filename>
    <member kind="function">
      <type></type>
      <name>PasswordDlg</name>
      <anchorfile>classPasswordDlg.html</anchorfile>
      <anchor>bcd0407b26775404209fceb266a84ca5</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PasswordSettings</name>
    <filename>classPasswordSettings.html</filename>
    <base>SystemWidget</base>
  </compound>
  <compound kind="class">
    <name>PostProcessing</name>
    <filename>classPostProcessing.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>process</name>
      <anchorfile>classPostProcessing.html</anchorfile>
      <anchor>e125ff80352842ecb8212377deacedc8</anchor>
      <arglist>(QString in, QString out, bool deleteIn=false)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Program</name>
    <filename>classProgram.html</filename>
  </compound>
  <compound kind="class">
    <name>ProgramManager</name>
    <filename>classProgramManager.html</filename>
  </compound>
  <compound kind="class">
    <name>QuickDownloader</name>
    <filename>classQuickDownloader.html</filename>
    <member kind="slot">
      <type>bool</type>
      <name>download</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>48f981e434a8976cc52b90b03b35a9db</anchor>
      <arglist>(QString url, QString filename=&quot;&quot;)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>cancelDownload</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>57dcbd26b0ebbd9d296c3d8c55b06ea4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QuickDownloader</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>67b915eb1fded253c32cf2ad50aaf904</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~QuickDownloader</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>4a6c586116c2d77e6fadc5ce9c9868a1</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>QuickUnpacker</name>
    <filename>classQuickUnpacker.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>unpack</name>
      <anchorfile>classQuickUnpacker.html</anchorfile>
      <anchor>75149090ae8460ecab8a98ddf92c16c5</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QuickUnpacker</name>
      <anchorfile>classQuickUnpacker.html</anchorfile>
      <anchor>e114727a91878c8525a6393fd51b68ff</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~QuickUnpacker</name>
      <anchorfile>classQuickUnpacker.html</anchorfile>
      <anchor>61a5701ab5c3dae46be7aa3d9d93cbb8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RecWidget</name>
    <filename>classRecWidget.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>record</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>f2f6580b665a3f623005ae07869551dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>stopRecording</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>ea566b7c6702d650a9bd4dd7059b1573</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setTitle</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>cff91a24bca63f7bc03899b8436057a4</anchor>
      <arglist>(QString newTitle)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>playback</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>2e63443ea49cc1e9d4a9a92f7751b579</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>stopPlayback</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>13ca4ca5375f23f165cce329668799d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>finishPlayback</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>93c5ef29b1e79cfb44460fe5e6716e72</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteSample</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>0991833910ffc843a47e5012ae078a53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayPosition</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>f2acddbf052c99b89296db22293d3ede</anchor>
      <arglist>(int msecs)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayRecordingProgress</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>1d13bebfa513b44b47b34d1247315208</anchor>
      <arglist>(int msecs)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayPlaybackProgress</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>60cfc801a8f918210bf4ccf1863445c9</anchor>
      <arglist>(int msecs)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RecWidget</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>b52a7d0210c76e7b1408391af12da0b2</anchor>
      <arglist>(QString name, QString filename, QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasRecordingReady</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>fad08a191b6b9d3d99089d5fa8f55c76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RecWidget</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>0281fa29f7ea412336316bd9a1a238a9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RegistryManager</name>
    <filename>classRegistryManager.html</filename>
    <member kind="function">
      <type></type>
      <name>RegistryManager</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>4dde08ee67a79be976df833c6bf79603</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RegistryManager</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>e4a4c085d203b971dfb021e48ccba721</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startProcess</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>455c818be2188e30bff95c7ada05ad07</anchor>
      <arglist>(QString command, QString workingDirectory=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QStringList *</type>
      <name>getAllPrograms</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>2c2996b4e3f619fd4cc6402bf90239cc</anchor>
      <arglist>(QString format)</arglist>
    </member>
    <member kind="function">
      <type>QStringList *</type>
      <name>getAllPrograms</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>40dad9068ffc0368e30dfc40b89f5f09</anchor>
      <arglist>(const QStringList formats)</arglist>
    </member>
    <member kind="function">
      <type>QStringList *</type>
      <name>getAllFormats</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>03df20f37dd1fc2b81301636b12bcef5</anchor>
      <arglist>(QString categorie=&quot;all&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPath</name>
      <anchorfile>classRegistryManager.html</anchorfile>
      <anchor>9256270ed1f35194bbe2943194fcbc1b</anchor>
      <arglist>(QString exeStr)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RemotePlacePage</name>
    <filename>classRemotePlacePage.html</filename>
    <member kind="function">
      <type></type>
      <name>RemotePlacePage</name>
      <anchorfile>classRemotePlacePage.html</anchorfile>
      <anchor>e932dd2b28288c723aff24d24c215909</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RemotePlacePage</name>
      <anchorfile>classRemotePlacePage.html</anchorfile>
      <anchor>ed31d344722e691da5e7edc95d03edc1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPlacePath</name>
      <anchorfile>classRemotePlacePage.html</anchorfile>
      <anchor>f63033818a58b15c3175f4414a19368a</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RenameTerminal</name>
    <filename>classRenameTerminal.html</filename>
  </compound>
  <compound kind="class">
    <name>RenameTerminalSelectParametersPage</name>
    <filename>classRenameTerminalSelectParametersPage.html</filename>
  </compound>
  <compound kind="class">
    <name>ReturnTableWidget</name>
    <filename>classReturnTableWidget.html</filename>
  </compound>
  <compound kind="class">
    <name>RtAudio</name>
    <filename>classRtAudio.html</filename>
    <member kind="enumeration">
      <name>Api</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d849</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNSPECIFIED</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d849a49af3ac767106158bc6cad74fb214ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_ALSA</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d849a7a2ba095c2806caa893b6fb8fc3a1a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_OSS</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d84939c84615e61de75e6a4e865596d62c82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNIX_JACK</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d8494767e17c6edfbd827f91763915df4105</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MACOSX_CORE</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d8499fec92b8272244a87a6a11a717bd662d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WINDOWS_ASIO</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d849219517d1df90f94d07758481155bd469</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WINDOWS_DS</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d84928478830f42c2fd61e6c7ad498901931</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTAUDIO_DUMMY</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c9b6f625da88249d08a8409a9db0d849b06b8fd88bf18c9a2b8271f6961ae2d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>fd0bfa26deae9804e18faff59d0273d9</anchor>
      <arglist>(RtAudio::Api api=UNSPECIFIED)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>a8a9716fd64680657ef69c9465442a2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>RtAudio::Api</type>
      <name>getCurrentApi</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>83687634795792b2c47e4ae1cf8a5246</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getDeviceCount</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>747ce2d73803641bbb66d6e78092aa1a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RtAudio::DeviceInfo</type>
      <name>getDeviceInfo</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>02d7ff44ad1d7eae22283a052f3dfda8</anchor>
      <arglist>(unsigned int device)</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getDefaultOutputDevice</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>3a3f3dbe13ea696b521e49cdaaa357bc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getDefaultInputDevice</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>ad8b94edd3cd379ee300b125750ac6ce</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>openStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>facc99740fa4c5606fb35467cdea6da8</anchor>
      <arglist>(RtAudio::StreamParameters *outputParameters, RtAudio::StreamParameters *inputParameters, RtAudioFormat format, unsigned int sampleRate, unsigned int *bufferFrames, RtAudioCallback callback, void *userData=NULL, RtAudio::StreamOptions *options=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>closeStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>90d599002ad32cf250a4cb866f2cc93a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>ec017a89629ccef66a90b60be22a2f80</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stopStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>f4c241ff86936ecc8108f0d9dfe3efdd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>abortStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>d0586b47cd6bb9591a80b4052815991f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isStreamOpen</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>8c432a65915ce12c9d7d97676464e4d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isStreamRunning</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>546338ee719fd2ddd9ef4f64483eaeb9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>getStreamTime</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>344e59a62353c5791db4621b985cb2ca</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>getStreamLatency</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>843c989d9f501c71bc2f2c5ca18df9f3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showWarnings</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>f0752ee51cce3dd90a3bd009f9fdbe77</anchor>
      <arglist>(bool value=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>getCompiledApi</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>1df0b60d45637f65dab961b658056503</anchor>
      <arglist>(std::vector&lt; RtAudio::Api &gt; &amp;apis)</arglist>
    </member>
    <class kind="struct">RtAudio::DeviceInfo</class>
    <class kind="struct">RtAudio::StreamOptions</class>
    <class kind="struct">RtAudio::StreamParameters</class>
  </compound>
  <compound kind="struct">
    <name>RtAudio::DeviceInfo</name>
    <filename>structRtAudio_1_1DeviceInfo.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>probed</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>97573717ccb1e75a834a0a108cd6982d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>name</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>24df596ab8d54f2eb540c7cf16212b3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>outputChannels</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>81aaf488f8158ef55b1bb678f66feb7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>inputChannels</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>56247b458a937ae84d98ca9c4b243275</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>duplexChannels</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>3e8ad34c2d14e24293a0beafc26186e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isDefaultOutput</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>5365245e73ffe29a05734de4474acd58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isDefaultInput</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>7c8aca084bd908799b6be5954b80def2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; unsigned int &gt;</type>
      <name>sampleRates</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>fa65f5f92f66ce7aacd3ff35e11b230f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>nativeFormats</name>
      <anchorfile>structRtAudio_1_1DeviceInfo.html</anchorfile>
      <anchor>23251b549b89b2fe04a57fcd7cb76ea4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtAudio::StreamOptions</name>
    <filename>structRtAudio_1_1StreamOptions.html</filename>
    <member kind="variable">
      <type>RtAudioStreamFlags</type>
      <name>flags</name>
      <anchorfile>structRtAudio_1_1StreamOptions.html</anchorfile>
      <anchor>0ecc98b031aa3af49d09b781643e298b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>numberOfBuffers</name>
      <anchorfile>structRtAudio_1_1StreamOptions.html</anchorfile>
      <anchor>75a14cfab903d0e1c091bc16aec80240</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>streamName</name>
      <anchorfile>structRtAudio_1_1StreamOptions.html</anchorfile>
      <anchor>2a4b0b8d690c624d20f4e8f4be6ca9ba</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtAudio::StreamParameters</name>
    <filename>structRtAudio_1_1StreamParameters.html</filename>
    <member kind="variable">
      <type>unsigned int</type>
      <name>deviceId</name>
      <anchorfile>structRtAudio_1_1StreamParameters.html</anchorfile>
      <anchor>b3c72bcf3ef12149ae9a4fb597cc5489</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>nChannels</name>
      <anchorfile>structRtAudio_1_1StreamParameters.html</anchorfile>
      <anchor>88a10091b6e284e21235cc6f25332ebd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>firstChannel</name>
      <anchorfile>structRtAudio_1_1StreamParameters.html</anchorfile>
      <anchor>d4b4503782653ec93c83328c46abe50c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RtError</name>
    <filename>classRtError.html</filename>
    <member kind="enumeration">
      <name>Type</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WARNING</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac3a1603c24a56cbdaf5f8ae4ddcb86398</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEBUG_WARNING</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac20e5a369394d19b704ace17002007eba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNSPECIFIED</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac444efd3ccf774b2ba9f9bde70ec71cd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NO_DEVICES_FOUND</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903acf267eb2d710f33d649ba840eeab6ff82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_DEVICE</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903acd6064062066fffdba258237a7c2159b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MEMORY_ERROR</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903acb4f813e5a36905c89d4081a59497432e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_PARAMETER</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903aca3eabf0f71120beaba94148a1b78fed6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_USE</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac8e65f51a3fbdc1a4a1552b3260df36bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DRIVER_ERROR</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac995e97ecf6beeedaba525022a63aec6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SYSTEM_ERROR</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903ac8947d0a2e2a84ecf3646271844e06a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>THREAD_ERROR</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b04667aae01bffc354a9ac6bda6903aceb6282f991e2d93ab294a4272fc3f6c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtError</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>a479a305ccbe56be0fd9137b9b405a37</anchor>
      <arglist>(const std::string &amp;message, Type type=RtError::UNSPECIFIED)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~RtError</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>6793e03386e7e69ffafb65a296dfa48c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>printMessage</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>da41f7472122f45bc5b4677f066e0943</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const Type &amp;</type>
      <name>getType</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>4ee7df9728d73f533afbaddcd9ca4d9c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const std::string &amp;</type>
      <name>getMessage</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>c7f467788e29d246333d7af050801164</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char *</type>
      <name>what</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>eb843b7a7785d66061c61ebfc29e7e9d</anchor>
      <arglist>(void) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RunApplicationView</name>
    <filename>classRunApplicationView.html</filename>
    <base>InlineWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>insertCommands</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>0baf760ac40e416eda982dc4e70ad1e8</anchor>
      <arglist>(CommandList list)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>runCommand</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>489e76c67e181ce91acccae5b5fd2abb</anchor>
      <arglist>(QTableWidgetItem *command)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>runSelectedCommand</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>2971452835c47aa115a7d6c6b4c8d4fc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>clearSearchText</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>1a95a67ac5bf3ba42db62aa518099744</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>filterByPattern</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>8cbad23f08609527b93f24455c6a5038</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>filterByPattern</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>e26608923a324af646cb199c2d0542b9</anchor>
      <arglist>(QString filter, CommandList *commlist=NULL)</arglist>
    </member>
    <member kind="slot">
      <type>CommandList</type>
      <name>filterByCategory</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>6aef4b6a276ffe57bfd6be13fddb9039</anchor>
      <arglist>(int commandtype, CommandList *commlist=NULL)</arglist>
    </member>
    <member kind="slot">
      <type>CommandList</type>
      <name>filterByStrCategory</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>e89eea5e60d52344463e71f1b52178d0</anchor>
      <arglist>(QString commandtype, CommandList *commlist=NULL)</arglist>
    </member>
    <member kind="slot">
      <type>int</type>
      <name>getCategory</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>6d0345a6bdd053dff222cab0dc0d04f2</anchor>
      <arglist>(QString commandtype)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>loadCommands</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>33c2dd67546bf794dcfb9c93edfa5457</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RunApplicationView</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>d276b16cc6ebac9cbe8e8d03b1ff4584</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RunApplicationView</name>
      <anchorfile>classRunApplicationView.html</anchorfile>
      <anchor>855e618c1553c4e4824a65b048424dc8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="interface">
    <name>RunBackend</name>
    <filename>classRunBackend.html</filename>
  </compound>
  <compound kind="class">
    <name>RunCommand</name>
    <filename>classRunCommand.html</filename>
    <member kind="function">
      <type>CommandList</type>
      <name>getCommands</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>3f66602a84d646dc86d5ab4f3d4e1193</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readCommands</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>6b8fb1257c10683af3bf80afce8f8f2e</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>replaceCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>651eba552746c7bbdb76ebe49275984b</anchor>
      <arglist>(QString commandName, Command *newCommand)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>2fb5f8a0e8d8358726a2734b699246ab</anchor>
      <arglist>(Command *newCommand)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deleteCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>43a261bae746edf2b42a0017c9e0048e</anchor>
      <arglist>(QString commandName)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>commandExists</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>466dc5316146984ca4553f6d0b1446d0</anchor>
      <arglist>(QString commandName)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>f58bcd59e18aa3ef178bdee9fbabcc00</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>reload</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>9c0193da022c6063e0954fd74cb07cb5</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>run</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>865faa2d4b1a377352ebdcf5a846546b</anchor>
      <arglist>(QString commandName)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RunCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>9bd0ead049d3a608c8a2acaae43ce634</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>RunCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>a90c791fdeed677813eaeb16a44a3057</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RunLinuxBackend</name>
    <filename>classRunLinuxBackend.html</filename>
    <base>RunBackend</base>
    <member kind="function">
      <type></type>
      <name>RunLinuxBackend</name>
      <anchorfile>classRunLinuxBackend.html</anchorfile>
      <anchor>ac455bad79cb974eec0ba91b8862a17f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>goTo</name>
      <anchorfile>classRunLinuxBackend.html</anchorfile>
      <anchor>9a9b1e7cc1caa6afda5dd06a999a24f8</anchor>
      <arglist>(QString place)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classRunLinuxBackend.html</anchorfile>
      <anchor>fbd325281bf6d17a2d6c4f8ef30c4c48</anchor>
      <arglist>(QString command, QString workingDir)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RunLinuxBackend</name>
      <anchorfile>classRunLinuxBackend.html</anchorfile>
      <anchor>1d0203c9f3d142c3aef5a2a7187aba36</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RunWindowsBackend</name>
    <filename>classRunWindowsBackend.html</filename>
    <base>RunBackend</base>
    <member kind="function">
      <type></type>
      <name>RunWindowsBackend</name>
      <anchorfile>classRunWindowsBackend.html</anchorfile>
      <anchor>a635524daaacf4268dbde10197c0b6eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>goTo</name>
      <anchorfile>classRunWindowsBackend.html</anchorfile>
      <anchor>d3c491cee0ecbfb086b2eff31c14375b</anchor>
      <arglist>(QString place)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classRunWindowsBackend.html</anchorfile>
      <anchor>1e71b573a544fdafc38d95d87db5f0ef</anchor>
      <arglist>(QString command, QString workingDirectory=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RunWindowsBackend</name>
      <anchorfile>classRunWindowsBackend.html</anchorfile>
      <anchor>45892f768471714355985e99209c96d3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ScreenGrid</name>
    <filename>classScreenGrid.html</filename>
  </compound>
  <compound kind="class">
    <name>SelectLanguageModelSourcePage</name>
    <filename>classSelectLanguageModelSourcePage.html</filename>
  </compound>
  <compound kind="class">
    <name>SelectProgramPage</name>
    <filename>classSelectProgramPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>searchForPrograms</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>040a4361aff42dba53fb553946d866b4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SelectProgramPage</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>5a04d83051e7381db9a72ca12e72e981</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SelectProgramPage</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>27ca4b66717d21fcdeaa52de9d78b350</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insertCategories</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>5527da2ebf083a07d007822b7fe4c699</anchor>
      <arglist>(ProgramCategoryList categorieList)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insertPrograms</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>5d97f357713c65ca5380e848fdeb138e</anchor>
      <arglist>(ProgramList *programList)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initializePage</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>ecc9cbe4da4523d373644b30d20ec386</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getExecPath</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>b82b3e6d3d6b4c6f7ca9d045669dd760</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getName</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>6975026295b87947121b07b10f1ae3c0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getIcon</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>209db171b5a29407f5fc7de88ba8666c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getWorkingDirectory</name>
      <anchorfile>classSelectProgramPage.html</anchorfile>
      <anchor>075bbe7c975d7be76b5f10e62a0ec836</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SelectShortcutButton</name>
    <filename>classSelectShortcutButton.html</filename>
    <member kind="function">
      <type></type>
      <name>SelectShortcutButton</name>
      <anchorfile>classSelectShortcutButton.html</anchorfile>
      <anchor>667abc32f292a629ff3dced810d6d581</anchor>
      <arglist>(QString name, QString text=&quot;&quot;, QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Settings</name>
    <filename>classSettings.html</filename>
    <member kind="function" static="yes">
      <type>static QVariant</type>
      <name>get</name>
      <anchorfile>classSettings.html</anchorfile>
      <anchor>7d90d5a650c0b63efa2ab3beb4a1d444</anchor>
      <arglist>(QString name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QString</type>
      <name>getS</name>
      <anchorfile>classSettings.html</anchorfile>
      <anchor>118b84add6102c5acafc4e83198f7fc8</anchor>
      <arglist>(QString name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set</name>
      <anchorfile>classSettings.html</anchorfile>
      <anchor>10b3768fa2eb92d6aaeaf61b5d2ad1d7</anchor>
      <arglist>(QString name, QVariant value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>initSettings</name>
      <anchorfile>classSettings.html</anchorfile>
      <anchor>65bf9c9679a474e26d507fcc6fee0aa7</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Shortcut</name>
    <filename>classShortcut.html</filename>
    <member kind="function">
      <type>const QString</type>
      <name>getDescription</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>3036010ef2479a20153e31a41bcba8f3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>initKeys</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>09e3f666b2d2a8c4832fd7e7201c88e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const int</type>
      <name>qtKeyToSimonKey</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>201b36f4578f4096710037ec1cac44b0</anchor>
      <arglist>(int qtKey)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Key *</type>
      <name>getQtKey</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>df66c24048e4e33c4d3c7a0366f78e64</anchor>
      <arglist>(int qtKey)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const int</type>
      <name>simonKeyToqtKey</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>0308f332367a4ab075f5fb831ff967c5</anchor>
      <arglist>(int simonKey, short type)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const QString</type>
      <name>getSimonKeyname</name>
      <anchorfile>classShortcut.html</anchorfile>
      <anchor>1a942b54339a76273e0037eac62a82ec</anchor>
      <arglist>(int simonKey, short type)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ShortcutControl</name>
    <filename>classShortcutControl.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>readShortcuts</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>4127c7cf2342d9e94cebb225a19b89d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearShortcutList</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>87123e492f2ca9c41f60f995608e4bf6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>save</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>30e66210753e576a9ffd5db59ea6b68f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setShortcuts</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>b3566bc0778b08bb08f229121bab55ec</anchor>
      <arglist>(ShortcutList *newShortcuts)</arglist>
    </member>
    <member kind="function">
      <type>ShortcutList *</type>
      <name>getShortcuts</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>6a72b0ec589381903cb4a8b5617df63c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Shortcut *</type>
      <name>getShortcut</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>842b1da63fe3604e683c6fce36d81b2a</anchor>
      <arglist>(QString trigger)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>nameExists</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>2878d66c365c3d787feb2baf96d3b6c7</anchor>
      <arglist>(QString name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>insertShortcut</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>22c7d1e6577e99d47b0a9cbe05a015a3</anchor>
      <arglist>(Shortcut *shortcut)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>removeShortcut</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>5a6c80acb14692226b27f785194a690f</anchor>
      <arglist>(Shortcut shortcut)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceShortcut</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>ded98738b6a3208525f5ce3b470a18fe</anchor>
      <arglist>(Shortcut oldShortcut, Shortcut newShortcut)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>ShortcutControl</name>
      <anchorfile>classShortcutControl.html</anchorfile>
      <anchor>e134f8695d94c2377d7b0eda2afcaad2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ShortcutSettigns</name>
    <filename>classShortcutSettigns.html</filename>
  </compound>
  <compound kind="class">
    <name>ShortcutXMLReader</name>
    <filename>classShortcutXMLReader.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>ShortcutXMLReader</name>
      <anchorfile>classShortcutXMLReader.html</anchorfile>
      <anchor>1f3da77e7b3ecb195c0fb21f41a575bd</anchor>
      <arglist>(QString path=&quot;conf/shortcuts.xml&quot;, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classShortcutXMLReader.html</anchorfile>
      <anchor>ca951925e74ccfce3553101f2fca658c</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>ShortcutList *</type>
      <name>getShortcuts</name>
      <anchorfile>classShortcutXMLReader.html</anchorfile>
      <anchor>a58d0580f2ed5c2a7efa6b02bff1330e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setShortcuts</name>
      <anchorfile>classShortcutXMLReader.html</anchorfile>
      <anchor>45bd1624eba57ef5ed329117a41ed81d</anchor>
      <arglist>(ShortcutList *shortcuts)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classShortcutXMLReader.html</anchorfile>
      <anchor>05ce5b5293e6c8cd1806fbda5f5c202c</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SimonControl</name>
    <filename>classSimonControl.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>connectToJulius</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>d151e1d4109ad12776fa2ea81d74542c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disconnectFromJulius</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>b57945d88459fcb2b2f74893362e8594</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connectedToJulius</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>a223e2407d6c65487b13f9c0458d93bb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disconnectedFromJulius</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>d9306e4aad166d9b5954c7eff75b6846</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>wordRecognised</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>4dc258564201e181d0e4f095681f7a03</anchor>
      <arglist>(QString word, QString sampa, QString samparaw)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>abortConnecting</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>31d41ce1f0eb62dadca12f7f99d30aa1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>errorConnecting</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>1072051b96ea54a644f3839f44f3b081</anchor>
      <arglist>(QString error)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>connectTo</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>bb11694febb9ef70f1259156a654fc02</anchor>
      <arglist>(QString host)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deactivateSimon</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>c0782da7f2281f2459ece6292782e825</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>activateSimon</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>840356c1261f16280b9579df13fb9aff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toggleActivition</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>220c4bee559f2a66d5ca378d539a2aec</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>getActivitionState</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>5f7542d6f6fe8d36deeee8e0b77915e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimonControl</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>2ae7ca8adef8af3cfbc8d85997fb94b4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SimonControl</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>7d9d8d2273571577380132d857fea0b1</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SimonInfo</name>
    <filename>classSimonInfo.html</filename>
    <member kind="function">
      <type></type>
      <name>SimonInfo</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>450b1130e73869ef92e7b801aacb01ba</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showSplash</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>98155ccdc3e963b0d1e50af580268030</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>writeToSplash</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>11dbf5cd78caf79846c472f1dfd76f0b</anchor>
      <arglist>(QString status)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hideSplash</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>df50e7383f36a6ca7657369bf91806cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SimonInfo</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>60529ff0386095a1cfe04bd2919e110a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>showMessage</name>
      <anchorfile>classSimonInfo.html</anchorfile>
      <anchor>7880f84d7af73379a1eec07326a040a3</anchor>
      <arglist>(QString message, short time)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SimonMainWindow</name>
    <filename>classSimonMainWindow.html</filename>
    <base>SimonWidget</base>
  </compound>
  <compound kind="class">
    <name>SimonView</name>
    <filename>classSimonView.html</filename>
    <base>SimonMainWindow</base>
    <member kind="slot">
      <type>void</type>
      <name>toggleVisibility</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>0de0c5b222cc28a6935492c946057c0e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>hideSimon</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>fe7c99100a1ca45c8ab48cf153f8dfd4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showSimon</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>8c0c6cf0868d9bb0b686070b4c98fea2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>closeSimon</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>6b85ea68dd919665b07b79463f279e3b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connectToServer</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>07195dc1be4755f934f4548293078f99</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>abortConnecting</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>b2db99cee13f343eff7117c5c1dea04c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>errorConnecting</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>64a7ecb5bbe4eeced3cc005a67e3c24f</anchor>
      <arglist>(QString error)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>connected</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>039bf478566538a7e78c38768d4bc1c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disconnected</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>388c07a35c79057625e77d69cbadf374</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>toggleActivation</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>eecaa4eac3455f6a2e7b68defb415998</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>representState</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>9e4f840a25e86f4158428bb5edb503d8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showAddWordDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>9d87a2b563a8591a80fbbe3d9c0b6127</anchor>
      <arglist>(bool show=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showRunDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>0f50dd495195786bae747475c103366b</anchor>
      <arglist>(bool show=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showTrainDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>9792414ed2bc9c3d94605d2b936d9144</anchor>
      <arglist>(bool show=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showWordListDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>017017f85d954ec47a2215cf45242416</anchor>
      <arglist>(bool show=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showSystemDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>6729252463c45ff33d955d9d149722e7</anchor>
      <arglist>(bool show=true)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>inlineWidgetRegistered</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>e350b58d4785c9648e77e3ef7b4248f1</anchor>
      <arglist>(InlineWidget *widget)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>inlineWidgetUnRegistered</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>13d6f665f1b0a2dbf3b5cf2200bc13ff</anchor>
      <arglist>(InlineWidget *widget)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>checkSettingState</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>ba527cbb9a9a87cf021fda319f753d3a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimonView</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>72d0f91bec8895ca946b79a47c17c760</anchor>
      <arglist>(QWidget *parent=0, Qt::WFlags flags=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SimonView</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>812f34e64284d55d66db5e48885e0f2d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>closeEvent</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>138ad02b265e961ceb98cab9bd4cfcb0</anchor>
      <arglist>(QCloseEvent *event)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkPassword</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>4828191f67e901b93a71d7be4cc7610b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hideSettings</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>2d00d9f1dbbb6e07a516886c71594a34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showSettings</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>85ca3f659b988593c0a81992426b810b</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SimonWidget</name>
    <filename>classSimonWidget.html</filename>
  </compound>
  <compound kind="class">
    <name>SoundControl</name>
    <filename>classSoundControl.html</filename>
    <member kind="function">
      <type>SoundDeviceList *</type>
      <name>getInputDevices</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>5d27c7db1e7715c6520e138b2ac268cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SoundDeviceList *</type>
      <name>getOutputDevices</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>1021e48b79394e0b7d9faa385cb54ba6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SoundControl</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>d48c17386cb800e8413997c313a38bb8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SoundControl</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>e36289abf3de9f18452fb59bed71a4d3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SoundDevice</name>
    <filename>classSoundDevice.html</filename>
  </compound>
  <compound kind="class">
    <name>SoundSettinsg</name>
    <filename>classSoundSettinsg.html</filename>
  </compound>
  <compound kind="class">
    <name>SystemView</name>
    <filename>classSystemView.html</filename>
    <base>InlineWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>apply</name>
      <anchorfile>classSystemView.html</anchorfile>
      <anchor>9ff85eb1b5b9566d73252e7567c0d2c2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classSystemView.html</anchorfile>
      <anchor>e32a7254f4b0ed7b412438db216dae1a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SystemView</name>
      <anchorfile>classSystemView.html</anchorfile>
      <anchor>660a455ff7b98cb92410b0bf1cbb2eeb</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerControl</name>
      <anchorfile>classSystemView.html</anchorfile>
      <anchor>a321ce8e5ce04b9755201fe947e68ccb</anchor>
      <arglist>(SystemWidget *control)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deleteControl</name>
      <anchorfile>classSystemView.html</anchorfile>
      <anchor>1b837d94f9ad1de5d7169e349b1db2fe</anchor>
      <arglist>(SystemWidget *control)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SystemWidget</name>
    <filename>classSystemWidget.html</filename>
    <base>InlineWidget</base>
    <member kind="function">
      <type></type>
      <name>SystemWidget</name>
      <anchorfile>classSystemWidget.html</anchorfile>
      <anchor>14f568fd3d928d1d47e65ea8d6b9e52e</anchor>
      <arglist>(QString title, QIcon icon, QString desc, QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrainingManager</name>
    <filename>classTrainingManager.html</filename>
    <member kind="function">
      <type>TrainingList *</type>
      <name>readTrainingTexts</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>3de8141319264993a096ac31e82fa2ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getProbability</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>d573805e2f63be29918bf4f2a17ae5f7</anchor>
      <arglist>(QString name)</arglist>
    </member>
    <member kind="function">
      <type>PromptsTable *</type>
      <name>readPrompts</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>f993e2c99760b9fa4f4f009b62442cca</anchor>
      <arglist>(QString pathToPrompts)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deleteWord</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>d0d9e51f6f43db356aef39063394011c</anchor>
      <arglist>(Word *w, bool recompiledLater=false)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deletePrompt</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>34d503167d23ff51b7cef79979c21f3d</anchor>
      <arglist>(QString key)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>savePrompts</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>efcad95d5b09e85512bc3a5b9a84370f</anchor>
      <arglist>(bool recompiledLater=false)</arglist>
    </member>
    <member kind="function">
      <type>TrainingList *</type>
      <name>getList</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>6ed1d6e5a54eb902049faca1d910101d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deleteText</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>4ef23143cb83f67b14a8e47d026a5c45</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>calcRelevance</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>d63219085278d08b854666e382780f5f</anchor>
      <arglist>(TrainingText *text)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trainWords</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>085160d7f74f9a89ff9a3de0e8803fe1</anchor>
      <arglist>(WordList *words)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>trainText</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>eca4f763539db90433d65ebfbbd9f85f</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getPageCount</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>b1de1ad62095085465ddf3381c9dadd0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPage</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>692b44b5b59e06d775784e2f0391d7fb</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getTextName</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>29a815d31a76552b8c052d14189311e8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TrainingText *</type>
      <name>getText</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>9d267e0b4c5e24a4b9161b8dd10a4e62</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finishTrainingSession</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>1001b74103d97c7b98f7785d675d261f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>allWordsExisting</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>545ecd1c81f6f145ccf7d36adebf9987</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addSamples</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>d35b837ce1cb6c1a2bda2234a5ca0cee</anchor>
      <arglist>(QHash&lt; QString, QString &gt; *hash)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>writePrompts</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>e6b4a2add16513c716c4dec003e3f173</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TrainingManager</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>c47942d070449a9838d06af987a75f7b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>TrainingManager</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>84466cd227fd49922cd9596e1ae68cb4</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrainingText</name>
    <filename>classTrainingText.html</filename>
    <member kind="function">
      <type>QString</type>
      <name>getName</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>db4f8b2432612888e136b8f9294fad2a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPath</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>7be639c160ccb6d91ab016700be87ccf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getPageCount</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>e9aca6f5701923e58abcea96c7d394b8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>getPages</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>3254a633bebc3b75addc155c7f4f752d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPage</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>6dda1f2ee74bb4311455914bd19dfcc0</anchor>
      <arglist>(int page)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getRelevance</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>5e971a9ff258a66b60a94f0d6e3a2d10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRelevance</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>e726b184bcfa19067b42a83ee724d4e0</anchor>
      <arglist>(float relevance)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TrainingText</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>203dafd34b42f56edcc9e501b12f6a70</anchor>
      <arglist>(QString name, QString path, QStringList pages)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TrainingText</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>8fcf097d5c57d8e2e3d78b370e553c6d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrainingView</name>
    <filename>classTrainingView.html</filename>
    <base>InlineWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>47644e1e2c4dd3dc7a770d13b5ca5ac2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importTexts</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>562fa3544b3fc8e69abbcc82b380d6a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>trainSelected</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>e5ce7c463648392f6e85163b043cdf73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>nextPage</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>53a7e4d23adf2f1b4daeb289a22d8a2d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>prevPage</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>d84c12a248810e90a3bac5a7c89ecc13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>loadList</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>e7d1bd5170244c131a514aa898944f21</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>cancelReading</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>0011abad7876f2807c2742416629542c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>cancelTraining</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>9c2fb61687f5abbd6faccf2ace3bba61</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>resetRecorder</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>e5f0a4a0685310bf591bcbaa89014fc1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteSelected</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>3a29b17a0f958192b540694ebf1d5939</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>fetchPage</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>ff43e460e22cc3f62105788b3df09787</anchor>
      <arglist>(int page)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>trainWords</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>307754d54697d505871eb72d28c53c0e</anchor>
      <arglist>(WordList *words)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>startTraining</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>99d165ab087adec1cb70632b310882c1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importDirectory</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>264419ce970ca5a7dadf5c6f3e1108d6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>exec</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>fceb1659239e5cc21ff42399673b817b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TrainingView</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>f00ed658a3724584c80f95c0fad2fe8c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hideSettings</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>072037c20f9211b45cd1f0750921a920</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSettingsVisible</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>c82dae6e23bda06a70e4e605a376eb00</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cleanUpTrainingSamples</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>7617ba6e5d082d954b3c3de094ff43e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>TrainingView</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>6d7002ef0883fd49f0a7e8a4a4801e4c</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrayIconManager</name>
    <filename>classTrayIconManager.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>triggered</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>b8cff621316189a9ef0abbca47beb8b1</anchor>
      <arglist>(QSystemTrayIcon::ActivationReason reason)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>clicked</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>a32ae04fe97636b435f2736ebe7d8b61</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>middleClicked</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>fdf5dc19a8f44f17dccc6d7ddbea064d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>createIcon</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>8cf4876dd2f7946fac05c021516b3779</anchor>
      <arglist>(QIcon icon, QString tooltip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hideIcon</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>172d3e2dcc35bdad5f0e38e9d3765dff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TrayIconManager</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>25b391d262757a31644d65acdd0783f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TrayIconManager</name>
      <anchorfile>classTrayIconManager.html</anchorfile>
      <anchor>43b829fc6b2e84b18d899c818b5a14ff</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WAV</name>
    <filename>classWAV.html</filename>
    <member kind="function">
      <type></type>
      <name>WAV</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>df251a5780f35271dfc5c9009bb7d7ee</anchor>
      <arglist>(QString filename, int channels=0, int samplerate=0)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>getRawData</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>252508fca7e5d792da812bcb63e746a3</anchor>
      <arglist>(int &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addData</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>77379d9b2e66ffd4f144506c8b2ca9f3</anchor>
      <arglist>(char *data, int length)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeFile</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>c9e42b0903381d097edd4c23fd481bf0</anchor>
      <arglist>(QString filename=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WAV</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>bf955e178e2e6682dbcf99b1f94db7a2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WavPlayer</name>
    <filename>classWavPlayer.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>c656a3c3546c39d7ba2678c1470e68c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>WavPlayer</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>b5c02ed6a1a20a998884a9e963a247d1</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>play</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>45f52976fda8cfd2b8f526153c44b7a1</anchor>
      <arglist>(QString filename)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WavPlayer</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>16c8f68bba1231eb3eca569bb5b3c0d3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WavRecorder</name>
    <filename>classWavRecorder.html</filename>
    <member kind="function">
      <type></type>
      <name>WavRecorder</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>0bd28fafb28c4fe82b546877aaf60d1f</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>finish</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>2cbc0924c5a46c6125f24cce19372493</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>record</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>e7c08332aef4e1c8a7762bdb62f0c88a</anchor>
      <arglist>(QString filename)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WavRecorder</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>91e335cc29e6d58b18d70bdf4ea5e676</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WiktionaryDict</name>
    <filename>classWiktionaryDict.html</filename>
    <base>Dict</base>
    <member kind="function">
      <type></type>
      <name>WiktionaryDict</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>0b2b78823ee4727c7e7a65c837ba9e2b</anchor>
      <arglist>(QString path=&quot;&quot;, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>b750ad1ea4c856ea207b6663c8aa0eac</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>startElement</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>5567a2d1fa9df8bb73f36b8804e07a61</anchor>
      <arglist>(const QString &amp;namespaceURI, const QString &amp;localName, const QString &amp;qName, const QXmlAttributes &amp;attributes)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>endElement</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>dda21266a3e2569ef72d53a39f74f712</anchor>
      <arglist>(const QString &amp;namespaceURI, const QString &amp;localName, const QString &amp;qName)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>characters</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>a4f208faded44e428eb250a47802ff8f</anchor>
      <arglist>(const QString &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>findIPAs</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>b83a24fa66a2323491662ec053d13ed3</anchor>
      <arglist>(QString haystack)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insertWords</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>7040baf73f3b28d46a0202b82ef045fe</anchor>
      <arglist>(QString word, QString terminal, QStringList pronunciations)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WiktionaryDict</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>0a192bbc2357001106938f1a9b9f1039</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WindowsProgramManager</name>
    <filename>classWindowsProgramManager.html</filename>
    <base>ProgramManager</base>
    <member kind="function">
      <type></type>
      <name>WindowsProgramManager</name>
      <anchorfile>classWindowsProgramManager.html</anchorfile>
      <anchor>8cbfe4571c837c174f9822fe023c421f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WindowsProgramManager</name>
      <anchorfile>classWindowsProgramManager.html</anchorfile>
      <anchor>94fc7cf1c3866618caca8786a90625fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ProgramList *</type>
      <name>getPrograms</name>
      <anchorfile>classWindowsProgramManager.html</anchorfile>
      <anchor>9f13765a730264bb29e3da6db5ded701</anchor>
      <arglist>(ProgramCategory categorie)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WindowsResourceHandler</name>
    <filename>classWindowsResourceHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>WindowsResourceHandler</name>
      <anchorfile>classWindowsResourceHandler.html</anchorfile>
      <anchor>863d1b3b3c715ce0ab44de156e094c9f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QIcon</type>
      <name>retrieveIcon</name>
      <anchorfile>classWindowsResourceHandler.html</anchorfile>
      <anchor>4f41eae0f78451ac4f9b14944ba0f7f9</anchor>
      <arglist>(QString file, int resourceId)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Word</name>
    <filename>classWord.html</filename>
    <member kind="function">
      <type></type>
      <name>Word</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>184ce163272988a1596262560976f118</anchor>
      <arglist>(QString word, QString pronunciation, QString terminal=NULL, int probability=NULL)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getWord</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>8a6b1d869b70c7d837cc43ffbd25794a</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPronunciation</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>21b992c79da61fb6cb02d8d6fa39cd2c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setProbability</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>93833770d3212af0257179e0d8c209ca</anchor>
      <arglist>(int newProbability)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getTerminal</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>54df1d45c3662d2cfcaf294eeb9fff6b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTerminal</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>a3c807d79fbd9e84d6047d14af96ca7e</anchor>
      <arglist>(QString terminal)</arglist>
    </member>
    <member kind="function">
      <type>const bool</type>
      <name>operator&lt;</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>c472b467e339367486d8c85af7253ea8</anchor>
      <arglist>(const Word w2) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getPropability</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>21890294c901f3867ed7e0e50779179d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Word</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>1fbcaae6859604d92e94cab540cb3523</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WordListManager</name>
    <filename>classWordListManager.html</filename>
    <member kind="function">
      <type>void</type>
      <name>safelyInit</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>3a4d064572564da89627da4c2bc27292</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>c355ce506c54e480ed14307cacc1f0bc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>saveWordList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>aaef927dac4280e85e08a269bac3252f</anchor>
      <arglist>(WordList *list, QString lexiconFilename, QString vocabFilename)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getRandomWord</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>14d982c69d11c6d4f3f5d88dcddbef67</anchor>
      <arglist>(QString terminal, bool includeShadow=true)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>mergeLists</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>149b2d9f956375f8b58954c7c60f992b</anchor>
      <arglist>(WordList *a, WordList *b, bool keepDoubles=true)</arglist>
    </member>
    <member kind="function">
      <type>Word *</type>
      <name>getWord</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>84040fca0970ec62bb941ce55899ef0f</anchor>
      <arglist>(QString word, QString pronunciation, QString terminal, bool &amp;isShadowed)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getWordIndex</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>628f78ba7fa94da41080c8ec55549c3a</anchor>
      <arglist>(WordList *list, bool &amp;found, QString word, QString pronunciation=&quot;&quot;, QString terminal=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>getWordsByTerminal</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>c2b786027b1e474164933b7182cabc80</anchor>
      <arglist>(QString terminal, bool includeShadow=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addWords</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>47680fa6c9302daa912d405a5328e332</anchor>
      <arglist>(WordList *list, bool isSorted=false, bool shadow=false)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>2525a7dbdaf7740b8a0b7aef152850fa</anchor>
      <arglist>(QString lexiconFilename=&quot;&quot;, QString vocabFilename=&quot;&quot;, QString shadowLexiconFilename=&quot;&quot;, QString shadowVocabFilename=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>moveToShadow</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>5318dfc58519dce6089e4935edc27905</anchor>
      <arglist>(Word *w)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>sortList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>7577fb6403c568892b4befd66be27372</anchor>
      <arglist>(WordList *list)</arglist>
    </member>
    <member kind="function">
      <type>PromptsTable *</type>
      <name>readPrompts</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>5d7f76c226e8369206689228f3b84d6e</anchor>
      <arglist>(QString promptspath=&quot;model/prompts&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WordListManager</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>4e4ec9f1bd864bb366c5ac32375be4b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>WordListManager</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>c848412182e4c51195ca178d46aa4d0f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WordListView</name>
    <filename>classWordListView.html</filename>
    <base>InlineWidget</base>
    <member kind="slot">
      <type>void</type>
      <name>reloadList</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>28b8c77703868c7474493cded875ae8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>reloadShadowList</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>172deef1dc7bb8e53fd1ceb41a4afee1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>suggestTraining</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>438df677f31768d8b1de9d071393f962</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>markWordToTrain</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>8651f04f996d80593da1f6f012e26a9b</anchor>
      <arglist>(Word word)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>copyWordToTrain</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>df16c7584ec6cb5913f3f4a90397f1f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteTrainingWord</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>e03b4296d7c3ae9323821baebd0160e3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteSelectedWord</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>2a17f3c316281e2ffba2925367cec3af</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>trainList</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>7da96dc085e7e66e21f5094b4fe3c54c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importDict</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>9ba85fdbdcdae96ae3a0498c4afff24b</anchor>
      <arglist>(WordList *list)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>clearList</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>04b4fc99893c43e2b78ab4b01dfbd055</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>filterListbyPattern</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>d648ba3a4b74512b5e9c483e25b66b63</anchor>
      <arglist>(QString filter=&quot;&quot;)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>clearSearchText</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>51cdf5414f665dff323f4891cb8a22d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>insertVocab</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>89848319a1c57ecf2550faeb6ce827e0</anchor>
      <arglist>(WordList *vocab)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>hideTbEditModel</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>f01204d8ae1c31086d38ae8b2684d2df</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setTbEditModelVisible</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>177b25fec831256cecd9c73a1a63a42a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>WordListView</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>2fae706e007f45aa45df8c93ff7f1730</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WordListView</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>181d528f81ea89e0e8169681d9e0b539</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XEvents</name>
    <filename>classXEvents.html</filename>
    <base>CoreEvents</base>
    <member kind="function">
      <type></type>
      <name>XEvents</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>f020a6ce1fb3013ac5aa076d92da6a92</anchor>
      <arglist>(char *displayName=&quot;:0.0&quot;)</arglist>
    </member>
    <member kind="function">
      <type>Display *</type>
      <name>openDisplay</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>18245d72ddbac031910f2182196b4e95</anchor>
      <arglist>(char *displayName)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>click</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>f590669002c2da081d79bf2907c9564a</anchor>
      <arglist>(int x, int y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendChar</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>f1b93410f473b402e2cd4844bf348fe3</anchor>
      <arglist>(char key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendKey</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>be942f098905f793d9553c828fee4f64</anchor>
      <arglist>(unsigned short key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setModifierKey</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>06b1255d8229cf348ddbd4b60974a123</anchor>
      <arglist>(int virtualKey, bool once)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendShortcut</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>3bd31fed6f85822a2f8bb333afedd24f</anchor>
      <arglist>(Shortcut shortcut)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unsetModifier</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>7edcca6f34430402bacceee64a436907</anchor>
      <arglist>(int virtualKey)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XEvents</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>0cec658f1a44f943bcf93008afea09e5</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLCommand</name>
    <filename>classXMLCommand.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>XMLCommand</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>97867c89b1e6ca50e6af86bb26aae049</anchor>
      <arglist>(QString path=&quot;conf/commands.xml&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>e67856cd0ae750a20676bd4bc3c243ca</anchor>
      <arglist>(QString path=&quot;conf/commands.xml&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>d65e745272dec79a6dadbcfc4fb913a8</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>CommandList</type>
      <name>getCommands</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>80eaea582775b34f50e7117bac66b759</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLCommand</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>35f6a747f5c1ca7eba5b668501b42270</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLDomReader</name>
    <filename>classXMLDomReader.html</filename>
    <base>XMLReader</base>
    <member kind="function">
      <type></type>
      <name>XMLDomReader</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>78337747cd839545eab6372ed2d7b8ca</anchor>
      <arglist>(QString path, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>e4eb3d7bf3b54a7ebdb89d7897af1286</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>cd3cbd6785130c9a7cec5cb9cf76ef9c</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLDomReader</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>09f107df068d1574b2f4853e64568053</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLReader</name>
    <filename>classXMLReader.html</filename>
    <member kind="function">
      <type></type>
      <name>XMLReader</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>42faf9c5d78994e597f608126123b559</anchor>
      <arglist>(QString path, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLReader</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>73065c7758f26ef69387e315c96d13ac</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLSAXReader</name>
    <filename>classXMLSAXReader.html</filename>
    <base>XMLReader</base>
    <member kind="function">
      <type></type>
      <name>XMLSAXReader</name>
      <anchorfile>classXMLSAXReader.html</anchorfile>
      <anchor>5ae4ce6329668d058ed2e63c0c6d2966</anchor>
      <arglist>(QString path, QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classXMLSAXReader.html</anchorfile>
      <anchor>548b7dd5baaad6f25335d9634c25d1e3</anchor>
      <arglist>(QXmlDefaultHandler *handler, QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLSAXReader</name>
      <anchorfile>classXMLSAXReader.html</anchorfile>
      <anchor>c8cbca7caeb8a1fa6541e5930074cdeb</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLTrainingText</name>
    <filename>classXMLTrainingText.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type>int</type>
      <name>getPageCount</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>884533bcc0be4c717f0aac7e428166db</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>save</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>4c1d1f1fbe243ddc50d56c3f1fbed483</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>8ee3e55d067ee81176a087428af3322f</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getPage</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>dec15aa27d2cd77bfe3b2a4ee55e83b3</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>getAllPages</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>2b1cdd0ff63cc9af7ecd977dae053cf3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLTrainingTextList</name>
    <filename>classXMLTrainingTextList.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>XMLTrainingTextList</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>7457ce32fbbd99ad5981f5d7e6d01389</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>596c4b01ca0b53323ca4de3b6ac2145c</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLTrainingTextList</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>de25b19743fe3eedf3f9737644c93d35</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>buildtmp/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/buildtmp/</path>
    <filename>dir_6e99fc9fc3520cfd176ed4d555bbcce3.html</filename>
    <file>moc_addwordintropage.cpp</file>
    <file>moc_addwordrecordpage.cpp</file>
    <file>moc_addwordresolvepage.cpp</file>
    <file>moc_addwordview.cpp</file>
    <file>moc_atbackend.cpp</file>
    <file>moc_atobject.cpp</file>
    <file>moc_atwatcher.cpp</file>
    <file>moc_bompdict.cpp</file>
    <file>moc_bunzip.cpp</file>
    <file>moc_chooseplacepage.cpp</file>
    <file>moc_commandsettings.cpp</file>
    <file>moc_configureprogrampage.cpp</file>
    <file>moc_dbusbackend.cpp</file>
    <file>moc_definedefaultvaluespage.cpp</file>
    <file>moc_desktopgridconfiguration.cpp</file>
    <file>moc_dict.cpp</file>
    <file>moc_dragtablewidget.cpp</file>
    <file>moc_droplistwidget.cpp</file>
    <file>moc_externalprogrammanager.cpp</file>
    <file>moc_firstruncreatedictionarypage.cpp</file>
    <file>moc_firstrunimportgrammardescriptionpage.cpp</file>
    <file>moc_firstrunwizard.cpp</file>
    <file>moc_generalsettings.cpp</file>
    <file>moc_grammarsettings.cpp</file>
    <file>moc_guievents.cpp</file>
    <file>moc_iconbutton.cpp</file>
    <file>moc_icondialog.cpp</file>
    <file>moc_importbomppage.cpp</file>
    <file>moc_importdict.cpp</file>
    <file>moc_importdictselectsourcepage.cpp</file>
    <file>moc_importdictview.cpp</file>
    <file>moc_importdictwiktionarypage.cpp</file>
    <file>moc_importdictworkingpage.cpp</file>
    <file>moc_importgrammar.cpp</file>
    <file>moc_importgrammarselectfilespage.cpp</file>
    <file>moc_importgrammarwizard.cpp</file>
    <file>moc_importgrammarworkingpage.cpp</file>
    <file>moc_importlexiconpage.cpp</file>
    <file>moc_importlocalwizardpage.cpp</file>
    <file>moc_importplacepage.cpp</file>
    <file>moc_importplacewizard.cpp</file>
    <file>moc_importprogrampage.cpp</file>
    <file>moc_importprogramwizard.cpp</file>
    <file>moc_importremotewizardpage.cpp</file>
    <file>moc_importtrainingdirectory.cpp</file>
    <file>moc_importtrainingdirectoryintropage.cpp</file>
    <file>moc_importtrainingdirectoryworkingpage.cpp</file>
    <file>moc_importtrainingtextlocalpage.cpp</file>
    <file>moc_importtrainingtextremotepage.cpp</file>
    <file>moc_importtrainingtexts.cpp</file>
    <file>moc_importtrainingtextselectsourcepage.cpp</file>
    <file>moc_importtrainingtextworkingpage.cpp</file>
    <file>moc_importworkingwizardpage.cpp</file>
    <file>moc_inlinewidget.cpp</file>
    <file>moc_inlinewidgetview.cpp</file>
    <file>moc_internetextensionsettings.cpp</file>
    <file>moc_introplacepage.cpp</file>
    <file>moc_juliuscontrol.cpp</file>
    <file>moc_lexicondict.cpp</file>
    <file>moc_localplacepage.cpp</file>
    <file>moc_logmanager.cpp</file>
    <file>moc_logview.cpp</file>
    <file>moc_mergeterminals.cpp</file>
    <file>moc_mergeterminalsselectterminalspage.cpp</file>
    <file>moc_mergeterminalsworkingpage.cpp</file>
    <file>moc_modelmanager.cpp</file>
    <file>moc_modelsettings.cpp</file>
    <file>moc_msaabackend.cpp</file>
    <file>moc_networksettings.cpp</file>
    <file>moc_osd.cpp</file>
    <file>moc_passworddlg.cpp</file>
    <file>moc_passwordsettings.cpp</file>
    <file>moc_quickdownloader.cpp</file>
    <file>moc_quickunpacker.cpp</file>
    <file>moc_recwidget.cpp</file>
    <file>moc_remoteplacepage.cpp</file>
    <file>moc_renameterminal.cpp</file>
    <file>moc_renameterminalwizard.cpp</file>
    <file>moc_renameterminalworkingpage.cpp</file>
    <file>moc_revert.cpp</file>
    <file>moc_runapplicationview.cpp</file>
    <file>moc_screengrid.cpp</file>
    <file>moc_selectlanguagemodelsourcepage.cpp</file>
    <file>moc_selectprogrampage.cpp</file>
    <file>moc_selectshortcutbutton.cpp</file>
    <file>moc_selectsourcewizardpage.cpp</file>
    <file>moc_shortcutsettings.cpp</file>
    <file>moc_simoncalendarwidget.cpp</file>
    <file>moc_simoncombobox.cpp</file>
    <file>moc_simoncontrol.cpp</file>
    <file>moc_simongroupbox.cpp</file>
    <file>moc_simonlineedit.cpp</file>
    <file>moc_simonlistwidget.cpp</file>
    <file>moc_simonmainwindow.cpp</file>
    <file>moc_simonslider.cpp</file>
    <file>moc_simonspinbox.cpp</file>
    <file>moc_simontablewidget.cpp</file>
    <file>moc_simontabwidget.cpp</file>
    <file>moc_simonview.cpp</file>
    <file>moc_soundsettings.cpp</file>
    <file>moc_systemview.cpp</file>
    <file>moc_systemwidget.cpp</file>
    <file>moc_systemwidgetpage.cpp</file>
    <file>moc_trainingmanager.cpp</file>
    <file>moc_trainingview.cpp</file>
    <file>moc_trayiconmanager.cpp</file>
    <file>moc_vumeter.cpp</file>
    <file>moc_wavplayer.cpp</file>
    <file>moc_wavrecorder.cpp</file>
    <file>moc_wordlistmanager.cpp</file>
    <file>moc_wordlistview.cpp</file>
    <file>moc_xmlreader.cpp</file>
    <file>moc_xmltrainingtextlist.cpp</file>
  </compound>
  <compound kind="dir">
    <name>tmp/modeltmp/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/tmp/modeltmp/</path>
    <filename>dir_754d8966e0d233580692d3dafb3c56b6.html</filename>
    <file>dfaTemp.h</file>
  </compound>
  <compound kind="dir">
    <name>modeltobias/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/modeltobias/</path>
    <filename>dir_fc0b33cebc655350617cf2a03825eb48.html</filename>
    <dir>modeltobias/training.data/</dir>
    <dir>modeltobias/training.data_backup/</dir>
  </compound>
  <compound kind="dir">
    <name>tmp/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/tmp/</path>
    <filename>dir_5897aedc9149ff09da9dd2d6b77c5e44.html</filename>
    <dir>tmp/modeltmp/</dir>
  </compound>
  <compound kind="dir">
    <name>modeltobias/training.data/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/modeltobias/training.data/</path>
    <filename>dir_cce34339baa429ee24d71f410718e5e7.html</filename>
    <file>out.diff</file>
  </compound>
  <compound kind="dir">
    <name>modeltobias/training.data_backup/</name>
    <path>/home/bedahr/Daten/simon/source/trunk/modeltobias/training.data_backup/</path>
    <filename>dir_620a41a29bbd9d020eeb4fb5325172fd.html</filename>
    <file>out.diff</file>
  </compound>
</tagfile>
