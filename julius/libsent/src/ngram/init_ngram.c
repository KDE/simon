/**
 * @file   init_ngram.c
 * 
 * <JA>
 * @brief  N-gramファイルをメモリに読み込み単語辞書と対応を取る
 * </JA>
 * 
 * <EN>
 * @brief  Load N-gram file into memory and setup with word dictionary
 * </EN>
 * 
 * @author Akinobu LEE
 * @date   Wed Feb 16 07:40:53 2005
 *
 * $Revision: 1.3 $
 * 
 */
/*
 * Copyright (c) 1991-2007 Kawahara Lab., Kyoto University
 * Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 * Copyright (c) 2005-2007 Julius project team, Nagoya Institute of Technology
 * All rights reserved
 */

#include <sent/stddefs.h>
#include <sent/ngram2.h>
#include <sent/vocabulary.h>

/** 
 * Read and setup N-gram data from binary format file.
 * 
 * @param ndata [out] pointer to N-gram data structure to store the data
 * @param bin_ngram_file [in] file name of the binary N-gram
 */
boolean
init_ngram_bin(NGRAM_INFO *ndata, char *bin_ngram_file)
{
  FILE *fp;
  
  jlog("Stat: init_ngram: reading in binary n-gram from %s\n", bin_ngram_file);
  if ((fp = fopen_readfile(bin_ngram_file)) == NULL) {
    jlog("Error: init_ngram: failed to open \"%s\"\n", bin_ngram_file);
    return FALSE;
  }
  if (ngram_read_bin(fp, ndata) == FALSE) {
    jlog("Error: init_ngram: failed to read \"%s\"\n", bin_ngram_file);
    return FALSE;
  }
  if (fclose_readfile(fp) == -1) {
    jlog("Error: init_ngram: failed to close \"%s\"\n", bin_ngram_file);
    return FALSE;
  }
  jlog("Stat: init_ngram: finished reading n-gram\n");
  return TRUE;
}

/** 
 * Read and setup N-gram data from ARPA format file.
 * 
 * @param ndata [out] pointer to N-gram data structure to store the data
 * @param ngram_file [in] file name of ARPA (reverse) 3-gram file
 * @param dir [in] direction (DIR_LR | DIR_RL)
 */
boolean
init_ngram_arpa(NGRAM_INFO *ndata, char *ngram_file, int dir)
{
  FILE *fp;

  ndata->root = NULL;
  ndata->dir = dir;

  jlog("Stat: init_ngram: reading in ARPA %s n-gram from %s\n", (ndata->dir == DIR_LR) ? "forward" : "backward", ngram_file);
  /* read RL n-gram */
  if ((fp = fopen_readfile(ngram_file)) == NULL) {
    jlog("Error: init_ngram: failed to open \"%s\"\n", ngram_file);
    return FALSE;
  }
  if (ngram_read_arpa(fp, ndata, FALSE) == FALSE) {
    jlog("Error: init_ngram: failed to read \"%s\"\n", ngram_file);
    return FALSE;
  }
  if (fclose_readfile(fp) == -1) {
    jlog("Error: init_ngram: failed to close \"%s\"\n", ngram_file);
    return FALSE;
  }
  jlog("Stat: init_ngram: finished reading n-gram\n");

  return TRUE;
}

/** 
 * Read additional LR 2-gram for 1st pass.
 * 
 * @param ndata [out] pointer to N-gram data structure to store the data
 * @param bigram_file [in] file name of ARPA 2-gram file
 */
boolean
init_ngram_arpa_additional(NGRAM_INFO *ndata, char *bigram_file)
{
  FILE *fp;

  jlog("Stat: init_ngram: reading in additional LR 2-gram for the 1st pass from %s\n", bigram_file);
  if ((fp = fopen_readfile(bigram_file)) == NULL) {
    jlog("Error: init_ngram: failed to open \"%s\"\n", bigram_file);
    return FALSE;
  }
  if (ngram_read_arpa(fp, ndata, TRUE) == FALSE) {
    jlog("Error: init_ngram: failed to read \"%s\"\n", bigram_file);
    return FALSE;
  }
  if (fclose_readfile(fp) == -1) {
    jlog("Error: init_ngram: failed to close \"%s\"\n", bigram_file);
    return FALSE;
  }
  jlog("Stat: init_ngram: finished reading LR 2-gram\n");

  return TRUE;
}

/** 
 * Make correspondence between word dictionary and N-gram vocabulary.
 * 
 * @param ndata [i/o] word/class N-gram, the unknown word information will be set.
 * @param winfo [i/o] word dictionary, the word-to-ngram-entry mapping will be done here.
 */
void
make_voca_ref(NGRAM_INFO *ndata, WORD_INFO *winfo)
{
  int i;

  jlog("Stat: init_ngram: mapping dictonary words to n-gram entries\n");
  ndata->unk_num = 0;
  for (i = 0; i < winfo->num; i++) {
    winfo->wton[i] = make_ngram_ref(ndata, winfo->wname[i]);
    if (winfo->wton[i] == ndata->unk_id) {
      (ndata->unk_num)++;
    }
  }
  if (ndata->unk_num == 0) {
    ndata->unk_num_log = 0.0;	/* for safe */
  } else {
    ndata->unk_num_log = (float)log10(ndata->unk_num);
  }
  jlog("Stat: init_ngram: finished word-to-ngram mapping\n");
}

/** 
 * @brief  Set unknown word ID to the N-gram data.
 *
 * In CMU-Cam SLM toolkit, OOV words are always mapped to UNK, which
 * always appear at the very beginning of N-gram entry, so we fix the
 * unknown word ID at "0".
 * 
 * @param ndata [out] N-gram data to set unknown word ID.
 */
void
set_unknown_id(NGRAM_INFO *ndata)
{
#if 0
  ndata->unk_id = ngram_lookup_word(ndata, unkword);
  if (ndata->unk_id == WORD_INVALID) {
    jlog("word %s not found, so assume this is a closed vocabulary model\n",
	    unkword);
    ndata->isopen = FALSE;
  } else {
    ndata->isopen = TRUE;
  }
#endif
  ndata->isopen = TRUE;
  ndata->unk_id = 0;		/* unknown (OOV) words are always mapped to
				   the number 0 (by CMU-TK)*/
}
