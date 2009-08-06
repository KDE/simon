/**
 * @file   read_rdhmmlist.c
 * 
 * <JA>
 * @brief  HMMListファイルを読み込む（バイナリ版）
 *
 * HMMList ファイルは，辞書上の音素表記（トライフォン表記）から
 * 実際に定義されている %HMM へのマッピングを行なうファイルです．
 * 
 * HMMListファイルでは，登場しうる音素について，対応する
 * HMM 定義の名前を記述します．一行に１つづつ，第1カラムに音素名，
 * スペースで区切って第2カラムに定義されている実際の %HMM 名を指定します．
 * 第1カラムと第2カラムが全く同じ場合，すなわちその音素名のモデルが直接
 * %HMM として定義されている場合は，第2カラムは省略することができます．
 *
 * トライフォン使用時は，HMMListファイルで登場しうる全てのトライフォンに
 * ついて記述する必要がある点に注意して下さい．もし与えられた認識辞書
 * 上で登場しうるトライフォンがHMMListに記述されていない場合，
 * エラーとなります．
 * </JA>
 * 
 * <EN>
 * @brief  Read in HMMList file from binary format
 *
 * HMMList file specifies how the phones as described in word dictionary,
 * or their context-dependent form, should be mapped to actual defined %HMM.
 *
 * In HMMList file, the possible phone names and their corresponding %HMM
 * name should be specified one per line.  The phone name should be put on
 * the first column, and its corresponding %HMM name in the HTK %HMM definition
 * file should be defined on the second column.  If the two strings are
 * the same, which occurs when a %HMM of the phone name is directly defined,
 * the second column can be omitted.
 *
 * When using a triphone model, ALL the possible triphones that can appear
 * on the given word dictionary should be specified in the HMMList file.
 * If some possible triphone are not specified in the HMMList, Julius
 * produces error.
 * </EN>
 * 
 * @author Akinobu LEE
 * @date   Wed Feb 16 04:04:23 2005
 *
 * $Revision: 1.1 $
 * 
 */
/*
 * Copyright (c) 1991-2007 Kawahara Lab., Kyoto University
 * Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 * Copyright (c) 2005-2007 Julius project team, Nagoya Institute of Technology
 * All rights reserved
 */

#include <sent/stddefs.h>
#include <sent/htk_hmm.h>
#include <sent/ptree.h>

static boolean
load_hmmlist_callback(void **data_p, void *data, FILE *fp)
{
  HTK_HMM_INFO *hmminfo = data;
  HMM_Logical *new;
  int len;
  char buf[MAX_HMMNAME_LEN];
  HTK_HMM_Data *d;

  new = (HMM_Logical *)mybmalloc2(sizeof(HMM_Logical), &(hmminfo->lroot));
  if (myfread(&(new->is_pseudo), sizeof(boolean), 1, fp) < 1) return FALSE;
  if (myfread(&len, sizeof(int), 1, fp) < 1) return FALSE;
  new->name = (char *)mybmalloc2(len, &(hmminfo->lroot));
  if (myfread(new->name, len, 1, fp) < 1) return FALSE;
  if (myfread(&len, sizeof(int), 1, fp) < 1) return FALSE;
  if (myfread(buf, len, 1, fp) < 1) return FALSE;
  if (new->is_pseudo) {
    jlog("Error: sorry, pseudo logical phone saving not implemented yet\n");
    return FALSE;
  } else {
    d = htk_hmmdata_lookup_physical(hmminfo, buf);
    if (d == NULL) {
      jlog("Error: load_hmmlist_callback: no name \"%s\" in hmmdefs!\n", buf);
      return FALSE;
    }
    new->body.defined = d;
  }
  new->next = hmminfo->lgstart;
  hmminfo->lgstart = new;

  *data_p = new;
  
  return TRUE;
}

boolean
load_hmmlist_bin(FILE *fp, HTK_HMM_INFO *hmminfo)
{
  HMM_Logical *l;
  int n;

  if (aptree_read(fp, &(hmminfo->logical_root), &(hmminfo->lroot), hmminfo, load_hmmlist_callback) == FALSE) {
    jlog("Error: load_hmmlist_bin: failed to read hmmlist from binary file\n");
    return FALSE;
  }
  n = 0;
  for(l=hmminfo->lgstart;l;l=l->next) n++;
  hmminfo->totallogicalnum = n;

  return TRUE;
}
