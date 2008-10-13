/***************************************************************************
                          aimlparser.h  -  description
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

#ifndef AIMLPARSER_H
#define AIMLPARSER_H

#include <QtXml>
#include <QMap>

#include <QList>

#include <QStringList>
#include <QTextStream>
#include <QRegExp>

#define MAX_LIST_LENGTH 50

/**
  *@author houssem bdioui
  */

struct Node;

struct Leaf
{
  Node *parent;
  QDomNode tmplate;
  QString topic;
  QString that;
  Leaf();
};

struct Node
{
  Node *parent;
  QString word;
  Node();
  QList<Node*> childs;
  QList<Leaf*> leafs;
  void debug(QTextStream* logStream, uint indent = 0);
  bool match(QStringList::const_iterator, const QStringList&,
             const QString&, const QString&, QStringList &, QStringList &, Leaf *&);
};

class AIMLParser
{
public:
  AIMLParser(QTextStream*);
  virtual ~AIMLParser();
  bool loadAiml(const QString&);
  bool loadSubstitutions(const QString&);
  bool loadVars(const QString&, const bool&);
  bool saveVars(const QString &);
  QString getResponse(QString, const bool &srai = false);
  void displayTree();
  void runRegression();
private:
  QString resolveNode(QDomNode*, const QStringList & = QStringList(),
     const QStringList & = QStringList(), const QStringList & = QStringList());
  void parseCategory(QDomNode*);
  void normalizeString(QString &);
  QString executeCommand(const QString&);
private:
  QMap<QString, QString> parameterValue;
  QMap<QString, QString> botVarValue;
  QList<QRegExp> subOld;
  QStringList subNew;
  QStringList inputList;
  QList<QStringList> thatList;
  Node root;
  int indent;
  QTextStream *logStream;
  QList<QDomNode*> visitedNodeList;
};

#endif




