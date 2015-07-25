/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "macprocessinfogathererhelper.h"
#include "SystemEvents.h"
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <ScriptingBridge/ScriptingBridge.h>
#include <QDebug>
#include <QFileInfo>

/**
 * Based on: http://www.qtcentre.org/threads/34752-NSString-to-QString
 */
QString qt_mac_NSStringToQString(const NSString *nsstr)
{
  NSRange range;
  range.location = 0;
  range.length = [nsstr length];
  unichar *chars = new unichar[range.length];
  [nsstr getCharacters:chars range:range];
  QString result = QString::fromUtf16(chars, range.length);
  delete[] chars;
  return result;
}

QString executableFromApplication(NSRunningApplication* app)
{
  QString fullPath = qt_mac_NSStringToQString([[app executableURL] path]);
  return QFileInfo(fullPath).fileName();
}

/**
 * Based on: http://stackoverflow.com/questions/1606321/how-do-i-get-a-list-of-the-window-titles-on-the-mac-osx
 */
QString MacProcessInfoGathererHelper::getActiveWindow()
{
  SystemEventsApplication *systemEvents = [SBApplication applicationWithBundleIdentifier:@"com.apple.systemevents"];
  SBElementArray *processes = [systemEvents processes];
  NSArray *processesArray = [[systemEvents processes] get];
  for (SystemEventsProcess* process in processesArray) {
    if (![process frontmost])
      continue;

    NSArray *titles = [[process windows]  arrayByApplyingSelector:@selector(title)];
    for (NSString* title in titles) {
      //first title will be the currently active window
      return qt_mac_NSStringToQString(title);
    }
  }

  return QString();
}

QString MacProcessInfoGathererHelper::getActiveProcess()
{
  return executableFromApplication([[NSWorkspace sharedWorkspace] frontmostApplication]);
}

QStringList MacProcessInfoGathererHelper::getCurrentProcesses()
{
  NSArray *appList = [[NSWorkspace sharedWorkspace] runningApplications];
  NSRunningApplication *runningApp;
  QStringList processes;
  for (runningApp in appList) {
    processes << executableFromApplication(runningApp);
  }
  return processes;
}

