#include "firstrunimportdictselectsourcepage.h"
#include <QRadioButton>
#include <QVariant>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the object
 */
FirstRunImportDictSelectSourcePage::FirstRunImportDictSelectSourcePage(QWidget* parent) 
	: ImportDictSelectSourcePage (parent) 
{
	none->show();
}

int FirstRunImportDictSelectSourcePage::nextId() const
{
	if (field("hadifix").toBool())
	{
		return FirstRunWizard::DictionaryBOMPPage;
	} else if (field("wiktionary").toBool()) 
		return FirstRunWizard::DictionaryWiktionaryPage;
	else if (field("NoDictionary").toBool())
		return FirstRunWizard::GrammarDescriptionPage;
	else
		return FirstRunWizard::DictionaryLexiconPage;
}
