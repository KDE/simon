/**
 * @file   adin_mic_sol2.c
 * 
 * <JA>
 * @brief  マイク入力 (Solaris2.x)
 *
 * Solaris 2.x でマイク入力を使用するための低レベル音声入力関数です．
 * Solaris 2.x のマシンではデフォルトでこのファイルが使用されます．
 *
 * Sun Solaris 2.5.1 および 2.6 で動作確認をしています．
 * ビッグエンディアンを前提としているため，Solaris x86 では動きません．
 *
 * 起動後オーディオ入力はマイクに自動的に切り替わりますが，
 * ボリュームは自動調節されません．gaintoolなどで別途調節してください． 
 *
 * デフォルトのデバイス名は "/dev/audio" です．環境変数 AUDIODEV に
 * デバイス名を指定することで，他のデバイス名を使用できます．
 * </JA>
 * <EN>
 * @brief  Microphone input on Solaris 2.x
 *
 * Low level I/O functions for microphone input on Solaris 2.x machines.
 * This file is used as default on Solaris 2.x machines.
 *
 * Tested on Sun Solaris 2.5.1 and 2.6.  Also works on later versions.
 * Please note that this will not work on Solaris x86, since machine
 * byte order is fixed to big endian.
 *
 * The microphone input device will be automatically selected by Julius
 * on startup.  Please note that the recoding volue will not be
 * altered by Julius, and appropriate value should be set by another tool
 * such as gaintool.
 * 
 * The default device name is "/dev/audio", which can be changed by setting
 * environment variable AUDIODEV.
 * </EN>
 * 
 * @author Akinobu LEE
 * @date   Sun Feb 13 19:06:46 2005
 *
 * $Revision: 1.2 $
 * 
 */
/*
 * Copyright (c) 1991-2007 Kawahara Lab., Kyoto University
 * Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 * Copyright (c) 2005-2007 Julius project team, Nagoya Institute of Technology
 * All rights reserved
 */

#include <sent/stddefs.h>
#include <sent/adin.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stropts.h>

/* sound header */
#include <sys/audioio.h>
static int afd;			///< Audio file descriptor
static struct audio_info ainfo;	///< Audio format information

/// Default device name, can be overridden by AUDIODEV environment variable
#define DEFAULT_DEVICE "/dev/audio"

/** 
 * Device initialization: check device capability and open for recording.
 * 
 * @param sfreq [in] required sampling frequency.
 * @param arg [in] a dummy data
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_standby(int sfreq, void *arg)
{
  char *defaultdev = DEFAULT_DEVICE;
  char *devname;

  /* get device name if specified in $AUDIODEV */
  if ((devname = getenv("AUDIODEV")) == NULL) {
    devname = defaultdev;
    jlog("Stat: adin_sol2: device name = %s\n", devname);
  } else {
    jlog("Stat: adin_sol2: device name obtained from AUDIODEV: %s\n", devname);
  }

  /* open the device */
  if ((afd = open(devname, O_RDONLY)) == -1) {
    jlog("Error: adin_sol2: failed to open audio device %s\n", devname);
    return(FALSE);
  }

#if 0
  {
    /* output hardware info (debug) */
    struct audio_device adev;
    if (ioctl(afd, AUDIO_GETDEV, &adev)== -1) {
      jlog("Erorr: adin_sol2: failed to get hardware info\n");
      return(FALSE);
    }
    jlog("Stat: adin_sol2: Hardware name: %s\n",adev.name);
    jlog("Stat: adin_sol2: Hardware version: %s\n", adev.version);
    jlog("Stat: adin_sol2: Properties: %s\n", adev.config);
  }
#endif

  /* get current setting */
  if (ioctl(afd, AUDIO_GETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to get current setting from device\n");
    return(FALSE);
  }
  /* pause for changing setting */
  ainfo.record.pause = 1;
  if (ioctl(afd, AUDIO_SETINFO, &ainfo) == -1) {
    jlog("Erorr: adin_sol2: failed to pause for changing setting\n");
    return(FALSE);
  }
  /* flush current input buffer (in old format) */
  if((ioctl(afd , I_FLUSH , FLUSHR)) == -1) {
    jlog("Error: adin_sol2: failed to flush current input buffer\n");
    return(FALSE);
  }
  /* set record setting */
  ainfo.record.sample_rate = sfreq;
  ainfo.record.channels = 1;
  ainfo.record.precision = 16;
  ainfo.record.encoding = AUDIO_ENCODING_LINEAR;
  /* ainfo.record.gain = J_DEF_VOLUME * (AUDIO_MAX_GAIN - AUDIO_MIN_GAIN) / 100 + AUDIO_MIN_GAIN; */
  ainfo.record.port = AUDIO_MICROPHONE;
  /* recording should be paused when initialized */
  ainfo.record.pause = 1;

  /* set audio setting, remain pause */
  if (ioctl(afd, AUDIO_SETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to set audio setting\n");
    return(FALSE);
  }

  return(TRUE);

}

/** 
 * Start recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_begin()
{
  if (ioctl(afd, AUDIO_GETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to get audio status\n");
    return(FALSE);
  }
  ainfo.record.pause = 0;
  if (ioctl(afd, AUDIO_SETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to set audio status\n");
    return(FALSE);
  }

  return(TRUE);
}

/** 
 * Stop recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_end()
{
  if (ioctl(afd, AUDIO_GETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to get audio status\n");
    return(FALSE);
  }
  ainfo.record.pause = 1;
  if (ioctl(afd, AUDIO_SETINFO, &ainfo) == -1) {
    jlog("Error: adin_sol2: failed to set audio status\n");
    return(FALSE);
  }
  return(TRUE);
}

/**
 * @brief  Read samples from device
 * 
 * Try to read @a sampnum samples and returns actual number of recorded
 * samples currently available.  This function will block until
 * at least some samples are obtained.
 * 
 * @param buf [out] samples obtained in this function
 * @param sampnum [in] wanted number of samples to be read
 * 
 * @return actural number of read samples, -2 if an error occured.
 */
int
adin_mic_read(SP16 *buf, int sampnum)
{
  int cnt;
  cnt = read(afd, buf, sampnum * sizeof(SP16)) / sizeof(SP16);
  if (cnt < 0) {
    jlog("Error: adin_sol2: failed to read sample\n");
    return(-2);
  }
  return(cnt);
}
