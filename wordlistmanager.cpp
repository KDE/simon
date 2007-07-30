//
// C++ Implementation: wordlistmanager
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wordlistmanager.h"
#include "logger.h"

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 * @param QString path
 * Sets the path (member) to the given string
 */
WordListManager::WordListManager ( QString lexiconPath, QString vocabPath )
{
	this->wordlist = new WordList();
	this->lexiconPath = lexiconPath;
	this->vocabPath = vocabPath;
	this->extralist = new WordList();
	this->wordlist = readWordList ( lexiconPath, vocabPath );
}


/**
 * \brief Sorts the given List of words
 * \author Peter Grasch
 * \param WordList* list
 * The list of words to sort
 * \return WordList*
 * The sorted list of words
 */
WordList* WordListManager::sortList(WordList* list)
{
	Logger::log("Sorting a list of "+QString::number(list->count())+" words");
	qSort(list->begin(), list->end());
	return list;
}
		

/**
 * \brief Saves the current WordList (member)
 * \author Peter Grasch
 * \param QString filename
 * If not provided we default on the filename we used to open the file
 * \return bool
 * Saving successful?
 */
bool WordListManager::save ( QString lexiconFilename, QString vocabFilename )
{
	Logger::log("Saving wordlist");
	Logger::log("Creating wordlist utilizing the vocabulary from "+vocabFilename
		   +"and the current wordlist");
	
	WordList *saving = this->wordlist;
	for (int i=0; i < extralist->count(); i++)
		saving->append(this->extralist->at(i));
	
	saving = sortList(saving);
	
	if (lexiconFilename.isEmpty()) lexiconFilename = this->lexiconPath;
	if (vocabFilename.isEmpty()) vocabFilename = this->vocabPath;
	
	Logger::log("Opening output file: "+lexiconPath);
	
	QFile *outfile = new QFile(lexiconPath);
	if (!outfile->open(QIODevice::WriteOnly)) {
		Logger::log("Cannot open outputfile "+lexiconPath);
		return false;
	}
	QTextStream outstream(outfile);
	outstream.setCodec("ISO 8859-15");
	
	for (int i=0; i< saving->count(); i++)
	{
		for (int j=0; j<saving->at(i).getPronunciations().count(); j++)
			outstream << QString(saving->at(i).getWord().trimmed().toUpper() 
				+ "\t\t[" + saving->at(i).getWord().trimmed() + "]\t\t" +
				saving->at(i).getPronunciations().at(j)).trimmed() << "\n";
	}
	
	Logger::log("Closing output file");
	outfile->close();
	
	return true;
}

/**
 * @brief Reads the Vocab and returns the read Data
 *
 * This method parses the given File and creates a WordList out of it
 *
 * @author Peter Grasch
 *
 * @param QString path
 * Path to the lexicon
 * @return Wordlist*
 * The parsed WordList
 */
WordList* WordListManager::readWordList ( QString lexiconpath, QString vocabpath, QString promptspath )
{
	Logger::log ("Reading wordlist consisting of ");
	Logger::log("\t\tLexicon: "+lexiconPath+",");
	Logger::log("\t\tVocabulary: "+vocabPath+",");
	Logger::log("\t\tPrompts: "+promptspath);
	
	WordList *wordlist = new WordList();
	//read the vocab
	WordList *vocablist = readVocab(vocabpath);
	//read the prompts
	PromptsTable *promptsTable = readPrompts(promptspath);

	//opening
	Logger::log("Opening output file at "+lexiconPath);
	QFile *lexicon = new QFile ( lexiconpath );
	
	if ( !lexicon->open ( QFile::ReadOnly ) || !vocablist || !promptsTable) return false;

	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString line;
	QString name;
	QString output;
	QString pronunciation;
	QString category;
	int probability;

	length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	
	while ( length != -1 )	//for each line that was successfully read
	{
		line = QString ( buffer );
		
		//parsing the line
		name = line.left ( line.indexOf ( "\t" ) ).trimmed();
		output = line.mid ( line.indexOf ( "\t" ), line.lastIndexOf ( "\t" ) - line.indexOf ( "\t" ) ).trimmed();
		output = output.mid ( 1, output.length()-2 );
		pronunciation = line.right ( line.length() - line.lastIndexOf ( "\t" ) ).trimmed();
		
		category = getTerminal( name, pronunciation, vocablist );
		probability = getProbability( name, promptsTable );
		
		//creates and appends the word to the wordlist
		if (!category.isEmpty())
			wordlist->append(Word(output, pronunciation, category, probability));
		else  extralist->append(Word(output, pronunciation, "Unbekannt", probability));
		

		//reading the next line
		length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	}
	Logger::log("Wordlist created");
	return wordlist;
}


WordList* WordListManager::removeDoubles(WordList *in)
{
	if (!in) return NULL;
	
	Logger::log("Cleaning up wordlist");
	
	for (int i=0; i < in->count(); i++)
	{
		for (int j=i; j < in->count(); j++)
		{
			if (in->at(i).getWord() == in->at(j).getWord())
			{
				for (int k=0; k < in->at(i).getPronunciations().count(); k++)
				{
					Word w = in->at(i);
					w.delPronunciation(w.getPronunciations().at(k));
				}
				if (in->at(j).getPronunciations().count() == 0)
				{
					in->removeAt(j);
				}
			}
		}
	}
	return in;
}


/**
 * \brief Returns the Terminal of the name & pronunciation (it is pulled out of the wlist)
 * \author Peter Grasch
 * \param QString name
 * Name of the word
 * \param QString pronunciation
 * Pronunciation of the word
 * \param WordList *wlist
 * The list of words where we look for the Terminal
 * \return QString
 * The found terminal
 */
QString WordListManager::getTerminal(QString name, QString pronunciation, WordList *wlist)
{
	int i=0;
	QString terminal("");
	while (i < wlist->count())
	{
		//Because vocabs have just one pronunciation for each entry
		if ((wlist->at( i ).getWord() == name) && 
				   ( (* wlist->at( i ).getPronunciation(0)) == pronunciation))
			
			terminal += (terminal.isEmpty()) ? wlist->at( i ).getTerminal() :
					", " + wlist->at( i ).getTerminal();
		i++;
	}
// 	Logger::log("Resolving terminal of "+name+": "+terminal);
	// there was no result
	return (terminal.isEmpty()) ? "" : terminal;
}

/**
 * \brief Adds the words to the local wordlist
 * \author Peter Grasch
 * \param WordList *list
 * List of words to add
 */
void WordListManager::addWords(WordList *list)
{
	Logger::log("Adding "+QString::number(list->count())+" words to the wordlist");
	for (int i=0; i<list->count(); i++)
		this->wordlist->append(list->at(i));
	
	removeDoubles(list);
}

/**
 * \brief Returns the probability of the name (it is pulled out of the promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \param PromptsTable
 * Promptsfile
 * \return int
 * Probability to recognize; The higher the more likly simon will recognize this word correctly
 */
int WordListManager::getProbability(QString wordname, PromptsTable *promptsTable)
{
	int i=0;
	int probability = 0;
	int foundpos;
	QList<QString> promptsList = promptsTable->values();
	while (i < promptsList.count())
	{
		foundpos = promptsList.at(i).indexOf( wordname );
		while (foundpos != -1)
		{
			probability++;
			promptsList.replace( i, promptsList.at(i).mid(
					     promptsList.at(i).indexOf( wordname )+ wordname.length()));
			foundpos = promptsList.at(i).indexOf( wordname );
		}
		i++;
	}
// 	Logger::log("Resolving probability of "+wordname+": "+QString::number(probability));
	
	return probability;
}

/**
 * \brief Builds and returns the promptstable by parsing the file at the given path
 * \author Peter Grasch
 * \param QString promptspath
 * The path to the prompts file
 * \return PromptsTable*
 * The table
 */
PromptsTable* WordListManager::readPrompts(QString promptspath)
{
	Logger::log("Parsing promptsfile from "+promptspath);
	PromptsTable *promptsTable = new PromptsTable();
	
	QFile *prompts = new QFile ( promptspath );
	prompts->open ( QFile::ReadOnly );
	if ( !prompts->isReadable() ) return false;
	
	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString label;
	QString prompt;
	QString line;
	length = prompts->readLine ( buffer, sizeof ( buffer ) );
	while ( length != -1 ) //for each line that was successfully read
	{
		line = QString ( buffer );
		
		label = line.left(line.indexOf(" ")).trimmed();
		prompt = line.right( line.length() - line.indexOf(" ") ).trimmed();

		promptsTable->insert( label, prompt );
		//reading the next line
		length = prompts->readLine ( buffer, sizeof ( buffer ) );
	}
	
	return promptsTable;
}

/**
 * \brief Reads the vocab of the given file and parses it into the WordList which is returned
 * \author Peter Grasch
 * \param QString vocabpath
 * Path to the vocabs
 * \return WordList*
 * The list of parsed vocab
 */
WordList* WordListManager::readVocab(QString vocabpath)
{
	Logger::log("Reading vocab from "+vocabpath);
	WordList *vocablist = new WordList();
	
	QFile *vocab = new QFile ( vocabpath );
	vocab->open ( QFile::ReadOnly );
	if ( !vocab->isReadable() ) return false;
	
	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString line;
	QString name;
	QString pronunciation;
	QString terminal;

	length = vocab->readLine ( buffer, sizeof ( buffer ) );
	while ( length != -1 ) //for each line that was successfully read
	{
		line = QString ( buffer );
		
		if (line.indexOf("%") == 0) 
		{
			//The Line is the Definition of the terminal
			terminal = line.mid(2).trimmed();
		}
		
		//parsing the line
		name = line.left ( line.indexOf ( "\t" ) ).trimmed();
		pronunciation = line.mid ( line.indexOf ( "\t" ) ).trimmed();
		
		//creates and appends the word to the wordlist
		vocablist->append ( Word(name, pronunciation, terminal, 0 ) );

		//reading the next line
		length = vocab->readLine ( buffer, sizeof ( buffer ) );
	}
	
	return vocablist;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WordListManager::~WordListManager()
{}
