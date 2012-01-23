/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "samplegroupcontext.h"
#include "ui_samplegroupcontext.h"
#include "newcondition.h"
#include <KMessageBox>
#include <QtXml/QDomDocument>

SampleGroupContext::SampleGroupContext(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SampleGroupContext)
{
    ui->setupUi(this);

    ui->pbAdd->setIcon(KIcon("list-add"));
    ui->pbEdit->setIcon(KIcon("edit-rename"));
    ui->pbRemove->setIcon(KIcon("edit-delete"));
    ui->pbPromote->setIcon(KIcon("arrow-up"));
    ui->pbDemote->setIcon(KIcon("arrow-down"));

    connect(ui->pbAdd, SIGNAL(clicked()),
            this, SLOT(addCondition()));
    connect(ui->pbEdit, SIGNAL(clicked()),
            this, SLOT(editCondition()));
    connect(ui->pbRemove, SIGNAL(clicked()),
            this, SLOT(removeCondition()));
    connect(ui->pbPromote, SIGNAL(clicked()),
            this, SLOT(promoteCondition()));
    connect(ui->pbDemote, SIGNAL(clicked()),
            this, SLOT(demoteCondition()));

    m_model = new SampleGroupConditionModel();
    ui->tvSampleGroupConditions->setModel(m_model);

    m_sampleGroupDelegate = new SampleGroupItemDelegate();
    ui->tvSampleGroupConditions->setItemDelegateForColumn(1, m_sampleGroupDelegate);
}

SampleGroupContext::~SampleGroupContext()
{
    delete ui;
    delete m_model;
}

Condition* SampleGroupContext::getCurrentCondition()
{
    QModelIndex index = ui->tvSampleGroupConditions->currentIndex();
    if (!index.isValid()) return 0;

    return static_cast<Condition*>(index.internalPointer());
}

void SampleGroupContext::addCondition()
{
    //get the CreateConditionWidgets
    QList<CreateConditionWidget*> widgets;
    QList<Condition*> conditions;
    ContextManager* manager = ContextManager::instance();
    QDomDocument *doc = new QDomDocument();
    CompoundCondition* compoundCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
    delete doc;

    conditions = manager->getConditions();

    foreach (Condition* condition, conditions)
    {
        widgets.push_back(condition->getCreateConditionWidget(compoundCondition, this));
    }

    //prepare the create condition dialog
    NewCondition *newCondition = new NewCondition(this);
    newCondition->registerCreators(widgets);

    //launch the dialog and add the condition if it is desired
    if (newCondition->newCondition())
    {
        if (compoundCondition->getConditions().count() > 0)
        {
            Condition *condition = compoundCondition->getConditions().front();

            manager->addSampleGroupCondition(condition, "default");
            //m_model->insertRow(manager->getSampleGroupConditionCount()-1);
        }
    }


    m_model->update();
    ui->tvSampleGroupConditions->reset();

    delete newCondition;
    delete compoundCondition;
}

void SampleGroupContext::editCondition()
{
    //get the condition to edit
    Condition* condition = getCurrentCondition();
    if (!condition)
        return;

    //get the CreateConditionWidgets
    QList<CreateConditionWidget*> widgets;
    QList<Condition*> conditions;
    ContextManager* manager = ContextManager::instance();

    QDomDocument *doc = new QDomDocument();
    CompoundCondition* compoundCondition = CompoundCondition::createInstance(CompoundCondition::createEmpty(doc));
    delete doc;

    conditions = manager->getConditions();

    foreach (Condition* c, conditions)
    {
        widgets.push_back(c->getCreateConditionWidget(compoundCondition, this));
    }

    //prepare the edit condition dialog
    NewCondition *editCondition = new NewCondition(this);
    editCondition->registerCreators(widgets);
    editCondition->init(condition);

    //launch the dialog and edit the condition if it is desired
    if (editCondition->newCondition())
    {
        if (compoundCondition->getConditions().count() > 0)
        {
            Condition *edit = compoundCondition->getConditions().front();

            int row = ui->tvSampleGroupConditions->currentIndex().row();
            QString sampleGroup = manager->getSampleGroup(row);
            manager->removeSampleGroupCondition(row);
            manager->addSampleGroupCondition(edit, sampleGroup, row);
        }
    }

    m_model->update();
    ui->tvSampleGroupConditions->reset();

    delete editCondition;
    delete compoundCondition;
}

void SampleGroupContext::removeCondition()
{
    Condition* condition = getCurrentCondition();

    if (!condition)
        return;

    if (KMessageBox::questionYesNoCancel(this, i18nc("%1 is the conditions name", "Are you sure that you want to irreversibly remove the sample group condition \"%1\"?", condition->name()), i18n("Remove Condition")) == KMessageBox::Yes)
    {
        int row = ui->tvSampleGroupConditions->currentIndex().row();
        if (!ContextManager::instance()->removeSampleGroupCondition(row))
        {
            kDebug() << "Error removing sample group condition!";
        }
    }

    m_model->update();
    ui->tvSampleGroupConditions->reset();
}

void SampleGroupContext::promoteCondition()
{
    int row = ui->tvSampleGroupConditions->currentIndex().row();
    ContextManager::instance()->promoteCondition(row);
    m_model->update();
}

void SampleGroupContext::demoteCondition()
{
    int row = ui->tvSampleGroupConditions->currentIndex().row();
    ContextManager::instance()->demoteCondition(row);
    m_model->update();
}
