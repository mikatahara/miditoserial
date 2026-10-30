//
// MIDI through using sequencer interface
// midiseqinout.c
//
#include <alsa/asoundlib.h> /* for alsa interface   */
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include "midircv.h"

snd_seq_t *seq_handle; /* sequencer handle */
int iportid;           /* input port */
int oportid;           /* output ports */

int init_midi_seq()
{
  int i;

  if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_DUPLEX, 0) < 0)
  {
    fprintf(stderr, "Error opening ALSA sequencer.\n");
    exit(1);
  }

  /* set client name */
  snd_seq_set_client_name(seq_handle, "MIDI Client");

  /* open input port */
  if ((iportid = snd_seq_create_simple_port(seq_handle, "TEST IN",
                                            SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                            SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
  {
    fprintf(stderr, "fatal error: could not open input port.\n");
    return (-1);
  }

  /* open output ports */
  if ((oportid = snd_seq_create_simple_port(seq_handle, "TEST OUT",
                                            SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                            SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
  {
    fprintf(stderr, "fatal error: could not open output port.\n");
    return (-1);
  }

  return 0;
}
