/**
 * @file   adin_mic_linux.c
 *
 * <JA>
 * @brief  マイク入力 (Linux) - デフォルトデバイス
 *
 * マイク入力のための低レベル関数です．
 * インタフェースを指定しない場合に呼ばれます．
 * 実際には，ALSA, OSS, ESD の順で最初に見つかったものが使用されます．
 * </JA>
 * <EN>
 * @brief  Microphone input on Linux - default device
 *
 * Low level I/O functions for microphone input on Linux.
 * This will be called when no device was explicitly specified.
 * A call to the functions in this file will be redirected to
 * ALSA, OSS or ESD.
 * </EN>
 *
 * @author Akinobu LEE
 * @date   Sun Feb 13 16:18:26 2005
 *
 * $Revision: 1.2 $
 * 
 */
/*
 * Copyright (c) 1991-2008 Kawahara Lab., Kyoto University
 * Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 * Copyright (c) 2005-2008 Julius project team, Nagoya Institute of Technology
 * All rights reserved
 */

#include <sent/stddefs.h>
#include <sent/adin.h>

/** 
 * Device initialization: check device capability and open for recording.
 * 
 * @param sfreq [in] required sampling frequency.
 * @param dummy [in] a dummy data
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_standby(int sfreq, void *dummy)
{
#if defined(HAS_ALSA)
  return(adin_alsa_standby(sfreq, dummy));
#elif defined(HAS_OSS)
  return(adin_oss_standby(sfreq, dummy));
#elif defined(HAS_ESD)
  return(adin_esd_standby(sfreq, dummy));
#else  /* other than Linux */
  jlog("Error: neither of alsa/oss/esd device is available\n");
  return FALSE;
#endif
}

/** 
 * Start recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_begin()
{
#if defined(HAS_ALSA)
  return(adin_alsa_begin());
#elif defined(HAS_OSS)
  return(adin_oss_begin());
#elif defined(HAS_ESD)
  return(adin_esd_begin());
#else  /* other than Linux */
  jlog("Error: neither of alsa/oss/esd device is available\n");
  return FALSE;
#endif
}

/** 
 * Stop recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_end()
{
#if defined(HAS_ALSA)
  return(adin_alsa_end());
#elif defined(HAS_OSS)
  return(adin_oss_end());
#elif defined(HAS_ESD)
  return(adin_esd_end());
#else  /* other than Linux */
  jlog("Error: neither of alsa/oss/esd device is available\n");
  return FALSE;
#endif
}

/**
 * @brief  Read samples from device
 * 
 * Try to read @a sampnum samples and returns actual number of recorded
 * samples currently available.  This function will block until
 * at least one sample can be obtained.
 * 
 * @param buf [out] samples obtained in this function
 * @param sampnum [in] wanted number of samples to be read
 * 
 * @return actural number of read samples, -2 if an error occured.
 */
int
adin_mic_read(SP16 *buf, int sampnum)
{
#if defined(HAS_ALSA)
  return(adin_alsa_read(buf, sampnum));
#elif defined(HAS_OSS)
  return(adin_oss_read(buf, sampnum));
#elif defined(HAS_ESD)
  return(adin_esd_read(buf, sampnum));
#else  /* other than Linux */
  jlog("Error: neither of alsa/oss/esd device is available\n");
  return -2;
#endif
}
