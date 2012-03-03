/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QTest>
#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
 
#include "../avatarmodel.h"
#include "../avatar.h"

class testAvatar: public QObject
{
  Q_OBJECT
  private slots:
    void testGeneral();
    void testAvatarModel();
};

void testAvatar::testGeneral()
{
  QImage testImage(10,10,QImage::Format_ARGB32_Premultiplied);
  QImage testImage2(5,5,QImage::Format_ARGB32_Premultiplied);
  
  QString testName("Test");
  QString testName2("Test2");
  
  Avatar *avatar = new Avatar(1, testName, testImage);
  QCOMPARE(avatar->id(), 1);
  QCOMPARE(avatar->name(), testName);
  QCOMPARE(avatar->image(), testImage);
  
  avatar->setName(testName2);
  avatar->setImage(testImage2);
  QCOMPARE(avatar->id(), 1);
  QCOMPARE(avatar->name(), testName2);
  QCOMPARE(avatar->image(), testImage2);
  
  QDomDocument *doc = new QDomDocument;
  QDomElement storedAvatar = avatar->serialize(doc);
  
  Avatar *readAvatar = Avatar::createInstance(storedAvatar);
  QCOMPARE(avatar->id(), readAvatar->id());
  QCOMPARE(avatar->name(), readAvatar->name());
  
  //ensuring the images are the same;
  ///@note This test WILL fail if you use a lossy compression for avatars (default = PNG)
  QBuffer imageCompare1;
  QBuffer imageCompare2;
  imageCompare1.open(QIODevice::WriteOnly);
  imageCompare2.open(QIODevice::WriteOnly);
  avatar->image().save(&imageCompare1); 
  readAvatar->image().save(&imageCompare2);
  
  QCOMPARE(imageCompare1.buffer(), imageCompare2.buffer());
  
  delete avatar;
  delete readAvatar;
  delete doc;
}

void testAvatar::testAvatarModel()
{
  QList<Avatar*> avatars;
  
  QImage testImage(10,10,QImage::Format_ARGB32_Premultiplied);
  QImage testImage2(5,5,QImage::Format_ARGB32_Premultiplied);
  
  QString testName("Test");
  QString testName2("Test2");
  
  Avatar *avatar1 = new Avatar(1, testName, testImage);
  Avatar *avatar2 = new Avatar(5, testName2, testImage);
  
  avatars << avatar1;
  avatars << avatar2;
  
  AvatarModel *model = new AvatarModel(avatars);
  
  bool result;
  
  QCOMPARE(model->rowCount(), 2);
  QCOMPARE(model->data(model->index(0, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(static_cast<Avatar*>(model->index(0, 0).internalPointer()), avatar1);
  QCOMPARE(static_cast<Avatar*>(model->index(0, 0).internalPointer())->id(), 1);
  QVERIFY(!model->index(10,3).isValid());
  
  int nextId1 = model->getNextId(testName);
  Avatar *avatar3 = new Avatar(nextId1, testName, testImage2);
  result = model->addAvatar(avatar3);
  QCOMPARE(model->rowCount(), 3);
  QVERIFY(result);
  
  int nextId2 = model->getNextId(testName);
  
  QVERIFY(nextId1 != nextId2);
  
  Avatar *avatar4 = new Avatar(nextId2, testName, testImage2);
  result = model->addAvatar(avatar4);
  QCOMPARE(model->rowCount(), 4);
  QVERIFY(result);
  
  Avatar *avatar5 = new Avatar(nextId2, testName, testImage2);
  result = model->addAvatar(avatar5);
  QCOMPARE(model->rowCount(), 4); //no duplicate ids
  QVERIFY(!result);
  delete avatar5;
  
  
  QCOMPARE(static_cast<Avatar*>(model->index(0, 0).internalPointer()), avatar1);
  QCOMPARE(static_cast<Avatar*>(model->index(1, 0).internalPointer()), avatar2);
  QCOMPARE(static_cast<Avatar*>(model->index(2, 0).internalPointer()), avatar3);
  QCOMPARE(static_cast<Avatar*>(model->index(3, 0).internalPointer()), avatar4);
  
  QDomDocument *doc = new QDomDocument;
  QDomElement storedElem = model->serialize(doc);
  
  //default model
  AvatarModel *defaultModel = AvatarModel::createInstance(QDomElement());
  QVERIFY(defaultModel);
  delete defaultModel;
  
  //faulty data
  QDomElement elem1 = doc->createElement("avatars");
  QDomElement elem2 = doc->createElement("foo");
  elem1.appendChild(elem2);
  AvatarModel *faultyModel = AvatarModel::createInstance(elem1);
  QVERIFY(faultyModel);
  delete faultyModel;
  delete doc;
  
  //restore saved instance
  AvatarModel *restoredModel = AvatarModel::createInstance(storedElem);
  QVERIFY(restoredModel);
  QCOMPARE(restoredModel->rowCount(), 4);
  QCOMPARE(restoredModel->data(restoredModel->index(0, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(restoredModel->data(restoredModel->index(1, 0), Qt::DisplayRole).toString(), testName2);
  QCOMPARE(restoredModel->data(restoredModel->index(2, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(restoredModel->data(restoredModel->index(3, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(0, 0).internalPointer())->id(), avatar1->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(1, 0).internalPointer())->id(), avatar2->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(2, 0).internalPointer())->id(), avatar3->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(3, 0).internalPointer())->id(), avatar4->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(0, 0).internalPointer())->name(), avatar1->name());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(1, 0).internalPointer())->name(), avatar2->name());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(2, 0).internalPointer())->name(), avatar3->name());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(3, 0).internalPointer())->name(), avatar4->name());
  
  //remove avatar
  Avatar *newAvatar3 = restoredModel->getAvatar(avatar3->id());
  QVERIFY(newAvatar3);
  result = restoredModel->removeAvatar(newAvatar3);
  QCOMPARE(restoredModel->rowCount(), 3);
  QVERIFY(result);
  
  QVERIFY(restoredModel->getAvatar(avatar3->id()) == 0);
  QVERIFY(restoredModel->removeAvatar(newAvatar3) == false);
  delete newAvatar3;
  
  QCOMPARE(restoredModel->data(restoredModel->index(0, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(restoredModel->data(restoredModel->index(1, 0), Qt::DisplayRole).toString(), testName2);
  QCOMPARE(restoredModel->data(restoredModel->index(2, 0), Qt::DisplayRole).toString(), testName);
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(0, 0).internalPointer())->id(), avatar1->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(1, 0).internalPointer())->id(), avatar2->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(2, 0).internalPointer())->id(), avatar4->id());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(0, 0).internalPointer())->name(), avatar1->name());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(1, 0).internalPointer())->name(), avatar2->name());
  QCOMPARE(static_cast<Avatar*>(restoredModel->index(2, 0).internalPointer())->name(), avatar4->name());
  
  //get index
  QModelIndex index = restoredModel->getAvatarIndex(avatar4->id());
  QVERIFY(index.row() == 2);
  QVERIFY(index.column() == 0);
  QVERIFY(index.internalPointer() == restoredModel->getAvatar(avatar4->id()));
  
  delete restoredModel;
  delete model;
}

  
QTEST_MAIN(testAvatar)

#include "dialogavatartest.moc"
