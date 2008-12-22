/**
 * @file   write_hmmlist.c
 * 
 * <JA>
 * @brief  HMMListファイルをバイナリ形式で出力する
 * </JA>
 * 
 * <EN>
 * @brief  Write HMMList data to binary file
 *
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
save_hmmlist_callback(void *data, FILE *fp)
{
  HMM_Logical *l = data;
  char *s;
  int len;

  if (myfwrite(&(l->is_pseudo), sizeof(boolean), 1, fp) < 1) return FALSE;
  len = strlen(l->name) + 1;
  if (myfwrite(&len, sizeof(int), 1, fp) < 1) return FALSE;
  if (myfwrite(l->name, len, 1, fp) < 1) return FALSE;
  if (l->is_pseudo) {
    s = l->body.pseudo->name;
  } else {
    s = l->body.defined->name;
  }
  len = strlen(s) + 1;
  if (myfwrite(&len, sizeof(int), 1, fp) < 1) return FALSE;
  if (myfwrite(s, len, 1, fp) < 1) return FALSE;
  
  return TRUE;
}

boolean
save_hmmlist_bin(FILE *fp, HTK_HMM_INFO *hmminfo)
{
  /* set mark */
  int x = 0;
  if (myfwrite(&x, sizeof(int), 1, fp) < 1) {
    jlog("Error: save_hmmlist_bin: failed to write hmmlist to binary file\n");
    return FALSE;
  }
  if (aptree_write(fp, hmminfo->logical_root, save_hmmlist_callback) == FALSE) {
    jlog("Error: save_hmmlist_bin: failed to write hmmlist to binary file\n");
    return FALSE;
  }
  return TRUE;
}
