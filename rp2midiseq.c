//
// MIDI through using sequencer interface
// midiseqinout.c
//
#include <alsa/asoundlib.h> /* for alsa interface   */
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>

static snd_seq_t *seq_handle;  /* sequencer handle */
static int iportid;        /* input port */
static int oportid;        /* output ports */

int main()
{
  snd_seq_event_t *ev;
  int npfd;
  struct pollfd *pfd;
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
    exit(1);
  }

  /* open output ports */
  if ((oportid = snd_seq_create_simple_port(seq_handle, "TEST OUT",
    SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
    SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
  {
    fprintf(stderr, "fatal error: could not open output port.\n");
    exit(1);
  }

  npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
  fprintf(stderr,"poll_descriptors=%d\n",npfd);
  pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
  snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

  while (1) /* main loop */
    if (poll(pfd, npfd, 1000000) > 0)
    {
      do
      {
        snd_seq_event_input(seq_handle, &ev);
        snd_seq_ev_set_source(ev, oportid);
        snd_seq_ev_set_subs(ev); 
        snd_seq_event_output_direct(seq_handle, ev);
        snd_seq_free_event(ev);
      } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
    }
  return 0;
}
