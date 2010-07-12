/***************************************************************************
                          aimlparser.cpp  -  description
                             -------------------
    begin                : mer sep 21 2005
    copyright            : (C) 2005 by houssem bdioui
    email                : houssem.bdioui@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "aimlparser.h"
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDomNode>
#include <QDomElement>
#include <QDebug>
#include <QXmlInputSource>

//For windows system execution
#ifdef _WIN32
#include <windows.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>
#endif
#include <cstdlib>

//A much faster replacement of regExp.exactMatch(str)
//it also captures the words corresponding to the wildcards * & _
bool exactMatch(QString regExp, QString str, QStringList &capturedText)
{
  QStringList regExpWords = regExp.split(' ');
  QStringList strWords = str.split(' ');
  if ((!regExpWords.count() || !strWords.count()) && (regExpWords.count() != strWords.count()))
    return false;
  if (regExpWords.count() > strWords.count())
    return false;
  QStringList::ConstIterator regExpIt = regExpWords.begin();
  QStringList::ConstIterator strIt = strWords.begin();
  while ((strIt != strWords.end()) && (regExpIt != regExpWords.end())) {
    if ( (*regExpIt == "*") || (*regExpIt == "_") ) {
      regExpIt++;
      QStringList capturedStr;
      if (regExpIt != regExpWords.end()) {
        QString nextWord = *regExpIt;
        if ( (nextWord != "*") && (nextWord != "_") ) {
          while (true) {
            if (*strIt == nextWord)
              break;
            capturedStr += *strIt;
            if (++strIt == strWords.end())
              return false;
          }
        }
        else {
          capturedStr += *strIt;
          regExpIt --;
        }
      }
      else {
        while (true) {
          capturedStr += *strIt;
          if (++strIt == strWords.end())
            break;
        }
        capturedText += capturedStr.join(" ");
        return true;
      }
      capturedText += capturedStr.join(" ");
    }
    else if (*regExpIt != *strIt)
      return false;
    regExpIt++;
    strIt++;
  }
  return (regExpIt == regExpWords.end()) && (strIt == strWords.end());
}


QList<QDomNode> elementsByTagName(QDomNode *node, const QString& tagName)
{
  QList<QDomNode> list;
  QDomNodeList childNodes = node->toElement().elementsByTagName(tagName);
  for (int i = 0; i < childNodes.count(); i++) {
    QDomNode n = childNodes.item(i);
    if (n.parentNode() == *node)
      list.append(n);
  }
  return list;
}


Leaf::Leaf()
{
}


Node::Node()
{
}


bool Node::match(QStringList::const_iterator input, const QStringList &inputWords,
const QString &currentThat, const QString &currentTopic, QStringList &capturedThatTexts,
QStringList &capturedTopicTexts, Leaf *&leaf)
{
  if (input == inputWords.end())
    return false;

  if ((word == "*") || (word == "_")) {
    ++input;
    for (;input != inputWords.end(); input++) {
      foreach (Node *child, children) {
        if (child->match(input, inputWords, currentThat, currentTopic, capturedThatTexts,
          capturedTopicTexts, leaf))
          return true;
      }
    }
  }
  else {
    if (!word.isEmpty()) {
      if (word != *input)
        return false;
      ++input;
    }
    foreach (Node *child, children) {
      if (child->match(input, inputWords, currentThat, currentTopic, capturedThatTexts,
        capturedTopicTexts, leaf))
        return true;
    }
  }
  if (input == inputWords.end()) {
    foreach (leaf, leafs) {
      capturedThatTexts.clear();
      capturedTopicTexts.clear();
      if ( (!leaf->that.isEmpty() && !exactMatch(leaf->that, currentThat, capturedThatTexts)) ||
        (!leaf->topic.isEmpty() && !exactMatch(leaf->topic, currentTopic, capturedTopicTexts)) )
        continue;
      return true;
    }
  }

  return false;
}


void Node::debug(QTextStream* logStream, uint indent)
{
  QString indentStr = QString().fill('\t', indent);
  *logStream << indentStr << word << " :\n";
  foreach (Node *child, children)
    child->debug(logStream, indent + 1);
  indentStr = QString().fill('\t', indent + 1);
  foreach (Leaf *leaf, leafs)
    *logStream << indentStr + "<topic-" + leaf->topic + " that-" + leaf->that + ">\n";
}


void AIMLParser::runRegression()
{
  QDomDocument doc;
  QFile file( "utils/TestSuite.xml" );
  if ( !file.open( QIODevice::ReadOnly ) )
    return;
  if ( !doc.setContent( &file ) ) {
    file.close();
    *logStream << QString("Error while parsing %1\n").arg(file.fileName());
    return;
  }
  file.close();

  *logStream << "Regression running:\n";

  loadAiml("utils/TestSuite.aiml");

  QDomElement docElem = doc.documentElement();
  QDomNodeList testCaseList = docElem.elementsByTagName ("TestCase");
  for (int i = 0; i < testCaseList.count(); i++) {
    QDomElement n = testCaseList.item(i).toElement();
    QString description = n.namedItem("Description").firstChild().nodeValue();
    QString input = n.namedItem("Input").firstChild().nodeValue();

    QString expectedAnswer = "";
    QDomNode child = n.namedItem("ExpectedAnswer").firstChild();
    while (!child.isNull()) {
      if (child.isText())
        expectedAnswer += child.toText().nodeValue();
      child = child.nextSibling();
    }
    *logStream << "===========================================================================\n";
    *logStream << "::Description: " + description + '\n';
    *logStream << "::Expected answer: " + expectedAnswer + '\n';
    QString answer = getResponse(input);
    if (answer.simplified().toLower() == expectedAnswer.simplified().toLower())
      *logStream << "=> Pass\n";
    else
      *logStream << "=> Fail\n";
  }
}


void AIMLParser::displayTree()
{
  root.debug(logStream);
}


void AIMLParser::normalizeString(QString &str)
{
  QString newStr;
  for (int i = 0; i < str.length(); i++) {
    QChar c = str.at(i);
    if (c.isLetterOrNumber() || (c == '*') || (c == '_') || (c == ' '))
      newStr += c.toLower();
  }
  str = newStr;
}


AIMLParser::AIMLParser(QTextStream* logStream_)
: indent(0),
logStream(logStream_)
{
  root.parent = 0;
  QTime currentTime = QTime::currentTime();
  int val = currentTime.msec() + currentTime.second() + currentTime.minute();
  srand(val);
}


AIMLParser::~AIMLParser()
{}

bool AIMLParser::loadSubstitutions(const QString &filename)
{
  QDomDocument doc;
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) )
    return false;
  if ( !doc.setContent( &file ) ) {
    file.close();
    *logStream << QString("Error while parsing %1\n").arg(filename);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList subsList = docElem.elementsByTagName ("substitution");
  for (int i = 0; i < subsList.count(); i++) {
    QDomElement n = subsList.item(i).toElement();
    subOld.append(QRegExp(n.namedItem("old").firstChild().nodeValue()));
    subNew.append(n.namedItem("new").firstChild().nodeValue());
  }
  return true;
}


bool AIMLParser::loadVars(const QString &filename, const bool &bot)
{
  QDomDocument doc;
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) )
    return false;
  if ( !doc.setContent( &file ) ) {
    file.close();
    *logStream << QString("Error while parsing %1\n").arg(filename);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList setList = docElem.elementsByTagName ("set");
  for (int i = 0; i < setList.count(); i++) {
    QDomElement n = setList.item(i).toElement();
    QString name = n.attribute("name");
    QString value = n.firstChild().nodeValue();
    if (bot)
      botVarValue[name] = value;
    else
      parameterValue[name] = value;
  }
  return true;
}


bool AIMLParser::saveVars(const QString &filename)
{
  QDomDocument doc;
  QDomElement root = doc.createElement("vars");
  doc.appendChild(root);

  QMap<QString, QString>::ConstIterator it;
  for ( it = parameterValue.begin(); it != parameterValue.end(); ++it ) {
    QDomElement setElem = doc.createElement("set");
    setElem.setAttribute("name", it.key());
    QDomText text = doc.createTextNode(*it);
    setElem.appendChild(text);
    root.appendChild(setElem);
  }
  //Backup the file first
  QFile fileBackup( filename + ".bak" );
  if ( !fileBackup.open( QIODevice::WriteOnly ) )
    return false;
  QTextStream tsBackup(&fileBackup);
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) )
    return false;
  tsBackup << QString(file.readAll());
  fileBackup.close();
  file.close();
  //now, save it!
  if ( !file.open( QIODevice::WriteOnly ) )
    return false;
  QTextStream ts(&file);
  ts << doc.toString();
  file.close();
  return true;
}


bool AIMLParser::loadAiml(const QString &filename)
{
  QDomDocument doc( "mydocument" );
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    *logStream << "could not open file";
    return false;
  }

  QXmlInputSource src(&file);
  QXmlSimpleReader reader;
  reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", true);

  QString msg;
  int line, col;
  if ( !doc.setContent( &src, &reader, &msg, &line, &col ) ) {
    file.close();
    *logStream << QString("Error while parsing %1: %2 (line %3 - col %4)\n").arg(filename).arg(msg).arg(line).arg(col);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList categoryList = docElem.elementsByTagName ("category");
  for (int i = 0; i < categoryList.count(); i++) {
    QDomNode n = categoryList.item(i);
    parseCategory(&n);
  }
  return true;
}


//parses a category and creates a correspondant element
void AIMLParser::parseCategory(QDomNode* categoryNode)
{
  QDomNode patternNode = categoryNode->namedItem("pattern");
  QString pattern = resolveNode(&patternNode);
  normalizeString(pattern);
  //find where to insert the new node
  QStringList words = pattern.split(' ');
  Node *whereToInsert = &root;
  for ( QStringList::ConstIterator it = words.begin(); it != words.end(); ++it ) {
    bool found = false;
    //         for (Node* child = whereToInsert->children.first(); child; child = whereToInsert->children.next())
    for (QList<Node*>::iterator childIterator = whereToInsert->children.begin();
    childIterator != whereToInsert->children.end(); childIterator++) {
      if ((*childIterator)->word == *it) {
        whereToInsert = (*childIterator);
        found = true;
        break;
      }
    }
    if (!found) {
      for (; it != words.end(); ++it ) {
        Node *n = new Node;
        n->word = *it;
        n->parent = whereToInsert;
        int index = 0;
        if (*it == "*")
          index = whereToInsert->children.count();
        else if ((*it != "_") && whereToInsert->children.count() &&
          (whereToInsert->children.at(0)->word == "_"))
          index = 1;
        whereToInsert->children.insert(index, n);
        whereToInsert = n;
      }
      break;
    }
  }

  //Now insert the leaf
  Leaf *leaf = new Leaf;
  leaf->parent = whereToInsert;
  QDomNode thatNode = categoryNode->namedItem("that");
  if (!thatNode.isNull()) {
    leaf->that = thatNode.firstChild().toText().nodeValue();
    normalizeString(leaf->that);
  }
  leaf->tmplate = categoryNode->namedItem("template");
  QDomNode parentNode = categoryNode->parentNode();
  if (!parentNode.isNull() && (parentNode.nodeName() == "topic")) {
    leaf->topic = parentNode.toElement().attribute("name");
    normalizeString(leaf->topic);
  }
  int index = 0;
  int leafWeight = !leaf->that.isEmpty() + !leaf->topic.isEmpty() * 2;

  //     for (Leaf* childLeaf = whereToInsert->leafs.first(); childLeaf;
  //         childLeaf = whereToInsert->leafs.next())
  for (QList<Leaf*>::iterator leafIterator = whereToInsert->leafs.begin();
  leafIterator != whereToInsert->leafs.end(); leafIterator++) {
    int childLeafWeight = !(*leafIterator)->that.isEmpty() + !(*leafIterator)->topic.isEmpty() * 2;
    if (leafWeight >= childLeafWeight)
      break;
    ++index;
  }
  whereToInsert->leafs.insert(index, leaf);
}


//recursively replace all the values & return the QString result
QString AIMLParser::resolveNode(QDomNode* node, const QStringList &capturedTexts,
const QStringList &capturedThatTexts, const QStringList &capturedTopicTexts)
{
  QString result("");
  QString nodeName = node->nodeName();
  QDomElement element = node->toElement();
  if (nodeName == "random") {
    QList<QDomNode> childNodes = elementsByTagName(node, "li");
    uint childCount = childNodes.count();
    uint random = rand() % childCount;
    QDomNode child = childNodes[random];
    result = resolveNode(&child, capturedTexts, capturedThatTexts, capturedTopicTexts);
  }
  else if (nodeName == "condition") {
    QString name("");
    uint condType = 2;
    if (element.hasAttribute("name")) {
      condType = 1;
      name = element.attribute("name");
      if (element.hasAttribute("value")) {
        condType = 0;
        QString value = element.attribute("value").toUpper();
        QStringList dummy;
        if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
          //dirty trick to avoid infinite loop !
          element.setTagName("parsedCondition");
          result = resolveNode(&element, capturedTexts, capturedThatTexts, capturedTopicTexts);
          element.setTagName("condition");
        }
      }
    }
    if (condType) {
      QList<QDomNode> childNodes = elementsByTagName(node, "li");
      for (int i = 0; i < childNodes.count(); i++) {
        QDomNode n = childNodes[i];
        if (n.toElement().hasAttribute("value")) {
          if (condType == 2)
            name = n.toElement().attribute("name");
          QString value = n.toElement().attribute("value").toUpper();
          QStringList dummy;
          if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
            result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
            break;
          }
        }
        else {
          result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
          break;
        }
      }
    }
  }
  else {
    QDomNode n = node->firstChild();
    while (!n.isNull()) {
      result += resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
      n = n.nextSibling();
    }
    if (node->isText())
      result = node->toText().nodeValue();
    else if (nodeName == "set")
      parameterValue[element.attribute("name")] = result.trimmed();
    else if (nodeName == "srai")
      result = getResponse(result, true);
    else if (nodeName == "think")
      result = "";
    else if (nodeName == "system")
      result = executeCommand(result);
    else if (nodeName == "learn") {
      loadAiml(result);
      result = "";
    }
    else if (nodeName == "uppercase") {
      result = result.toUpper();
    }
    else if (nodeName == "lowercase") {
      result = result.toLower();
    }
    else if (!node->hasChildNodes()) {
      if (nodeName == "star") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTexts.count() ? capturedTexts[index] : QString("");
      }
      else if (nodeName == "thatstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedThatTexts.count() ? capturedThatTexts[index] : QString("");
      }
      else if (nodeName == "topicstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTopicTexts.count() ? capturedTopicTexts[index] : QString("");
      }
      else if (nodeName == "that") {
        QString indexStr = element.attribute("index", "1,1");
        if (!indexStr.contains(','))
          indexStr = "1," + indexStr;
        int index1 = indexStr.section(',', 0, 0).toInt()-1;
        int index2 = indexStr.section(',', 1, 1).toInt()-1;
        result = (index1 < thatList.count()) && (index2 < thatList[index1].count()) ?
          thatList[index1][index2] : QString("");
      }
      else if (nodeName == "sr")
        result = getResponse(capturedTexts.count() ? capturedTexts[0] : QString(""), true);
      else if ( (nodeName == "br") || (nodeName == "html:br") )
        result = "\n";
      else if ( nodeName == "get" )
        result = parameterValue[element.attribute("name")];
      else if ( nodeName == "bot")
        result = botVarValue[element.attribute("name")];
      else if ( (nodeName == "person") || (nodeName == "person2") || (nodeName == "gender") )
        result = capturedTexts.count() ? capturedTexts[0] : QString("");
      else if (nodeName == "input") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < inputList.count() ? inputList[index] : QString("");
      }
      //the following just to avoid warnings !
      else if (nodeName == "li")
        ;
      else
        *logStream << "Warning: unknown tag \"" + nodeName + "\"\n";
    }
    //the following just to avoid warnings !
    else if ((nodeName == "template") || (nodeName == "pattern") || (nodeName == "li")
      || (nodeName == "person") || (nodeName == "person2") || (nodeName == "gender")
      || (nodeName == "parsedCondition"))
      ;
    else
      *logStream << "Warning: unknown tag \"" + nodeName + "\"\n";
  }
  return result;
}


QString AIMLParser::getResponse(QString input, const bool &srai)
{
  //debug
  if (srai)
    ++indent;
  QString indentSpace = QString().fill(' ', 2*indent);
  *logStream << (!srai ? "\n" : "") + indentSpace + (srai ? "::SRAI: " : "::User Input: ") +
    input + '\n';
  //perform substitutions for input string
  QList<QRegExp>::Iterator itOld = subOld.begin();
  QStringList::Iterator itNew = subNew.begin();
  for (; itOld != subOld.end(); ++itOld, ++itNew )
    input.replace(*itOld, *itNew);
  if (!srai) {
    inputList.prepend(input);
    if (inputList.count() > MAX_LIST_LENGTH)
      inputList.pop_back();
  }

  QStringList capturedTexts, capturedThatTexts, capturedTopicTexts;
  QString curTopic = parameterValue["topic"];
  normalizeString(curTopic);
  Leaf *leaf = 0;
  QString result("");
  QStringList sentences = input.split(QRegExp("[\\.\\?!;\\x061f]"));
  QStringList::Iterator sentence = sentences.begin();
  while (true) {
    //normalizeString(*sentence);
    *sentence = (*sentence).toLower();
    QStringList inputWords = sentence->split(' ');
    QStringList::ConstIterator it = inputWords.begin();
    if (!root.match(it, inputWords, thatList.count() && thatList[0].count() ?
      thatList[0][0] : QString(""), curTopic, capturedThatTexts, capturedTopicTexts, leaf))
      return "Internal Error!";
    Node *parentNode = leaf->parent;
    QString matchedPattern = parentNode->word;
    while (parentNode->parent->parent) {
      parentNode = parentNode->parent;
      matchedPattern = parentNode->word + ' ' + matchedPattern;
    }
    *logStream << indentSpace + "::Matched pattern: [" + matchedPattern + ']';
    if (!leaf->that.isEmpty())
      *logStream << " - Matched that: [" + leaf->that + ']';
    if (!leaf->topic.isEmpty())
      *logStream << " - Matched topic: [" + leaf->topic + ']';
    *logStream << "\n";
    capturedTexts.clear();
    exactMatch(matchedPattern, *sentence, capturedTexts);
    //strip whitespaces from the beginning and the end of result
    if (visitedNodeList.contains(&leaf->tmplate))
      *logStream << "Infinite loop detected!";    //check why... result += "ProgramQ: Infinite loop detected!";
    else {
      visitedNodeList.append(&leaf->tmplate);
      result += resolveNode(&leaf->tmplate, capturedTexts, capturedThatTexts, capturedTopicTexts).trimmed();
    }
    sentence++;
    if (sentence != sentences.end())
      result += ' ';
    else
      break;
  }
  if (!srai) {
    QString tempResult = result.simplified();
    //get the sentences of the result split by: . ? ! ; and "arabic ?"
    QStringList thatSentencesList = tempResult.split(QRegExp("[\\.\\?!;\\x061f]"));
    QStringList inversedList;
    for (QStringList::Iterator it = thatSentencesList.begin(); it != thatSentencesList.end(); ++it) {
      //perform substitutions for that string
      itOld = subOld.begin();
      itNew = subNew.begin();
      for (; itOld != subOld.end(); ++itOld, ++itNew )
        tempResult.replace(*itOld, *itNew);
      normalizeString(*it);
      inversedList.prepend(*it);
    }
    thatList.prepend(inversedList);
    if (thatList.count() > MAX_LIST_LENGTH)
      thatList.pop_back();
    visitedNodeList.clear();
  }
  //debug
  *logStream << indentSpace + "::Result: " + result + '\n';
  if (srai)
    indent --;

  return result;
}


QString AIMLParser::executeCommand(const QString &commandStr)
{
  QString returnString("");
  QString spaceIndent = QString().fill(' ', 2*indent);
  *logStream << spaceIndent + "Executing \"" + commandStr + "\" ...\n";
  #ifdef Q_OS_WIN32

  STARTUPINFO si;
  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;
  PROCESS_INFORMATION pi;
  Qt::HANDLE read_pipe, write_pipe;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  int fd, create;
  OSVERSIONINFO osv;
  osv.dwOSVersionInfoSize = sizeof(osv);

  GetVersionEx(&osv);

  if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
    sa.lpSecurityDescriptor = &sd;
  }
  else                                            /* Pipe will use ACLs from default descriptor */
    sa.lpSecurityDescriptor = 0;

  /* Create a new pipe with system's default buffer size */
  if (!CreatePipe(&read_pipe, &write_pipe, &sa, 0)) {
    *logStream << spaceIndent + "Execution failed !\n";
    return "";
  }

  GetStartupInfo(&si);

  /* Sets the standard output handle for the process to the
  handle specified in hStdOutput */
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

  si.hStdOutput = write_pipe;
  si.hStdError  = (Qt::HANDLE) _get_osfhandle (2);
  si.wShowWindow = 0;
  create = CreateProcess(0,                       // The full path of app to launch
                                                  // Command line parameters
    (WCHAR*) QString("cmd.exe /c \"%1\"").arg(commandStr).utf16(),
    0,                                            // Default process security attributes
    0,                                            // Default thread security attributes
    TRUE,                                         // Inherit handles from the parent
    0,                                            // Normal priority
    0,                                            // Use the same environment as the parent
    0,                                            // Use app's directory as current
    &si,                                          // Startup Information
    &pi);                                         // Process information stored upon return

  if (!create) {
    *logStream << spaceIndent + "Execution failed !\n";
    return "";
  }

  /* Associates a file descriptor with the stdout pipe */
  fd = _open_osfhandle(/*(intptr_t)*/(long int)read_pipe, _O_BINARY);

  /* Close the handle that we're not going to use */
  CloseHandle(write_pipe);

  if (!fd) {
    *logStream << spaceIndent + "Execution failed !\n";
    return "";
  }

  /* Open the pipe stream using its file descriptor */
  FILE *file = fdopen(fd, "r");

  if(!file) {
    *logStream << spaceIndent + "Execution failed !\n";
    return "";
  }
  #else

  FILE *file = popen(commandStr.toLatin1().data(), "r");
  if (!file) {
    *logStream << spaceIndent + "Execution failed !\n";
    return "";
  }
  #endif

  char c = 0;
  while (c != EOF) {
    c = (char)getc(file);

    //         if (isascii(c))
    returnString += c;
  }

  fclose(file);

  *logStream << (spaceIndent + "Execution succeeded with result: \"" + returnString + "\"\n");

  return returnString;
}
