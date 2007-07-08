<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="class">
    <name>AddWordView</name>
    <filename>classAddWordView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>saveWord</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>c57f1c42723c1afb3835ee357c423053</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>nextStep</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>d64470f22a81e7ef941b160bc1f78e9d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>prevStep</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>1a826e442eadffc79db0e8de6c53ca59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>2f4d7f340225af6754ff700c9e5b39b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>checkReady</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>d4004cde19bb06e1e5ba2d7a799fafb0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setNotReady</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>dd5eeee0b21339013afa6df015b56901</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AddWordView</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>9982dfac4b5fbdda9c09fefc33b90c14</anchor>
      <arglist>(QWidget *parent=0, Qt::WFlags flags=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~AddWordView</name>
      <anchorfile>classAddWordView.html</anchorfile>
      <anchor>78308d2d8c842889fec67b78e84af029</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CallbackInfo</name>
    <filename>structCallbackInfo.html</filename>
    <member kind="function">
      <type></type>
      <name>CallbackInfo</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>567d581fd6b0dbffbc9f7970546fb9e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>object</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>89e06f547a26d9b57bec56804c2e2d37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ThreadHandle</type>
      <name>thread</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>45db19fa393c92a55e8f5f725fbaea7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>usingCallback</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>9fab8cfe3381974bf58b268880c13c6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>callback</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>698977fa4fba0e92b2f5c7b30642f5d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>userData</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>2d45a4cf61c20f400aa606e63685b536</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>apiInfo</name>
      <anchorfile>structCallbackInfo.html</anchorfile>
      <anchor>3528274ab6dee1a780fa052be726eee3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Command</name>
    <filename>classCommand.html</filename>
    <member kind="function">
      <type></type>
      <name>Command</name>
      <anchorfile>classCommand.html</anchorfile>
      <anchor>5679754e507a37a895933773666dc6b2</anchor>
      <arglist>(QString name, CommandType type, QString value)</arglist>
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
  </compound>
  <compound kind="class">
    <name>CoreEvents</name>
    <filename>classCoreEvents.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>sendKey</name>
      <anchorfile>classCoreEvents.html</anchorfile>
      <anchor>fde4f79f8f4ffce5338a5df34a8fe90b</anchor>
      <arglist>(int key)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setModifierKey</name>
      <anchorfile>classCoreEvents.html</anchorfile>
      <anchor>23b79d36c28a51e77d3d5574a71bcfd6</anchor>
      <arglist>(int virtualKey, bool once)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>unsetModifier</name>
      <anchorfile>classCoreEvents.html</anchorfile>
      <anchor>a6a9873ad077307a3333f69709313db6</anchor>
      <arglist>(int virtualKey)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CoreEvents</name>
      <anchorfile>classCoreEvents.html</anchorfile>
      <anchor>0855d43101247faa18bd5e3405179dbc</anchor>
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
      <anchor>3772a1b20db8a4dc86cf6b9603a7ed93</anchor>
      <arglist>()</arglist>
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
    <member kind="variable" protection="protected">
      <type>QHash&lt; int, QString &gt;</type>
      <name>phonemes</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>6ee4b7aa48960e7cca843e76667b5253</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QHash&lt; int, QString &gt;</type>
      <name>modifiers</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>096c15926581c49e39cc231960bdd937</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QStringList</type>
      <name>words</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>9ec374990bf53addbe2242cf7697e99c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QStringList</type>
      <name>pronunciations</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>648057130953f8ddcae66f51bdd98e2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QStringList</type>
      <name>terminals</name>
      <anchorfile>classDict.html</anchorfile>
      <anchor>ced13b55e586dfdb96a8fc82f7f2564a</anchor>
      <arglist></arglist>
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
    <member kind="signal">
      <type>void</type>
      <name>droppedText</name>
      <anchorfile>classDropListWidget.html</anchorfile>
      <anchor>248e15da34634d258b7cb9abfaa4fbd6</anchor>
      <arglist>(QString text)</arglist>
    </member>
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
      <type></type>
      <name>EventHandler</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>8fe27b69582cce5c6a89a0b134bc8158</anchor>
      <arglist>()</arglist>
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
    <member kind="function">
      <type>void</type>
      <name>setModifier</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>15a70733660c001319f38720a184855c</anchor>
      <arglist>(int virtualKey, bool once)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unsetModifier</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>34a98363ca9794fd2db6e44cb1304213</anchor>
      <arglist>(int virtualKey)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendKeyCode</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>bff75095b37f21515fe70c3edb74aef8</anchor>
      <arglist>(int keycode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>runCommandKey</name>
      <anchorfile>classEventHandler.html</anchorfile>
      <anchor>6b5030122c9ae363dd12218789e0a9ef</anchor>
      <arglist>(int keycombination)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportDict</name>
    <filename>classImportDict.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>status</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>b2b9798d5410dca2cf427ef1893f96bb</anchor>
      <arglist>(QString)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>progress</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>ba91b089b59c2906196fe039a587e201</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>finished</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>721b5bcdd028d23d4e7f9f8d056729a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>opened</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>e76c6b29579ffa55d8f33d6e75c00e9e</anchor>
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
      <type>WordList *</type>
      <name>getWordList</name>
      <anchorfile>classImportDict.html</anchorfile>
      <anchor>58449b0b8676590170aef6063c1f2c09</anchor>
      <arglist>()</arglist>
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
      <anchor>e0574fa2a486e9a019589fc25386dc33</anchor>
      <arglist>(QString pathToDict)</arglist>
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
    <name>ImportDictView</name>
    <filename>classImportDictView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>importDict</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>65aa3482572b4b9d1c8237cea891efa2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>openFileDialog</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>189304275dcec6168b4a6def6784f0fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>finishedImporting</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>9febca17ccb05c54f20117c29b98b41b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>openingFinished</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>5e42470b3cd63b87285f6791984021d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>validatePath</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>54989c7d6e9b3c26b0cb8463ef8b9a45</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayStatus</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>f795f5cbaf6e95115807626a348b4da8</anchor>
      <arglist>(QString status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>displayProgress</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>d1e0957d3cc0de952afb90f07e6881bd</anchor>
      <arglist>(int progress)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportDictView</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>7fc799478e8fc1e41cee3eb94d781075</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>getList</name>
      <anchorfile>classImportDictView.html</anchorfile>
      <anchor>3c4502fd6b46a4878ad8de9ab6af6281</anchor>
      <arglist>()</arglist>
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
    <name>ImportLocalWizardPage</name>
    <filename>classImportLocalWizardPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setLocalSourceFile</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>5a45f085667b3f8187257603b6abd054</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportLocalWizardPage</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>a3348ec3ce4a61f8ea7b52cd86d26a9f</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPathEdit</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>978445e59ed48bb0b3910ee447acc421</anchor>
      <arglist>(QLineEdit *lePath)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerField</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>235161271fc877eb0d68e4f662005be9</anchor>
      <arglist>(const QString &amp;name, QWidget *widget, const char *property=0, const char *changedSignal=0)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>c3a2578e5c797a0620b975f3f7c2f6d0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>QVariant</type>
      <name>getField</name>
      <anchorfile>classImportLocalWizardPage.html</anchorfile>
      <anchor>2570721495eaaad07a2c842178180a9c</anchor>
      <arglist>(QString name)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportRemoteWizardPage</name>
    <filename>classImportRemoteWizardPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>fetchList</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>9cb0ebfac9ebedca15d5c473e67850bc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importList</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>e24b26dee5c39131be1c08f285348ff6</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportRemoteWizardPage</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>654c4eb84acbe8937c9dfbf7778f3f44</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setList</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>700eb491da695360531a4523d363e880</anchor>
      <arglist>(QListWidget *list)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerField</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>3536e60a1d338fb92cf16cbf44bc2faf</anchor>
      <arglist>(const QString &amp;name, QWidget *widget, const char *property=0, const char *changedSignal=0)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>e260d142f3033c47dcb5b4cf855b2e8a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getCurrentData</name>
      <anchorfile>classImportRemoteWizardPage.html</anchorfile>
      <anchor>21e9c7ce83f70fa4b4139faf87dd8f68</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ImportTrainingTexts</name>
    <filename>classImportTrainingTexts.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>idChanged</name>
      <anchorfile>classImportTrainingTexts.html</anchorfile>
      <anchor>b4f7eee43f2bf2b34783ac31fdd97a6b</anchor>
      <arglist>(int id)</arglist>
    </member>
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
    <name>ImportWorkingWizardPage</name>
    <filename>classImportWorkingWizardPage.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>startImport</name>
      <anchorfile>classImportWorkingWizardPage.html</anchorfile>
      <anchor>b63346433d3ff4b7b6e7c67187663926</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>processText</name>
      <anchorfile>classImportWorkingWizardPage.html</anchorfile>
      <anchor>87f4065450240f5c2884b21ce71ae391</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>parseFile</name>
      <anchorfile>classImportWorkingWizardPage.html</anchorfile>
      <anchor>b850662d36d0e53a676f696aa5bd7315</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ImportWorkingWizardPage</name>
      <anchorfile>classImportWorkingWizardPage.html</anchorfile>
      <anchor>24ce187ad8e2bba8c77fa83a32e75265</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>JuliusControl</name>
    <filename>classJuliusControl.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>recognised</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>6950bb3c775ab30777e973109de217e5</anchor>
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
      <name>disconnect</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>4a932cddda622c7cb34784b395560355</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>wordRecognised</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>d1e9ac191ba60edb54676723a17f4e4e</anchor>
      <arglist>(QString word)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>connected</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>6ef2325a9686ab4d584ed6b29728a17c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>disconnected</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>aea3fbe33ba99737bcd973185f7e73ea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>error</name>
      <anchorfile>classJuliusControl.html</anchorfile>
      <anchor>cbd812a26677526abda5cfaa257ce2dc</anchor>
      <arglist>(QString error)</arglist>
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
    <name>Logger</name>
    <filename>classLogger.html</filename>
    <member kind="function">
      <type></type>
      <name>Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>bc41bfb031d896170c7675fa96a6b30c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>cb668a9e186a25fbaad2e4af6d1ed00a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>init</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>0deec41c73d34a4f368dd08424953b46</anchor>
      <arglist>(QString path=&quot;log/simon.log&quot;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>dfb43f948970a1b123a143317f399318</anchor>
      <arglist>(QString message)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>close</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>8320f65e240047c9d08f72316310aaab</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ModelManager</name>
    <filename>classModelManager.html</filename>
    <member kind="function">
      <type></type>
      <name>ModelManager</name>
      <anchorfile>classModelManager.html</anchorfile>
      <anchor>9b0d22b1baf59c5b566e82fcd8ca424a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ModelManager</name>
      <anchorfile>classModelManager.html</anchorfile>
      <anchor>93f4eea50036cc5453014234aa921c7c</anchor>
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
      <anchor>b60b9c0f51371b190aeb2c5522f0e223</anchor>
      <arglist>(QString message, int timeout, int alignment=Qt::AlignBottom)</arglist>
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
    <name>QuickDownloader</name>
    <filename>classQuickDownloader.html</filename>
    <member kind="slot">
      <type>bool</type>
      <name>download</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>d48057c94c4190b4f54ebb454cb2cf6b</anchor>
      <arglist>(QString url)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>cancelDownload</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>57dcbd26b0ebbd9d296c3d8c55b06ea4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>startDownload</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>ed7936aa0c1956a11d1c5e10d593180c</anchor>
      <arglist>(QString url)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>progress</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>4e199015343a13866361eb2d4b9b5261</anchor>
      <arglist>(int now, int max)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>downloadFinished</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>55a25dbfd775c0ede0d6e5b4ffca0bc5</anchor>
      <arglist>(QString filename)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>errorOccured</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>5753cfc9c1d763a8528e480790d28fb1</anchor>
      <arglist>(QString)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>aborted</name>
      <anchorfile>classQuickDownloader.html</anchorfile>
      <anchor>92bf9dd1edb459b4d21507895b21826d</anchor>
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
    <member kind="signal">
      <type>void</type>
      <name>playing</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>29997c4d47749589c0eeede6095f3179</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>playBackStopped</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>d483eb28f76cdd1fcf17893425ee199d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>recording</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>983c8542488645c72aaf78d334300733</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>recordingStopped</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>3b3a911e15544ffaf38a855f5a4ab7d8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>sampleDeleted</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>43ce1a552c6f341550c25a5ec95f21bd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>progress</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>6238ec8375bf16099a31dba298588950</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>recordingFinished</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>c4049811568270c006636d414ba98413</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>playbackFinished</name>
      <anchorfile>classRecWidget.html</anchorfile>
      <anchor>0de6f148ae27e3040f5c77a6b1d6dc48</anchor>
      <arglist>()</arglist>
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
    <name>RtApi</name>
    <filename>classRtApi.html</filename>
    <member kind="enumeration">
      <name>StreamState</name>
      <anchor>9941bf8dc3ead0a3de58fd8823390da7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STREAM_STOPPED</name>
      <anchor>9941bf8dc3ead0a3de58fd8823390da73fde6860b0950833916151591fc5e388</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STREAM_RUNNING</name>
      <anchor>9941bf8dc3ead0a3de58fd8823390da798203a905c6a37d6b91c1f1746dad965</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtApi</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>bb8d6a34fdef926feea06ba0925e16bb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~RtApi</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>7880ecf5954c116aebcbe1ff6bb1c658</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>openStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>6992b4ef7bf1c130fab01fdb92ccffda</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int numberOfBuffers)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>openStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>833b75bb6904115e350f9f52cd801f75</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int *numberOfBuffers)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setStreamCallback</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>b3b41c5a881c1d29fbffc235a6e519c9</anchor>
      <arglist>(RtAudioCallback callback, void *userData)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>cancelStreamCallback</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>9c1f9f3dbbbcbbfc7b4e5a4d736f67b0</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getDeviceCount</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>8bf602c7775f6d007c1470a2298d2cf2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RtAudioDeviceInfo</type>
      <name>getDeviceInfo</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>3f54ae2bf2a00e864c907a8b20fa730b</anchor>
      <arglist>(int device)</arglist>
    </member>
    <member kind="function">
      <type>char *const</type>
      <name>getStreamBuffer</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>f5cb2cf19803e3375a5585f23ecdaa98</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>RtApi::StreamState</type>
      <name>getStreamState</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>ebea51cde09f74cd179283385288da44</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>tickStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>588fe4593bd5e5dc89ffa4fd467825a5</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>closeStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>b5aa7c4b05faf688b86675a2dd4cd49b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>startStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>c3b71a7218a48a19b580ec4811dd360b</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>stopStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>feaad31a4d21421fc6bdd63967c9fdde</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>abortStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>daf12640685f49ac725a0e67b33e0972</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="enumvalue">
      <name>FAILURE</name>
      <anchor>b988f69d75ee4ef5354c1c10d4822ad9b51a4532db173f4dc9202260fd08aaee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SUCCESS</name>
      <anchor>b988f69d75ee4ef5354c1c10d4822ad9653bd8f777a5f2a796cdc63aaec21c82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>StreamMode</name>
      <anchor>7df4fd666bdd1ec02cffd4921cbcbebd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OUTPUT</name>
      <anchor>7df4fd666bdd1ec02cffd4921cbcbebdeb2b4bed5a164cb6fdbefaf4208785cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INPUT</name>
      <anchor>7df4fd666bdd1ec02cffd4921cbcbebd8c798d6bca4bc1e040edf99735ece43e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DUPLEX</name>
      <anchor>7df4fd666bdd1ec02cffd4921cbcbebd30eca962e4abb8147d3b460ad065d757</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNINITIALIZED</name>
      <anchor>7df4fd666bdd1ec02cffd4921cbcbebde1206e4422d260aecd6740de4bf8d021</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>signed short</type>
      <name>Int16</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>a5a524a28ea28f031ca0a0a9e024ff39</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>signed int</type>
      <name>Int32</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>438fed76129128575398354b306a4cc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>float</type>
      <name>Float32</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>efb8b52708f6a43a62fcb31da11c0484</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>double</type>
      <name>Float64</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>e10c5228f8184b69c1102f5d90992f23</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>90742255c030825642e8ec1df710d694</anchor>
      <arglist>(void)=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>probeDeviceInfo</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>bc6072f1bf9b889bc8fb6a619178fe48</anchor>
      <arglist>(RtApiDevice *info)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>probeDeviceOpen</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>66cb4b6c44d1f6d2a09c05b908c46ee2</anchor>
      <arglist>(int device, StreamMode mode, int channels, int sampleRate, RtAudioFormat format, int *bufferSize, int numberOfBuffers)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual int</type>
      <name>getDefaultInputDevice</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>2b74b3a061769412e8153ef187f9a6ae</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual int</type>
      <name>getDefaultOutputDevice</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>0c1d02821fe8534d3602ba5578a6b341</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>clearDeviceInfo</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>dd4210be0af9e411040a9ccd97dab7d5</anchor>
      <arglist>(RtApiDevice *info)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>clearStreamInfo</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>db7ab7111510e3db1f0884a68926b5a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>error</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>80d75cb44a194e2f07807f3ab80961a4</anchor>
      <arglist>(RtError::Type type)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>verifyStream</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>ce999056ad9e6cab09711264f496da2b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>convertBuffer</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>85ed0a3827faddf0c796869e72800850</anchor>
      <arglist>(char *outBuffer, char *inBuffer, ConvertInfo &amp;info)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>byteSwapBuffer</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>42b7c5ea1cde90594600c1e01e65c914</anchor>
      <arglist>(char *buffer, int samples, RtAudioFormat format)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int</type>
      <name>formatBytes</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>26c1a3e8c9419f37d533e33899c99346</anchor>
      <arglist>(RtAudioFormat format)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>message_</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>3385e807000f161b515c264c28958635</anchor>
      <arglist>[1024]</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>nDevices_</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>d99494d6e72ca14696b640a349b3a80f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; RtApiDevice &gt;</type>
      <name>devices_</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>5c5d7bbdaca76f443607606a01ad66a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>RtApiStream</type>
      <name>stream_</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>818637ca9005b0270d56eaaea4697b34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const unsigned int</type>
      <name>MAX_SAMPLE_RATES</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>59917a16adcd5eb468dd88d4c5b8c289</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const unsigned int</type>
      <name>SAMPLE_RATES</name>
      <anchorfile>classRtApi.html</anchorfile>
      <anchor>84f89655b9f487005d10929ff9453ad2</anchor>
      <arglist>[]</arglist>
    </member>
    <class kind="struct">RtApi::ConvertInfo</class>
    <class kind="struct">RtApi::RtApiDevice</class>
    <class kind="struct">RtApi::RtApiStream</class>
  </compound>
  <compound kind="struct">
    <name>RtApi::ConvertInfo</name>
    <filename>structRtApi_1_1ConvertInfo.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>channels</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>e6fdb3e6b6a3a4974b49ded33291678b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>inJump</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>4af72ecf20214d857c3a8e6190230840</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>outJump</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>cb0920de8188e4453daf37a080ff78f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>inFormat</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>44d9cf8d7c26ff60fed5d0274cf6d81a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>outFormat</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>64970c900e1649a386f3df8254ef4aec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; int &gt;</type>
      <name>inOffset</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>5e3b0a93cd8bfbf62e275cf4de22bcc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; int &gt;</type>
      <name>outOffset</name>
      <anchorfile>structRtApi_1_1ConvertInfo.html</anchorfile>
      <anchor>fe1baae9554f14df0f5f90c3f2beadaa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtApi::RtApiDevice</name>
    <filename>structRtApi_1_1RtApiDevice.html</filename>
    <member kind="function">
      <type></type>
      <name>RtApiDevice</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>c93a512a8c7f5f0fddcf706d3c6680ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>name</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>db1c8e41c2ffb628d4147c7d3c30c637</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>probed</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>59142a7ffc965c9eab14c9a4ad689a8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>apiDeviceId</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>0188940a7184ea7f6fb9e249bb4aa035</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>maxOutputChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>26bcac8e7f928d7e8f0d23be141a2b97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>maxInputChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>5b98da1a9943c1584f702c7ea721b2b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>maxDuplexChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>c2bbc015dffca8a5ea17d7c76345678a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>minOutputChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>2c80bdf8a28ced9f7d365fe72346df99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>minInputChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>ca7aadf959e15ff0961512df0e41c8c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>minDuplexChannels</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>d06ce331986d9ecd1e69d257c491e7f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>hasDuplexSupport</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>bf22e950716a65d37a5595252b1eefe8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isDefault</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>2b8dd432ed4faa4befe7b9bda6bc60e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; int &gt;</type>
      <name>sampleRates</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>40e36ec975fc4e96a46ef3fabc86c22b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>nativeFormats</name>
      <anchorfile>structRtApi_1_1RtApiDevice.html</anchorfile>
      <anchor>b53f0c8f6d8d6a886c87d19f61d9919a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtApi::RtApiStream</name>
    <filename>structRtApi_1_1RtApiStream.html</filename>
    <member kind="function">
      <type></type>
      <name>RtApiStream</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>e40149669eaecbdbdc3092e74fb2ecfb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>device</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>361bf7a4ceb9e20477a9e8d8dd74da24</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>apiHandle</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>7e3a1a42bb786be6dd4085540a56e49f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>StreamMode</type>
      <name>mode</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>5989e685a5bbe545705a962caa66c259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>StreamState</type>
      <name>state</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>ccf19137332f916e96dd35f0f4259697</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>userBuffer</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>62e7f66867443ed14dfe7093e3423d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>deviceBuffer</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>253293c901cfb48e3d7a7abb76ea80b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>doConvertBuffer</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>5ea1c0ba6fc1f41360e8c7f759078b00</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>deInterleave</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>d2af98b589bec80ee82c3571814e7b9d</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>doByteSwap</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>5a610c6c9631b68bba44493f138fd876</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>sampleRate</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>595cc34c91f829f7cf16c3faf4c87647</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>bufferSize</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>26396d10ef21ed25917f752a8462d910</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nBuffers</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>9d99b709ad0ed290836c77f0b3e24cf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nUserChannels</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>8e47f906176c6e6f68350c45d14046c3</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nDeviceChannels</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>8f6d5e889cb6bf3f6a563bc09106641a</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>userFormat</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>dc6fab24ece3a7e0318f77d687f90866</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>deviceFormat</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>9eb0511e02ce60efb17fa6725fd4bea3</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>StreamMutex</type>
      <name>mutex</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>8b7f7b5ada995d6885a76ac09fdc0a58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CallbackInfo</type>
      <name>callbackInfo</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>b6194fc15a14da5667c493502380ec6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ConvertInfo</type>
      <name>convertInfo</name>
      <anchorfile>structRtApi_1_1RtApiStream.html</anchorfile>
      <anchor>28b8a898ae50d607976b8d7c56422c52</anchor>
      <arglist>[2]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RtAudio</name>
    <filename>classRtAudio.html</filename>
    <member kind="enumeration">
      <name>RtAudioApi</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f883</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNSPECIFIED</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f883a49af3ac767106158bc6cad74fb214ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_ALSA</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f883a7a2ba095c2806caa893b6fb8fc3a1a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_OSS</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f88339c84615e61de75e6a4e865596d62c82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_JACK</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f88348b58e4b8c244b1b2dfeb8ba1cdeae2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MACOSX_CORE</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f8839fec92b8272244a87a6a11a717bd662d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IRIX_AL</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f88388dd3ffa0044d120ee87a5c7b62cfdd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WINDOWS_ASIO</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f883219517d1df90f94d07758481155bd469</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WINDOWS_DS</name>
      <anchor>26ddd1d27d18273ca7aba8a3ffc9f88328478830f42c2fd61e6c7ad498901931</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>d581cdf6170b503cc53fe58ebc04a954</anchor>
      <arglist>(RtAudioApi api=UNSPECIFIED)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>64c24540d80d6baea28ef0c29cfc911c</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int numberOfBuffers, RtAudioApi api=UNSPECIFIED)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>10958ea9881eedb8f3c46ae7d58b7cb0</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int *numberOfBuffers, RtAudioApi api=UNSPECIFIED)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RtAudio</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>a8a9716fd64680657ef69c9465442a2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>openStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>a424831e2b1b8a3fa6ca4520e50c02d7</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int numberOfBuffers)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>openStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>f316981a2dd8f1c669730d59626b953a</anchor>
      <arglist>(int outputDevice, int outputChannels, int inputDevice, int inputChannels, RtAudioFormat format, int sampleRate, int *bufferSize, int *numberOfBuffers)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setStreamCallback</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>7c3d61c56adb8d340b8432c198b9d0ae</anchor>
      <arglist>(RtAudioCallback callback, void *userData)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cancelStreamCallback</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>6542507ccb339383d2f230b5cfda7304</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getDeviceCount</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>bd9795153ecf7c4542b388f7672fb93f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RtAudioDeviceInfo</type>
      <name>getDeviceInfo</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>b7d4e9d8fd2f2c824d603aae561da4f0</anchor>
      <arglist>(int device)</arglist>
    </member>
    <member kind="function">
      <type>char *const</type>
      <name>getStreamBuffer</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>80ca7a7d7eb0e2c34ec2d79f5b3e7436</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tickStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>e969c74d190e2cb940df30acc45d6c68</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>closeStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>6b326667416c552ef406931c7fe2452d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>c992c2cb690b7eedd2f4cec362feaf46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stopStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>7f61050750bdf99058b7df6cebbaa074</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>abortStream</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>9cac2544404188278d8238bb43586c51</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>6f35b025d11977bb0a5dce2b303faf5a</anchor>
      <arglist>(RtAudioApi api)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>RtApi *</type>
      <name>rtapi_</name>
      <anchorfile>classRtAudio.html</anchorfile>
      <anchor>b2a64255e2563d501c1318dd9b60f471</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RtAudioDeviceInfo</name>
    <filename>structRtAudioDeviceInfo.html</filename>
    <member kind="function">
      <type></type>
      <name>RtAudioDeviceInfo</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>a92302dc487e824e5291b2c9a9ef480a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>name</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>51916baf692cca05e467a09db901e867</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>probed</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>19f5d8d476c2c27a0c801bc87c47de2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>outputChannels</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>de0059514d7b5ddfa42b05a3709126ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>inputChannels</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>0371be55c12d8c0d20b2e2ca9757fa3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>duplexChannels</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>93a79db4ff2c0912a0487bddd775451a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isDefault</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>29e349a64f35f7f6576be0ca8fb13de8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; int &gt;</type>
      <name>sampleRates</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>24cff847f60035715f07163512e129c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RtAudioFormat</type>
      <name>nativeFormats</name>
      <anchorfile>structRtAudioDeviceInfo.html</anchorfile>
      <anchor>866a41581d182648feaaaaa622b43fc3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RtError</name>
    <filename>classRtError.html</filename>
    <member kind="enumeration">
      <name>Type</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WARNING</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac3a1603c24a56cbdaf5f8ae4ddcb86398</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DEBUG_WARNING</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac20e5a369394d19b704ace17002007eba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNSPECIFIED</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac444efd3ccf774b2ba9f9bde70ec71cd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NO_DEVICES_FOUND</name>
      <anchor>b04667aae01bffc354a9ac6bda6903acf267eb2d710f33d649ba840eeab6ff82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_DEVICE</name>
      <anchor>b04667aae01bffc354a9ac6bda6903acd6064062066fffdba258237a7c2159b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_STREAM</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ace49e244b799da3bed7b327dbed4bee85</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MEMORY_ERROR</name>
      <anchor>b04667aae01bffc354a9ac6bda6903acb4f813e5a36905c89d4081a59497432e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INVALID_PARAMETER</name>
      <anchor>b04667aae01bffc354a9ac6bda6903aca3eabf0f71120beaba94148a1b78fed6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DRIVER_ERROR</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac995e97ecf6beeedaba525022a63aec6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SYSTEM_ERROR</name>
      <anchor>b04667aae01bffc354a9ac6bda6903ac8947d0a2e2a84ecf3646271844e06a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>THREAD_ERROR</name>
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
      <name>getMessageString</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>45783a02453ae08821d0fb531d6b86b5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>message_</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>6f63960ea24c5281cddd244d053d479c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Type</type>
      <name>type_</name>
      <anchorfile>classRtError.html</anchorfile>
      <anchor>b4f7a5e085eb9fce8e1d264944e53ab3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RunApplicationView</name>
    <filename>classRunApplicationView.html</filename>
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
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classRunBackend.html</anchorfile>
      <anchor>5ac8a9d78e2e6bae4f616827a7f4af55</anchor>
      <arglist>(QString command)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>goTo</name>
      <anchorfile>classRunBackend.html</anchorfile>
      <anchor>99ec20abebb7882a80752ebc69c2dbac</anchor>
      <arglist>(QString place)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~RunBackend</name>
      <anchorfile>classRunBackend.html</anchorfile>
      <anchor>73c3db7abb1290a01f8ff58dddd5788e</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RunCommand</name>
    <filename>classRunCommand.html</filename>
    <member kind="function">
      <type></type>
      <name>RunCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>3c98a4f5e6cc5c933a8d03cd6bda3344</anchor>
      <arglist>(QString path=&quot;commands.xml&quot;)</arglist>
    </member>
    <member kind="function">
      <type>CommandList</type>
      <name>getCommands</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>3f66602a84d646dc86d5ab4f3d4e1193</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>readCommands</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>f9d0e88c34fed149c5ec939bb09ae293</anchor>
      <arglist>(QString path)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>e4f54e2867cdd9af04edae9361b4bcc0</anchor>
      <arglist>(QString commandName)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~RunCommand</name>
      <anchorfile>classRunCommand.html</anchorfile>
      <anchor>9bd0ead049d3a608c8a2acaae43ce634</anchor>
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
      <anchor>9d06067c589f67cb8c947ae8b6a778c9</anchor>
      <arglist>(QString command)</arglist>
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
      <anchor>3b4eb784456812e358c79d8e031deb0b</anchor>
      <arglist>(QString command)</arglist>
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
    <name>SelectSourceWizardPage</name>
    <filename>classSelectSourceWizardPage.html</filename>
    <member kind="function">
      <type></type>
      <name>SelectSourceWizardPage</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>9929cfe8a68fd7fe2b4ac55264bc593b</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nextId</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>1088d58a55342397b90690865d98dc1d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>QRadioButton *</type>
      <name>getLocal</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>01be75f52a163a6e2715448e8ef504fd</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>QRadioButton *</type>
      <name>getRemote</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>96d1d3bb8015cc25011944d6b8c64b13</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLocal</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>0a97ebcd4c7c1d089f46abc5b7559390</anchor>
      <arglist>(QRadioButton *local)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRemote</name>
      <anchorfile>classSelectSourceWizardPage.html</anchorfile>
      <anchor>00f44b6d1c3d13e86c479cc66fdd3ba9</anchor>
      <arglist>(QRadioButton *remote)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SettingsView</name>
    <filename>classSettingsView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>unsetAllTabs</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>99cadf248f2cc973084c5b0940a4b257</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>switchToSystem</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>487f2341fc88cc925ecb1f8ca6a6e8fd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>switchToSound</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>fb1e5539f3633164962f51c7adbd9692</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>switchToCommands</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>c6f2a13e9d2478104e74af4d0894f0b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>switchToProtocols</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>03099bb326cff6322096e8b2c984545a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>switchToHistory</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>06799e750a0d02d63ba6fcb52e31c8a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>apply</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>0547766333ed5e0e813ced6e51586736</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>refreshDeviceCapabilities</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>69456494eda33d25958d3de8c0bed5fd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>deleteAddress</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>521f0784b11a75399400916e36e1d038</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>addAddress</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>a93ec7db4400fa6a023d2c5562182f3c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SettingsView</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>3aa347797fd7f79057881be84d7f4c4b</anchor>
      <arglist>(QWidget *parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SettingsView</name>
      <anchorfile>classSettingsView.html</anchorfile>
      <anchor>3287e7c87008472a93bbc10fe331b6c2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SimonControl</name>
    <filename>classSimonControl.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>connect</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>d5b1c9b9f023d41a27665878c9a70214</anchor>
      <arglist>(QString host)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disconnect</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>eabf24aacd8a21e8a1451eeb8e32c5a4</anchor>
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
      <anchor>8eac632aa9c584fc282e323343354518</anchor>
      <arglist>(QString word)</arglist>
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
    <member kind="signal">
      <type>void</type>
      <name>connected</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>d81a62a49825b10e244e22ffcccbef37</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>disconnected</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>c8abf11c6c227ba51b6da22c39967a25</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>connectionError</name>
      <anchorfile>classSimonControl.html</anchorfile>
      <anchor>82d9da51480c11e96ce83b83674bfe11</anchor>
      <arglist>(QString error)</arglist>
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
    <name>SimonView</name>
    <filename>classSimonView.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setLevel</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>49a534b0edf83e6c40245023bf5aca6a</anchor>
      <arglist>(int level)</arglist>
    </member>
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
      <anchor>917bb70e833ca8ee65baca971e2d102c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showRunDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>ca4a7aceec4df1902d66a35f4a6c8ddf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showTrainDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>21a9ef4c7b09c8ce280699167ead20a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showWordListDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>ef4246903c2aa5cf7b48db39b01ba147</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showSettingsDialog</name>
      <anchorfile>classSimonView.html</anchorfile>
      <anchor>268254badb0dce837cada50481fc2c61</anchor>
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
      <type>QList&lt; int &gt; *</type>
      <name>getSamplerate</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>3c0f3e72454220d11181beccf2af1cb1</anchor>
      <arglist>(QString id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getChannel</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>0b8d5b328ae643d0bc15ded9d3fe617b</anchor>
      <arglist>(QString id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getVolume</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>cefd40e725b3bb6b1f5f63e4ab81e4d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setVolume</name>
      <anchorfile>classSoundControl.html</anchorfile>
      <anchor>c83c0a05e45a9d8adf3941e959ebdcee</anchor>
      <arglist>(int percent)</arglist>
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
    <member kind="function">
      <type></type>
      <name>SoundDevice</name>
      <anchorfile>classSoundDevice.html</anchorfile>
      <anchor>a07bfbe692ea268436b35451db963407</anchor>
      <arglist>(QString deviceID, QString name)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getDeviceID</name>
      <anchorfile>classSoundDevice.html</anchorfile>
      <anchor>397e00d04c658fd477ba980bb0ceb261</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getName</name>
      <anchorfile>classSoundDevice.html</anchorfile>
      <anchor>5a2ccbf333e5d08d208885c44e7310ee</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SoundDevice</name>
      <anchorfile>classSoundDevice.html</anchorfile>
      <anchor>ac25582f3f92e4d5ee732001f4e05c5f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrainingManager</name>
    <filename>classTrainingManager.html</filename>
    <member kind="function">
      <type></type>
      <name>TrainingManager</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>da596679dcf606009ada12e4e4464c95</anchor>
      <arglist>(QString pathToTexts=&quot;texts/&quot;)</arglist>
    </member>
    <member kind="function">
      <type>TrainingList *</type>
      <name>readTrainingTexts</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>48a81f64ee4d0ecfca0dc495aea2a251</anchor>
      <arglist>(QString pathToTexts=&quot;texts/&quot;)</arglist>
    </member>
    <member kind="function">
      <type>TrainingList *</type>
      <name>getList</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>6ed1d6e5a54eb902049faca1d910101d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getLabel</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>3d203970496bc5407942bef1885d73a9</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deleteText</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>4ef23143cb83f67b14a8e47d026a5c45</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>calcRelevance</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>86cb50f9e103190dccd8e12adeb4e25e</anchor>
      <arglist>(TrainingText *text, WordList *wlist)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trainWords</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>085160d7f74f9a89ff9a3de0e8803fe1</anchor>
      <arglist>(WordList *words)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>abortTraining</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>5fd2847ba008bbf0b024ca4807f8f236</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pauseTraining</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>b469e8558582021e385cf203aeeca05a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resumeTraining</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>bf9fa899628816b39c8a5ad99d9f1a15</anchor>
      <arglist>()</arglist>
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
      <type></type>
      <name>~TrainingManager</name>
      <anchorfile>classTrainingManager.html</anchorfile>
      <anchor>c47942d070449a9838d06af987a75f7b</anchor>
      <arglist>()</arglist>
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
      <type></type>
      <name>TrainingText</name>
      <anchorfile>classTrainingText.html</anchorfile>
      <anchor>bb429323a177637522f348498f17a362</anchor>
      <arglist>(QString name, QString path, QStringList pages, float relevance)</arglist>
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
    <member kind="slot">
      <type>void</type>
      <name>switchToWordList</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>4201ae338dc14c3eb9b4feb8a7640d5e</anchor>
      <arglist>()</arglist>
    </member>
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
    <member kind="function">
      <type></type>
      <name>TrainingView</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>a5f6a721f79a252d198e2da58e50d1b9</anchor>
      <arglist>(QWidget *parent=0, WordList *trainWords=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TrainingView</name>
      <anchorfile>classTrainingView.html</anchorfile>
      <anchor>f00ed658a3724584c80f95c0fad2fe8c</anchor>
      <arglist>()</arglist>
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
    <name>Ui_AddWord</name>
    <filename>classUi__AddWord.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>67abcdddd384da99d3400c01763cf8eb</anchor>
      <arglist>(QDialog *AddWord)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>a8aee48cc5c776920e9aecc6b8f268ba</anchor>
      <arglist>(QDialog *AddWord)</arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>f6d75f4e7958f251eb405c66a84b694d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QStackedWidget *</type>
      <name>swMain</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>9a10a325f5d19a3e4adb9cc2cb362408</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>eda437d489e61c71dd075682e9b8e3fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>dca95f23a4172746bf83c7dca0e71d86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>a9cb11e78497d3478a26e6596b0c6eff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>340ae9fc6326fc4a7b72da21ad355cd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leWord</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>f1b3c5fab97d2db13c09170dcaaba19a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>efa8f537189aad20e6cbcc6c114c3f3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>3a7cdbf4651231392b8871d2541ecc39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>9070fc0f59b3340433d4be826a127a0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbNext</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>78a598b93ba3fef2c5c0e3fd6890bb25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>faac43726bdbcc8ba782f4717c400495</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>745563c8bd33906812a3f3c88cfedbbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_3</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>b8325bb876cfb3b7904cac611ddc7351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>wRec1</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>61f779af2c0895bcc37d7f588dcc3b57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>ebf018310037096a006355a6b0a32c88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>wRec2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>372e76329356b354400c55b521b5e82f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout3</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>bf10db8e2b23462e1e767195759ea3c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout4</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>fa03b719f33e6491b3e9d9b5433ae4db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack_2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>54e39f2c64be1913aa2a9dcf4e953158</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbNext_2</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>5dd8eb187aa7fade485270ab5d5c2aa3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_3</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>d7a3aa2124e434aa0f0ac64b17cf7a3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>f6c32a89e0b7257217aba30880c760d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_4</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>3310460dd5e4f0f684d562db2c4f7916</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout5</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>3d5a80ca15c6e2ffa33a7ae4076b3a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack_3</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>9500fbcebfeac5e49b194cda66fa9147</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbFinish</name>
      <anchorfile>classUi__AddWord.html</anchorfile>
      <anchor>ae27721e35228fc107494e8823139095</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_ImportView</name>
    <filename>classUi__ImportView.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>f287079845174518a8f799fb8d3c0570</anchor>
      <arglist>(QDialog *ImportView)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>7cc2f7e1d2034bef912c58776778462c</anchor>
      <arglist>(QDialog *ImportView)</arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>8ce65d5eaaf30167644f7005d561524a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QStackedWidget *</type>
      <name>swMain</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>08816a61d94df6d99e97f1e07b9bd708</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>68b54653408fc60fc6a946d5b72b608f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>a5249326978ac823e5c020cf1a0d4c6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbSource</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>fbd82404cb97280f8c98d286e15e456d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>b47d3b637918da23678c0f520d16baed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>a3e6692b87536dd7d11b96927f73a56d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>lePath</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>deb6debccdf4d52c8287bf4fc15db2b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbGetPath</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>136cd8f90d662820f2d1700dcdf201fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>cb446503e4c29774fff2b2cfdbb37f3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>1c967d71532d457a769ac86aaba97c7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbCancel</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>e7264ecc6ca66ad3d7730bca68a127c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbImport</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>0e3a4fb1ba3ba3edf3d3be0d2a035fa1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_2</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>54145202f3726403ee016bdbc6c8db40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>41a2362b0a0394aed93a044aa87d7ce9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbImporting</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>3dbdb543287cf91057c6c6c95fbc5d6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>8888740a4449def593e116e6945b5f3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>66188377cad4478e7ae566a0a1f0fd4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbProgress</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>12cab8f31629156313d95f0038e91cd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbStatus</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>f87d3f53a917e75afbf07515bc65d64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem1</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>9008ab253c6ccf2f35c40067a15a468f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_2</name>
      <anchorfile>classUi__ImportView.html</anchorfile>
      <anchor>4cd45f96d0cfa99fa840d350fb07c856</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_MainWindow</name>
    <filename>classUi__MainWindow.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>cf4a0872c4c77d8f43a2ec66ed849b58</anchor>
      <arglist>(QMainWindow *MainWindow)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>097dd160c3534a204904cb374412c618</anchor>
      <arglist>(QMainWindow *MainWindow)</arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>centralwidget</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>356f1cf3ebda15f1fac59467ee081b74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>31d285ec7c66c071800632d96d856ca4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbLevel1</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>0f48a0c66fb61e4496489eb47288a320</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>f00b077691d1f8fac3fb2ad4a78b55af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>3260f8759d997f75cf8450d3728639b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>0a5a37019e8a14581803c0b4a813f0c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem1</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>7055e4138a16d2c84a79a22854fe85a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbConnect</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>56e2a9004b1d4ee7db2fba6d6d3dcee1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem2</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>709f772da77d3a8c237094b428d1fa9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbLogo</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>a488cd5e51ca77db2c8a7db48097f4d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem3</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>6ae1bfb8cff3d940521ec8f82e539ebd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>03c1c791729c9bad03c14ed130485fe8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem4</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>28d4994acfd29bb9e2780043fa38af7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>3331cd38e502a816227773f4049f70b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem5</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>787fdb4414ff7b88ab8f71790174a899</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAddWord</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>a55a9d131000e7a885f562159e8b4ea6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem6</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>bf6178a22625443b41ca00f51607a4b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbEditWordList</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>d0700e6dd4db5bf2b1aa21a525113dc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem7</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>b68e704ce322d036ac2347d5e7935f44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>66b59c46be5cc6b26d0b0b06e14320bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem8</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>5934fd1d9544b5e07b71c889a8fa005c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>frmConnecting</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>40e40a4e95a50555e849a1ed59f1215d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>c715bfc8bd8ca66f880c0aa785ffbd70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>d9c89133780f28e6efa2ec17ceb9cff5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>progressBar</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>21cff38a2f7c1fbae6334c65bc92dd65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbCancelConnect</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>519f422a4b8dc83eb822df22b89fef80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem9</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>1e681ef3a9e00e648ca06a41a80be521</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout4</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>de961bc49b3a73f75a986e9ee82980f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem10</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>fce0b7cf32bf336efa10f13b6b773381</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbTrain</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>debe9786ba5a6d8ebaf088d8fe6ae751</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem11</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>f2cd6736c20a9def2046fb2ab8aea7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbRunProgram</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>fddab549ca8e1a5662fe9ca5f3c93ee6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem12</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>52a05e38dcd7571a9e043a85d6d2383e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem13</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>606e76fd0d56ecd8f88ff28bc7d74f72</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem14</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>81f2c1e74f9c75264253deae5dde8388</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout3</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>9df21ad129c0ab1c97c8d757bad28ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbSettings</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>513cdb384471b1bd624a127b80d2c026</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem15</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>738c73d005c1893dc74a33d0117c72bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbActivision</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>80c57895a875b435a5428b9477bd925e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbClose</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>bf3e50ae65736f775a4c0657aea684dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem16</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>e07252e043f2930a69b193d10f5fdbe6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbHide</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>83eece1fe0941d9a4f657e335dcc36f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem17</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>ac39c6d02fda561831932786326fae59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbLevel2</name>
      <anchorfile>classUi__MainWindow.html</anchorfile>
      <anchor>242509b0d9ebf5ed825b89f1b9049009</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_RunDialog</name>
    <filename>classUi__RunDialog.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>d12028ae8d0300e6b5f4c2a0daa3a989</anchor>
      <arglist>(QDialog *RunDialog)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>bcad95b8ac18d97a7331334555654930</anchor>
      <arglist>(QDialog *RunDialog)</arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>c74bb342156ed5a51db253d1939c71f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>a59b2e901832cceeb6be82f573722b5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSearch</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>72bba2d9a8f979ec3e3522ae7788412b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSearch</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>c9a81f890051efe3cbe62aa3acba373f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbClearSearch</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>81dbb089b75f7c2a227e0410dfa5b3cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbShow</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>90403cf3f85c0a424878a5a538988271</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbShow</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>0e9a634560214d6118c15d476e194d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTableWidget *</type>
      <name>twPrograms</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>1f1b30518eefa30f18645ded7c6741d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>0628f0f5883f4e19eccbc4fa738c5095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>a3a9f9649ae2a2458730399ff1c4a6d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>7447eeb59375558a5fea4b54d342b186</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbRun</name>
      <anchorfile>classUi__RunDialog.html</anchorfile>
      <anchor>a7804e321813fb63407cd6e1f182172b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_SettingsDialog</name>
    <filename>classUi__SettingsDialog.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>4666ab89748eeb11cdb95d857bd02c6e</anchor>
      <arglist>(QDialog *SettingsDialog)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7a0adf32eef516ceffcc0633a90c3b34</anchor>
      <arglist>(QDialog *SettingsDialog)</arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>20b850cc7fe12e1e1fc41b299aead260</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>frame</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>befa115f8d0614521108da84f6253a6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6b7668acf4f2047fce467e549252d64f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>94daa640bbfbc565df646a88f530547a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>146705f30489627f6a84b007dbed3820</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>4c1656308d95e5de072218d3a71d97f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSystemSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1b64b5cf04ebbb2487fd445b0640031d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem1</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6aa6332e00194788b4062ce3da1eefee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbSystemSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>79e54af19fa5f5581f77da952b22a69b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f8d044fa97b6c3fb89d881437f581c3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f0f6be996388ff4ea57b59f211912593</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>14258445047f0af133f24efd340ef04a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSoundSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3fc5e55e3c0d02e313498ba7df452039</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>523751a769120bd425d0c4421de5374d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbSoundSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7bdf02925f2ef40a47a2dd72670d77ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>29b3f10a7e23b4f4569dc625e61a1dc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>00beab269c526867f3efafb47e7beaa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c91d25bdb0f14cc7dda8e1a96cc1c976</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbCommands</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>5149a8efa866be7133be369709f2aa6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>398a10158d19a352be5d169ad45b86b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbCommandSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d9492a8027908ae523f5dd4caa46e6f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>06afc8517a12dc2d7447eec53eb3776f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d6c49d213593525f59688ed09c5f69af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem6</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>5e8eb2adb91a2377ca82364396f59479</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbProtocol</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6a534908caa5bfaeb059d89ab85b275e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem7</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e14cf3d2e36ff03ec5fb8c9a9a6ad835</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbProtocolSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>853f795ec7fce6c8213441a70cf6da20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>df47210fdc7429c1883bba4ce3f536a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>94a33406bdd834d0d3e41887d33a6615</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem8</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>57cebc189dbf8179c8d5fde35abce284</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbRevert</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6055c6edc68e0271876cedad6142e618</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem9</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2e197e6c435ee0a7ae51462d2f217cef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbRevert</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a7259d5d2d798c05a8c0f18379fa853b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QStackedWidget *</type>
      <name>swSettings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a71a274554ebc73fd4e53b078bc14fd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0564d24ea6314b9594e5f9429bbb84f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout6</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3487055ce418ddf65f4e9963a7a486d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGridLayout *</type>
      <name>gridLayout</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>07d152c7cc63e2cffdda613baea54c0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem10</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>b0289bbb764774d1e5ccdecf78aa5f81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem11</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d9a5b0ae812bc88782622202fd134824</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbGrammar</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2448aaa4030b2587604f4f352b747db3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem12</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2b61d54e96dbfae03ed11507f297188f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem13</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d131ee73fcb2ad4a2da9eb08e644cc30</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbGeneralSystem</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7d1531c20454d8f9ebc25175782c32d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbStartJuliusdOnBoot</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d92380d193bafeb6ca1feee3b331283e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbAskBeforeExit</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a710d6d8fb4b3985c66d0b95e8787285</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leVocab</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1a3bcf9697d9cf4ad3d9016fc0e431af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem14</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>159cf215db10561cc84331fce0e277b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem15</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e79697b122ee70ec2192705c79c25eb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem16</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3e5d0e7801033d3ab638767577e70b01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem17</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>5c7775e95b51d541bf235a4c349a91a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem18</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0ddc42613dba8aa154ea2362ceb65ce8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem19</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e6004badb7089b3fe1b0bd10ba4f0fed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem20</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>41801e078cae8266aa34fd8bc1a81c69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem21</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2be0b36e4f18cb51d1e5e7c86c2f6b56</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbJuliusdAddr</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a62aa44c4e14ea861dfea463b506a1e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem22</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>018300d6ba8aae7a1d2a3a4d6b7a0265</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem23</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2f954330c0f5cc5238aede53b93b783f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbCommands_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6834d266b02c79c63eacca6a59670239</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem24</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>17bf4694d314e3fa3a27e81ab9270ad5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem25</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>14e139765fe5ea1a466e1239e6f4fa73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem26</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6a3e659ccdf187433b38de0cc231bd11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>lePrompts</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e6a3ff3ea50486213d66cde98075e584</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem27</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>4d660331768cd64189e87163686090fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbJuliusd</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c877ace2d471ec62536e44c7882bb024</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem28</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c1f594da4d2249a6a99125988ecc2854</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem29</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>342c1ee22babcdd8dc8730969c32c2f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem30</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>90432c4cb579bf93d9490687d8e0d649</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem31</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>9f320957d98df191207b74e2f1ba9431</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem32</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d8db6d69c40a87174739d6614aaeade1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem33</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>ca0a1f1c8a9d363310bab4e3ad3ea1b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leCommands</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>91445c30af0bf79007fd48f3a68443ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem34</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>10a3033670dc11335edbd11f47fd225a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem35</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>9868e85c084161b95227b984fd1375ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem36</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f75bb4852f9e10e04deedb46e65690af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbStartSimonOnBoot</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3303471872002a327523e56e5381d5ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem37</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>fac1b3d2b5b11993f9b56655e6c426f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem38</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6e3d4ece709200a4407146b4841924fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbPrompts</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>38c79fd30a74a98ba0f8ab9a6ca539ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem39</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2ef13414b25d0e9ccddc16130e477006</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbStartJuliusAsNeeded</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1c472ba7ce45d0a99a7bb313224c72ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem40</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3ed75f18cc7457926182b6a6e4760ae2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem41</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>187b693e4a109215121ece3da71e5c34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leGrammar</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>000d374fd9b71e558dd246e83acbc04e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbVocab</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>5ce8407da00aba0e08aae21a803b0902</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem42</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6fbcd061174d468982d74c96426ed252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leLexicon</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>ba3d2f6006bfbc89aa6d02a8f7e15132</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem43</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a797b7919b24ebfa212b81684ff56e3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem44</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2dd62f453d13e0c38316d17a0f5719b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem45</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8d0c701a3d4b4816df35b2015e68f011</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem46</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>14b6c813fbab28c31bb3da7265eb8ff3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem47</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>64490d6d77be778ddc7f5f4dd40addc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbAutostart</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>366e0b2374b3f68de02a4e29bdf8d762</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbFiles</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7a3e10090b9768d8c94da13fdebc584a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbLexicon</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2ae4bd0a0733f1cbb68d6bc0cb2eda5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem48</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>28a6ebe048cd93f231f4cf9ad08419dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem49</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>12e2ffeeac53ac4e9b50f64bb94d4fae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout7</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8dee4ab1e34fee424a83fae2e591676f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbAddress</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>5deeb1ea36c2a448babd18b71925c46d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbDeleteAdress</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>092b3b6e27590bfdffc9acc52602ebd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAddAdress</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>46fadee58e716cf47fb6c585881e4e49</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>liSep1</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>4faf9d2d4ba0642c8a0dd50043bde205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbHelpSystem</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c42d6c6ca2ed5f97e8061e1ad8d25562</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0bfbe0e225dbbbee97fa0a1dd19511ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout8</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2792d7073905e4f8eba6bcbfe66e1b5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout6</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>169adcb6a280d706fc3a15d215114ebf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout9</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0f0875a583ccb74f9d2a606aa5171d11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGridLayout *</type>
      <name>gridLayout1</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>35bd5c01f071107f41d8b963307a80a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbInDevice</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a077f41754e221ee7d51c255e9f8f3c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbInDevice</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>9fa90822d1ce1c7948b9653ff78a42c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbOutDevice</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8ae4f770b8adcfb3075d7b8ae51b4570</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbOutDevice</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>65c2ed43805ff944e2a65ef0d8948fbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>widget</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>67c1bdc500da8b0f8bd6aa4d7273bc25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbDirectX</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7d057fad0e6e414ddcf37fa623f9a6f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbALSA</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>262d6b257502bd369acfec5884e3686b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout10</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>251c6291d74f695cb8eac7949f0a5fa1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>groupBox</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8d7174927b227d4e77de14db04993b32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout7</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>51d5a2fb81b663885afcf238105b85d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbSaveAllRecordings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c62849a853df1ef32d40b72d2e95a869</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout11</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>602a48bc17f38211cae8cc9a13612d27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSaveRecordingsTo</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d54d03e3fd37941105e73e77726b3102</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSaveRecordingsTo</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d67f7740baf2851366c18bc436a7dbaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGridLayout *</type>
      <name>gridLayout2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>901ce0899a68ae77acb73d7acd4b5952</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbChannels</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d02993008410c7c89f2eb6a577c8066a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbChannels</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c2f32c35e2b505fc9ad595e923b4b374</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSamplerate</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d8395e1354cc2267e22a451b91641476</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbSampleRate</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>9c40d7565eca12ea6c8b22789bb279df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbCalibrate</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a6eb2052bd007424d565118b40e978d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout8</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7d96ea329c3275245c92bed8b5ca506a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout12</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a9802263252d69fd2a15e27965eb72ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem50</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6cbcd3c7d02d9824069bb0bec94fbd83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAutoCalibrate</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0a4be72906ce358dc946660b4337aa7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout9</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0b14bc2c47a0defd656c0f03ff8bb26d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f6fb284cc70940632af79f88828f2a60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSlider *</type>
      <name>hsMic</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>63f7edaec83087c5ab907443137dc879</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout10</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3cea3de6f78cbf29d102eec8ebb3f5c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbActLoudness</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2c4b7da46a003b7bec3a7d0ebf9b1c8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbLoudness</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d7ed016ece9848889c7d2a8bfff31661</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>liSep1_2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1e0f144b25077626dc5f3fb3ccbc12d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbHelpSystem_2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c9d5f1c3cfb3103408d82dd1719fcdc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>89beecf0f5819e93405bf8b4e57bbf1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout13</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>fb0c8e19a66b28ec3b54dc179597693c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout11</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>61dbd4baae8539281e4bf1510a872c41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout14</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f343754754dccc41d62deac8ee9b5c55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSearchCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>b21e91315495b7712e1c2ba1446571d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSearchCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>ca80e7b11ff6265bf99e1800521642cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbClearSearchCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>83cccdb14aaebcf7d6be3b4c74cd373e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbShowCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>aed5eae61378741e38a20693c0cf72b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>cbShowCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>bf7f87dd39017ba46d38adba990bcc20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTableWidget *</type>
      <name>twCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>a24841c8898ccd2f660a849df24a2162</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout15</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>cdc017a5894d5e5c11b8093fa74634e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSchluesselWort</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>67f5dcefd192f49800cec927ad57d798</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSchluesselWort</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>77a13d77a71e1887919d1b1bb4928760</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout12</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8fd99cac537cf27cd73de31b4048be86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbReloadCommands</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>57be1eefd5156bebfa02ab4cc856d163</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1aaf0ae3627e7f75384f0b34be6c8f58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbEditCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6284e09d6aef5ce4f732384fff6e0f32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e1c0989de2392cb90ff4eeae5c026650</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbNewCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6185a3d1a0f7adf3e64ed5043b4a4bbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbDeleteCommand</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c3ce11aaf864363b424a57fe28790299</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_2</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>914c1deaae84ac2d00f895f82e4a9dea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbImportProgram</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>000fcc2e7b15333894f7cfab1c250d66</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbImportPlace</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1e916da68ce0894fe6222707baa12aa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbCreateMacro</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>7711bc1c49b05d5ce858dee62cd1b22a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout16</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>adf2b74e97507e7fbeb39c03cce07fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>liSep1_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6e8c15b85985edd0eb3b5e207cbba4d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbHelpSystem_3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>8b5e345d026ff238fa030942956dc1d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>39264192ced48f69bb205d1b817497a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout17</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>09243047226ea88abd73ae34aef2e683</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gwSearchInLogs</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>14392122f5ce2727caf06a804049f61f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout13</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>fe55ee31bb9433691ebc4af2a21a5860</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSearchLogs</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>42abbaad5cdeef01a87289ab25b7a2a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGridLayout *</type>
      <name>gridLayout3</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>06cddac27cd3515d9e83e609c787cc6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbLogError</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>ca9b4a50453ea17dc7dc235cb9129a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbLogInfo</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2114fb29a22e447f29aa9e788c50bfa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem51</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>adac851f80e203565a07e293c3def6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSearchLogsFor</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>c8ce73ffdaed1d43b3c66b1d2dcf7d10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbOnlyDay</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>13aad1542d5ae8ef65b80c922132a546</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCalendarWidget *</type>
      <name>cwLogDay</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2b4ceef6b5d18061726a78e812fa4c52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem52</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>e113e13de8481579c611945ef4051b03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout14</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>bad1eae1b5fad1fbf1ed51f9e5b6f958</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbLogComponents</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f0875a4d632a2bd69cbe06f94999cd7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout18</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>deb062becaf6340bdcd5d0b028a1a91f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbActionLog</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>26fa8dc4789ceab20b107524c9dc6539</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbRecordings</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3bd4308775b4941e091b44d954fde262</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTreeWidget *</type>
      <name>twLogEntries</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>133ddb3e556789962a5bccf9970eed02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>liSep1_4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>276c087f60a30764b9aa25981fa791bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbHelpSystem_4</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>efbf7d15c85d173c421272856617c9c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>dcc51577046084df23594add9d8d4e23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout19</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>9f962b5d9459676c76968520ad3753c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout15</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>42372fa1b5170e5b2f607759261f3675</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gpChooseRevert</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>f9897ee292bcaeee63f37932dbea20fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout16</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>4f59a243bfb127ed6f277e64ef655ebd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QComboBox *</type>
      <name>comboBox</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1469020e06677e0a1791c5d7a0b0cbee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gwSearchInChanges</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>3f36772512d7f9a9c5275310cb74a33b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout17</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>2a0cffa62c5049bdf0695900477b6880</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSearchChanges</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>fd0b03207c496ccec407649cf6149ebb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbGoBackTill</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>681fd4a22c5a414c621d58f8b3283ec9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCalendarWidget *</type>
      <name>cwTimeMachineDay</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>d412275e5f52bf0f44f64575ade4ebfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem53</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1a08207c44f212cb9ff2e0cfc120667a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout18</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>05463022ade69722880cb7fc9d171e53</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTableWidget *</type>
      <name>twChanges</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>83e0843382a1ae8baf3d01b10ce7130b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbRevertChanges</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>49cd0e2a2b9164729116c674b4a36a46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>liSep1_5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>6d1333b3517840a911a9e9deb0df2c80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbHelpSystem_5</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>1a042507499659990aeb9e48ed9d094a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout20</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>cb427c609a9bea2e87022ba7a5f1ba1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0bf4a3d9cae4b791a5fab19b8fc09620</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem54</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>54cfff6384c1e51ce9b0281413ff37c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbApply</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>0e4d2c4f8d0d6a33138b4c27c9176260</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbConfirm</name>
      <anchorfile>classUi__SettingsDialog.html</anchorfile>
      <anchor>70b57599dc24cb7991e284a1e09c15e2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_TrainMain</name>
    <filename>classUi__TrainMain.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>d1d0bc04241c51a43d437470b58b8ccd</anchor>
      <arglist>(QDialog *TrainMain)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>e20fb57668d5ec87d16e7eb19eb1c7e7</anchor>
      <arglist>(QDialog *TrainMain)</arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>2d4ec32ae92fdd550eb645b5ae42789d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QStackedWidget *</type>
      <name>swAction</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>07fcfa9f3457c4673c3185acc88eb957</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>61b0dbe81b9634dc40cb823ac05be4c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>847f7e3d5e140a1d174dea03dfdad408</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTableWidget *</type>
      <name>twTrainingWords</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>0f322525d9e96d353780f21683e55e58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>6aad029e984308bb335e047482635c7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbTrainText</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>c45bad9b295428aa5bc74cec7fb0b174</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGridLayout *</type>
      <name>gridLayout</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>a57238b1617de9fd8871e4f0dfc8cc84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbWordList</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>b380149ebb3b8758508d9e0ccc34cd55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>ea6026d9a10769d870f4b8cf75727eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbImportText</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>a34f44e07014f89549180b89dc507bcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbDelText</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>45708e1980966267ef1aeea763c68556</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_3</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>2639e8f60bcc6cbac9c2c0309143f72e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>47b94f31bedc4a35f262ae2afe2ea374</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbPage</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>4a3c8334b739533a99bbfcb827de0a8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout4</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>b4c172fe8d60aa91f59128cfb49137b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>frame</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>9a3897f345d7de29b45dead976e95843</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout5</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>7008dd2882b780012537238e4ff5a875</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbPage</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>135567d56d545e095882dea52bab63e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>wRecTexts</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>4f273b9a38283fbaf5283160064c0ff2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>05ad7bcfd824a09967434b0c04aa3371</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>pgRecogniced</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>c78b41f283f23c7bd02d742ef6029f94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>039dc37093a9ae10aba979ace003ddc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTextEdit *</type>
      <name>teRecognitionInfo</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>10447f07a8563c6213d2e35770699018</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbRecognitionInfo</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>0d3c386f682a056dbddb0eb00afcb5ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout6</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>5b5873fc99973bf45d1fc930a626ea21</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>ab97ced2e7d2db26e2db917ae4fc267b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbPrevPage</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>6a38d57f64a6399e96454023bbade463</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbNextPage</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>8bba85f5310cce06f497ac5d4e704b81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout3</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>9e625f051a232ee303ba65e4b2294ede</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBackToMain</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>407f85d09c5182f36d08434f63336458</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbPages</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>c1e8122cee0676cdfcc18f993c3da745</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbFinish</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>22be6ff4b60b8a21415226b996a38c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>page_2</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>fbe1e3f75856de7f5bea5fba169b4195</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout7</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>67ab6eba00bb7d6e638556dd2ca674e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QGroupBox *</type>
      <name>gbCurrentlyTraining</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>298b995ebf4851f3cd1d080a730e5842</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout8</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>cccf65b7ac939aae4e2c6231e6f8a1b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbProgressInfo</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>8dc864a6d8730ceceb3395319213f7da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QProgressBar *</type>
      <name>pbCompileModel</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>10016ff3105615791f5509a7e1b0006e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbCurrentStatus</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>5a1eeb4942423540debce331e623c2ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout4</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>4c8cd3610d87390546f9f5da93f2d065</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>3e8b0f58cf97648c5127602519f9ac88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbCancelTraining</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>954117ebeb47318f4fd33224436a1aee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem1</name>
      <anchorfile>classUi__TrainMain.html</anchorfile>
      <anchor>bf23dab08a42d805bf7229abd6b13754</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ui_WordList</name>
    <filename>classUi__WordList.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setupUi</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>7eaa8f3c1198f72ba47c63609628dafc</anchor>
      <arglist>(QDialog *WordList)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>retranslateUi</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>80fc5091702728b45b92f1cab182647e</anchor>
      <arglist>(QDialog *WordList)</arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>8452b01df6f7b8404804308ffa91f778</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>09f2dbe3bd5fc8a3ed2ae5c3164e241d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout1</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>2dd50307a5bc792ec4566fac40729e59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSearch_2</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>05506c4a15349889c0a7c5fd138d054a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QSpacerItem *</type>
      <name>spacerItem</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>ab6af2c0bb7ba4421ecae71cfbd3073b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>lbSearch</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>379e583c0bd692d8268f74a262101aa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLineEdit *</type>
      <name>leSearch</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>cd346b3f9c94bff18fd66175578ef7ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbClearSearch</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>2011e8cdac4e035b71a8508573fdc837</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>wList</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>91a6dc8321870dc9136ae47ad4d1660c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout1</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>068f25dd93e2fb67719eac8b36f4a973</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QCheckBox *</type>
      <name>cbShowCompleteLexicon</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>ddb826c78e8b08a89de14882f0975a6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout2</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>c6ac2cf63fb57073772cf67b2afe9252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout2</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>19374c23a8044826dd540c90eb5d4117</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout3</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>b1e6d3d792a05e6af13f53b968c9b7f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QLabel *</type>
      <name>label_2</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>3b3b65ea87e0e2bd38a0013345cfdb4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QWidget *</type>
      <name>wTrainList</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>e3405b846c79bfe3ea5ac306a46de817</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QVBoxLayout *</type>
      <name>vboxLayout4</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>c78918801ab7a0b8a66947ff1dfbadae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_2</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>f06c95d56437d9742ec982abca47ecfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAddToTraining</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>d5d6e9cc4df760ad9f6fcc6c2a3ab216</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbTrainList</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>abd51c1269f37823c492b95136a11d8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbDeleteTrainingWord</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>d5484e9792b2d729a9500fe3a19d03a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbSuggestTrain</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>40ebb52eb486836dc5e297ecc184c950</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_4</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>987a8deb0cccdc5bf991ae6adca1688f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbImport</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>bb6390f4dbede57698451d70d8ae34b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAddWord</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>b24fb7893798ce3d17b5709a3c432a0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbAddWord_3</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>88405753621144a7eb9e523c31ec8299</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>2d10bceccba1a78b8717d0afac7ef259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QTextBrowser *</type>
      <name>textBrowser</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>8843966ec291ca0038039e17a556504b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QFrame *</type>
      <name>line_3</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>dcbf9f0114b85677ca1465ae5e735281</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QHBoxLayout *</type>
      <name>hboxLayout3</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>7bdb558707b80e5addb3368e6e87b7c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbBack</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>f7d0f5bca65778b497f5fa9a62130539</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QPushButton *</type>
      <name>pbSwitchToTraining</name>
      <anchorfile>classUi__WordList.html</anchorfile>
      <anchor>a8248be5f3c5ad919d9ca98af52d2cfa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>VuMeter</name>
    <filename>classVuMeter.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>level</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>2c269c2bb0ce18a62bf906f632999779</anchor>
      <arglist>(int level)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>VuMeter</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>bc57d7d349c6484081a84682e3ce5e19</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>run</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>f820f98e26341422c2ffa2e8b0847a48</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prepare</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>057e38c74177645acf45ecb2442f2202</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>exec</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>3f9c3402c31d5cfd97e687cd3c20e9f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~VuMeter</name>
      <anchorfile>classVuMeter.html</anchorfile>
      <anchor>d2f02da41c2edc66fb99183eb2eb5041</anchor>
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
      <anchor>c960f514d34898a87354585a31b4f8a9</anchor>
      <arglist>(QString filename, int samplerate=0)</arglist>
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
      <type>int</type>
      <name>getSampleRate</name>
      <anchorfile>classWAV.html</anchorfile>
      <anchor>133226920a224b73179889de3f2527c4</anchor>
      <arglist>()</arglist>
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
      <name>increaseProgress</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>b83fc3823e4b0041fba07f157884a962</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>c656a3c3546c39d7ba2678c1470e68c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>currentProgress</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>4d09bea43b9122eb759901d8dae8edf7</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>finished</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>908575ee0849f37f648df9db6c6b846f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>getData</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>4db57cd1feb19e7f9e3495c38636bd35</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getChannels</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>f23748c3681467b6a9a06cd02b3c8cd4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>getPosition</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>1f3a1835c2d5a3a5939a735f18e896bd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPosition</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>cbb32d51446d49bd77ddb1a51e67bd35</anchor>
      <arglist>(long position)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getLength</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>8d0e3720e3c94ad7057b12901915fcd9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>WavPlayer</name>
      <anchorfile>classWavPlayer.html</anchorfile>
      <anchor>436a19426b3aa47c01ababc03bffcec8</anchor>
      <arglist>(QWidget *parent=0)</arglist>
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
    <member kind="slot">
      <type>void</type>
      <name>increaseProgress</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>702057fa295abbe664f1ead6e7f1e18e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>currentProgress</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>a853450c9be1fceb7a4ff46a8d20be42</anchor>
      <arglist>(int msecs)</arglist>
    </member>
    <member kind="function">
      <type>WAV *</type>
      <name>getWav</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>71f3b8b8809a090983fa9ecb5b9422b4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getChannels</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>d4a6ea7dc2f87cf12351d71c09796305</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>WavRecorder</name>
      <anchorfile>classWavRecorder.html</anchorfile>
      <anchor>5cd0123849358e024ae5d5dc5c5e5778</anchor>
      <arglist>(QWidget *parent=0)</arglist>
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
      <anchor>8fd49ff4568d24373e6009736f57f29d</anchor>
      <arglist>(QString filename, short channels, int sampleRate)</arglist>
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
    <member kind="signal">
      <type>void</type>
      <name>loaded</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>95ede07ec3bf8724f63e41de484f0d85</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>progress</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>8c35ccaf550bebc621cabe4bf2435645</anchor>
      <arglist>(int prog)</arglist>
    </member>
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
      <type>QStringList</type>
      <name>getWords</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>6a1d7824b0da1dc3acec9023d81250ee</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>getPronuncations</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>14753585357a7ebdc68e0956bd7a4a9f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>getTerminals</name>
      <anchorfile>classWiktionaryDict.html</anchorfile>
      <anchor>52b08c95afbd48cf80df3f0ef241d031</anchor>
      <arglist>()</arglist>
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
    <name>Word</name>
    <filename>classWord.html</filename>
    <member kind="function">
      <type></type>
      <name>Word</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>8c04c3e5bb5bb55279cd8d03fad23635</anchor>
      <arglist>(QString word, QStringList pronunciations, QString terminal=NULL, int probability=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Word</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>49e69fd0589df72dafd2931dbe484e5d</anchor>
      <arglist>(QString word, QString pronounciation, QString terminal=NULL, int probability=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addPronunciation</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>d332234b046cf3d413753a17c18eee33</anchor>
      <arglist>(QString pronounciation)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delPronunciation</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>9ae293e18498b60d6c46e9a42e7069c7</anchor>
      <arglist>(QString pronounciation)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delPronunciation</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>dabb1cf87fd32da988606499c44d7cd8</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getWord</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>761fa6bae4f866d7b313156fd01090ae</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const QString *</type>
      <name>getPronunciation</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>9748bb562318b61b6c414172c1889823</anchor>
      <arglist>(int i) const</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>getPronunciations</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>facdc7c1a298d737cbaa0b61836127fe</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getTerminal</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>49cfb150a2ceb50bf05d072a39cffa52</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const bool</type>
      <name>operator&lt;</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>9f933457433307266133f680bc7cd3d8</anchor>
      <arglist>(const Word w2) const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getPropability</name>
      <anchorfile>classWord.html</anchorfile>
      <anchor>8b624e994268542645f03bd4c218af48</anchor>
      <arglist>() const</arglist>
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
      <type>WordList *</type>
      <name>getExtraWords</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>ed48006bdb1037f3bb0fbbc9796c8179</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>removeDoubles</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>5fc0e96fd7c10c5a21e4c418c792569a</anchor>
      <arglist>(WordList *in)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>WordListManager</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>5d50f66586cbf234d280b864dc47a072</anchor>
      <arglist>(QString lexiconPath=&quot;model/lexicon&quot;, QString vocabPath=&quot;model/model.voca&quot;)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>readWordList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>751968652043976ff0c0e9e16e57baa4</anchor>
      <arglist>(QString lexiconpath=&quot;model/lexicon&quot;, QString vocabpath=&quot;model/model.voca&quot;, QString promptspath=&quot;model/prompts&quot;)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>readVocab</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>487bcaa10838ef57dfa6d14a59f777ad</anchor>
      <arglist>(QString vocabpath=&quot;model/model.voca&quot;)</arglist>
    </member>
    <member kind="function">
      <type>PromptsTable *</type>
      <name>readPrompts</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>5d7f76c226e8369206689228f3b84d6e</anchor>
      <arglist>(QString promptspath=&quot;model/prompts&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getTerminal</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>7556d2f5b835aed08684b8c2e2eb2fbc</anchor>
      <arglist>(QString name, QString pronunciation, WordList *wlist)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getProbability</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>0188caa271be5d7cae5dabf7621760f8</anchor>
      <arglist>(QString name, PromptsTable *promptsTable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trainList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>a9e5c01b9340ab7d23758b58d5d8aa53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>getWordList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>09afe03532709e4fdcaf6722a9013636</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addWords</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>0a150cfe6acc9a3bb2e08d2946643c64</anchor>
      <arglist>(WordList *list)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>3db07833c8ae18b3a9ca12406f85089b</anchor>
      <arglist>(QString lexiconFilename=&quot;&quot;, QString vocabFilename=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>WordList *</type>
      <name>sortList</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>7577fb6403c568892b4befd66be27372</anchor>
      <arglist>(WordList *list)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~WordListManager</name>
      <anchorfile>classWordListManager.html</anchorfile>
      <anchor>4e4ec9f1bd864bb366c5ac32375be4b2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>WordListView</name>
    <filename>classWordListView.html</filename>
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
      <name>trainList</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>7da96dc085e7e66e21f5094b4fe3c54c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>importDict</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>3d37440910f0dba298587bb33ba6a363</anchor>
      <arglist>()</arglist>
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
      <name>addWord</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>3c1293d94f87207b753a91a9d92884c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>toggleExtraWords</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>7c12383c92c27bcce4b421b656053749</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>filterListbyPattern</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>1b5683cb019b767aa3614f4f094c2ff3</anchor>
      <arglist>(QString filter)</arglist>
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
      <name>switchToGenericTraining</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>28ddc8fe1bb48e44f9f8f82afca3020d</anchor>
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
      <name>askToSave</name>
      <anchorfile>classWordListView.html</anchorfile>
      <anchor>b4b297176dad0b28dbc69965c7e5b857</anchor>
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
      <name>sendKey</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>31a98e6002e47a801e39a30e3285348b</anchor>
      <arglist>(int key)</arglist>
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
      <name>setModifierKey</name>
      <anchorfile>classXEvents.html</anchorfile>
      <anchor>06b1255d8229cf348ddbd4b60974a123</anchor>
      <arglist>(int virtualKey, bool once)</arglist>
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
      <type>void</type>
      <name>save</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>628c827ca7adb75c6020fc72487f20a2</anchor>
      <arglist>(CommandList commandlist, QString path=&quot;conf/commands.xml&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classXMLCommand.html</anchorfile>
      <anchor>368b2f611b2ae1fc79d5613d3f3510dd</anchor>
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
      <type>int</type>
      <name>save</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>6e41807ebdc1165b6ad6406b368b085f</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>load</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>7df75b1bbe6d0767fa25b9c919a57a1f</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLDomReader</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>09f107df068d1574b2f4853e64568053</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QDomDocument *</type>
      <name>doc</name>
      <anchorfile>classXMLDomReader.html</anchorfile>
      <anchor>a60903528553b068f1335f07b289cae6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLReader</name>
    <filename>classXMLReader.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>loaded</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>5056554cacea7b646572af1a79b8d61a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>closed</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>4bb2c4e86a62bf064c1182c461a0b5e4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>written</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>1428080732e0653749020fde20dd1f15</anchor>
      <arglist>()</arglist>
    </member>
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
    <member kind="variable" protection="protected">
      <type>QString</type>
      <name>path</name>
      <anchorfile>classXMLReader.html</anchorfile>
      <anchor>0ea761bd441baba8f06e5597ab23d463</anchor>
      <arglist></arglist>
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
    <name>XMLSetting</name>
    <filename>classXMLSetting.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>XMLSetting</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>0c48d7d1a5d321979e419d5a8c766cdd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>saveSettings</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>2c12fc69f747cda38e9e1af458a7236c</anchor>
      <arglist>(QHash&lt; QString, QString &gt; *settings)</arglist>
    </member>
    <member kind="function">
      <type>QHash&lt; QString, QString &gt; *</type>
      <name>getSettings</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>d5c08dcd30724d07bafde89979079429</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>settingToNode</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>db35603de794bee5e871bce36eb63c40</anchor>
      <arglist>(QDomDocument &amp;d, QString name, QString value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>loadSettings</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>dd2b1bfdc2fbe5c921375eea077070d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLSetting</name>
      <anchorfile>classXMLSetting.html</anchorfile>
      <anchor>a52fd39092128803a095dc35a15c19c4</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>XMLTrainingText</name>
    <filename>classXMLTrainingText.html</filename>
    <base>XMLDomReader</base>
    <member kind="function">
      <type></type>
      <name>XMLTrainingText</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>9a31237b81df69c69d6b7cee6d50fe24</anchor>
      <arglist>(QString path)</arglist>
    </member>
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
      <name>getTitle</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>7a1ae9dc1e016ce6aedc624d631ac738</anchor>
      <arglist>()</arglist>
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
    <member kind="function">
      <type>void</type>
      <name>addPages</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>f387f31d25d5cdcb5cf242bd6d2a5bb2</anchor>
      <arglist>(QStringList pages)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTitle</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>2d7da9085f92b40b1efe89f98f9682a4</anchor>
      <arglist>(QString title)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLTrainingText</name>
      <anchorfile>classXMLTrainingText.html</anchorfile>
      <anchor>15ba3b7d922633601dd0fc57becb915b</anchor>
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
      <type>void</type>
      <name>load</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>d258e71b4fb1552593c4804b7b5b66f7</anchor>
      <arglist>(QString path=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>QHash&lt; QString, QString &gt;</type>
      <name>getTrainingTextList</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>e3666979182255a40bd852aaee294531</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~XMLTrainingTextList</name>
      <anchorfile>classXMLTrainingTextList.html</anchorfile>
      <anchor>de25b19743fe3eedf3f9737644c93d35</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Ui</name>
    <filename>namespaceUi.html</filename>
    <class kind="class">Ui::AddWord</class>
    <class kind="class">Ui::ImportView</class>
    <class kind="class">Ui::MainWindow</class>
    <class kind="class">Ui::RunDialog</class>
    <class kind="class">Ui::SettingsDialog</class>
    <class kind="class">Ui::TrainMain</class>
    <class kind="class">Ui::WordList</class>
  </compound>
  <compound kind="class">
    <name>Ui::AddWord</name>
    <filename>classUi_1_1AddWord.html</filename>
    <base>Ui_AddWord</base>
  </compound>
  <compound kind="class">
    <name>Ui::ImportView</name>
    <filename>classUi_1_1ImportView.html</filename>
    <base>Ui_ImportView</base>
  </compound>
  <compound kind="class">
    <name>Ui::MainWindow</name>
    <filename>classUi_1_1MainWindow.html</filename>
    <base>Ui_MainWindow</base>
  </compound>
  <compound kind="class">
    <name>Ui::RunDialog</name>
    <filename>classUi_1_1RunDialog.html</filename>
    <base>Ui_RunDialog</base>
  </compound>
  <compound kind="class">
    <name>Ui::SettingsDialog</name>
    <filename>classUi_1_1SettingsDialog.html</filename>
    <base>Ui_SettingsDialog</base>
  </compound>
  <compound kind="class">
    <name>Ui::TrainMain</name>
    <filename>classUi_1_1TrainMain.html</filename>
    <base>Ui_TrainMain</base>
  </compound>
  <compound kind="class">
    <name>Ui::WordList</name>
    <filename>classUi_1_1WordList.html</filename>
    <base>Ui_WordList</base>
  </compound>
  <compound kind="dir">
    <name>Daten/</name>
    <path>/home/bedahr/Daten/</path>
    <filename>dir_75a69c984baa7e6e8be50e7ca3dbf29a.html</filename>
    <dir>Daten/Projects/</dir>
  </compound>
  <compound kind="dir">
    <name>Daten/Projects/</name>
    <path>/home/bedahr/Daten/Projects/</path>
    <filename>dir_bbf8db06686680272d23b3ca61529fc4.html</filename>
    <dir>Daten/Projects/simon/</dir>
  </compound>
  <compound kind="dir">
    <name>Daten/Projects/simon/</name>
    <path>/home/bedahr/Daten/Projects/simon/</path>
    <filename>dir_93ff0f96adb530d413f116882182f4fb.html</filename>
    <dir>Daten/Projects/simon/trunk/</dir>
  </compound>
  <compound kind="dir">
    <name>Daten/Projects/simon/trunk/</name>
    <path>/home/bedahr/Daten/Projects/simon/trunk/</path>
    <filename>dir_a426ca655a68ab7bac2a01c64fd326a0.html</filename>
    <file>addwordview.cpp</file>
    <file>addwordview.h</file>
    <file>command.h</file>
    <file>coreevents.h</file>
    <file>dict.cpp</file>
    <file>dict.h</file>
    <file>dragtablewidget.cpp</file>
    <file>dragtablewidget.h</file>
    <file>droplistwidget.cpp</file>
    <file>droplistwidget.h</file>
    <file>eventhandler.cpp</file>
    <file>eventhandler.h</file>
    <file>importdict.cpp</file>
    <file>importdict.h</file>
    <file>importdictview.cpp</file>
    <file>importdictview.h</file>
    <file>importlocalwizardpage.cpp</file>
    <file>importlocalwizardpage.h</file>
    <file>importremotewizardpage.cpp</file>
    <file>importremotewizardpage.h</file>
    <file>importtrainingtexts.cpp</file>
    <file>importtrainingtexts.h</file>
    <file>importworkingwizardpage.cpp</file>
    <file>importworkingwizardpage.h</file>
    <file>juliuscontrol.cpp</file>
    <file>juliuscontrol.h</file>
    <file>logger.cpp</file>
    <file>logger.h</file>
    <file>main.cpp</file>
    <file>moc_addwordview.cpp</file>
    <file>moc_dict.cpp</file>
    <file>moc_dragtablewidget.cpp</file>
    <file>moc_droplistwidget.cpp</file>
    <file>moc_importdict.cpp</file>
    <file>moc_importdictview.cpp</file>
    <file>moc_importlocalwizardpage.cpp</file>
    <file>moc_importremotewizardpage.cpp</file>
    <file>moc_importtrainingtexts.cpp</file>
    <file>moc_importworkingwizardpage.cpp</file>
    <file>moc_juliuscontrol.cpp</file>
    <file>moc_osd.cpp</file>
    <file>moc_quickdownloader.cpp</file>
    <file>moc_recwidget.cpp</file>
    <file>moc_runapplicationview.cpp</file>
    <file>moc_selectsourcewizardpage.cpp</file>
    <file>moc_settingsview.cpp</file>
    <file>moc_simoncontrol.cpp</file>
    <file>moc_simonview.cpp</file>
    <file>moc_trainingview.cpp</file>
    <file>moc_trayiconmanager.cpp</file>
    <file>moc_vumeter.cpp</file>
    <file>moc_wavplayer.cpp</file>
    <file>moc_wavrecorder.cpp</file>
    <file>moc_wiktionarydict.cpp</file>
    <file>moc_wordlistview.cpp</file>
    <file>moc_xmlreader.cpp</file>
    <file>modelmanager.cpp</file>
    <file>modelmanager.h</file>
    <file>osd.cpp</file>
    <file>osd.h</file>
    <file>qrc_simon.cpp</file>
    <file>quickdownloader.cpp</file>
    <file>quickdownloader.h</file>
    <file>recwidget.cpp</file>
    <file>recwidget.h</file>
    <file>RtAudio.cpp</file>
    <file>RtAudio.h</file>
    <file>RtError.h</file>
    <file>runapplicationview.cpp</file>
    <file>runapplicationview.h</file>
    <file>runbackend.h</file>
    <file>runcommand.cpp</file>
    <file>runcommand.h</file>
    <file>runlinuxbackend.cpp</file>
    <file>runlinuxbackend.h</file>
    <file>runwindowsbackend.cpp</file>
    <file>runwindowsbackend.h</file>
    <file>selectsourcewizardpage.cpp</file>
    <file>selectsourcewizardpage.h</file>
    <file>settingsmanager.h</file>
    <file>settingsview.cpp</file>
    <file>settingsview.h</file>
    <file>simoncontrol.cpp</file>
    <file>simoncontrol.h</file>
    <file>simoninfo.cpp</file>
    <file>simoninfo.h</file>
    <file>simonview.cpp</file>
    <file>simonview.h</file>
    <file>soundcontrol.cpp</file>
    <file>soundcontrol.h</file>
    <file>sounddevice.h</file>
    <file>trainingmanager.cpp</file>
    <file>trainingmanager.h</file>
    <file>trainingtext.cpp</file>
    <file>trainingtext.h</file>
    <file>trainingview.cpp</file>
    <file>trainingview.h</file>
    <file>trayiconmanager.cpp</file>
    <file>trayiconmanager.h</file>
    <file>ui_addword.h</file>
    <file>ui_importdict.h</file>
    <file>ui_main.h</file>
    <file>ui_rundialog.h</file>
    <file>ui_settings.h</file>
    <file>ui_trainmain.h</file>
    <file>ui_wordlist.h</file>
    <file>vumeter.cpp</file>
    <file>vumeter.h</file>
    <file>wav.cpp</file>
    <file>wav.h</file>
    <file>wavplayer.cpp</file>
    <file>wavplayer.h</file>
    <file>wavrecorder.cpp</file>
    <file>wavrecorder.h</file>
    <file>wiktionarydict.cpp</file>
    <file>wiktionarydict.h</file>
    <file>word.h</file>
    <file>wordlistmanager.cpp</file>
    <file>wordlistmanager.h</file>
    <file>wordlistview.cpp</file>
    <file>wordlistview.h</file>
    <file>xevents.cpp</file>
    <file>xevents.h</file>
    <file>xmlcommand.cpp</file>
    <file>xmlcommand.h</file>
    <file>xmldomreader.cpp</file>
    <file>xmldomreader.h</file>
    <file>xmlreader.cpp</file>
    <file>xmlreader.h</file>
    <file>xmlsaxreader.cpp</file>
    <file>xmlsaxreader.h</file>
    <file>xmlsetting.cpp</file>
    <file>xmlsetting.h</file>
    <file>xmltrainingtext.cpp</file>
    <file>xmltrainingtext.h</file>
    <file>xmltrainingtextlist.cpp</file>
    <file>xmltrainingtextlist.h</file>
  </compound>
</tagfile>
