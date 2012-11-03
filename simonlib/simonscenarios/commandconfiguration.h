/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_COMMANDCONFIGURATION_H_DADC6869FF594E9BAD62C1FDB2785840
#define SIMON_COMMANDCONFIGURATION_H_DADC6869FF594E9BAD62C1FDB2785840

#include "simonmodelmanagement_export.h"
#include <simonscenariobase/scenarioobject.h>
#include <QWidget>

class Scenario;
class QString;
class KComponentData;
class KAboutData;

/**
 * \class CommandConfiguration
 * \brief Provides a base class for a graphical configuration page for a CommandManager
 *
 * Derive a class from CommandConfiguration if you want your command plugin to have a configuration
 * widget. The configuration widget will be displayed in the "Manage plugins" dialog.
 *
 * This configuration page should normally list options of the plugin as a whole. If you want
 * to configure sub-elements you most likely want to use the command architecture.
 * \sa Command
 */
class MODELMANAGEMENT_EXPORT CommandConfiguration : public QWidget, public ScenarioObject
{
  Q_OBJECT
    signals:
  /// Emit this signal if the configuration has changed. If you fail to emit this
  /// signal, serialize() will not be called and all changes made will
  /// be discarded.
  void changed(bool);

  protected:
    /// This provides information about the plugin (configuration). This will
    /// automatically be initialized with the values you pass to the constructor
    /// but you can change it anytime
    KAboutData *about;

  private slots:
    void slotChanged();

  public:
    /// Constructor
    CommandConfiguration(Scenario *parent, const QByteArray& internalName, const KLocalizedString& name,
      const QByteArray& version, const KLocalizedString& desc,
      const QString& iconName, const KComponentData& componentData,
      const QVariantList &args = QVariantList());
    /// Destructor
    ~CommandConfiguration();

    /// Accessor: Returns the about data object
    KAboutData* aboutData() { return about; }

  public slots:
    /**
     * \brief Load the configuration from the given XML node
     *
     * This method will be called after the configuration object was created.
     * You should use it to parse the given QDomElement and read the
     * current configuration from it.
     *
     * The first time the plugin is loaded the given element will most likely
     * be 0 (altough your CommandManager subclass could potentially change
     * that) so it is a good idea to provide some sensible defaults if
     * isNull() returns true.
     *
     * The return value should only be false if the given configuration was
     * errenous.
     *
     * \param element The configuration to parse.
     * \return Success
     * \sa deSerialize()
     */
    virtual bool deSerialize(const QDomElement& element)=0;

    /**
     * \brief Save the current configuration
     *
     * This method will be called during the serialization of the
     * CommandManager. You should construct a sensible XML structure storing
     * all configuration options of this class and return the root element.
     *
     * The element you return here will eventually be passed back to
     * deSerialize() the next timem the configuration is initialized.
     *
     * \param *doc The parent document
     * \return The created configuration structure (XML Element)
     */
    virtual QDomElement serialize(QDomDocument *doc)=0;

    /**
     * \brief Return to the default values
     *
     * This should reset all your configuration values to their default values.
     */
    virtual void defaults()=0;

};
#endif
