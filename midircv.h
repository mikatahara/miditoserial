
#include <alsa/asoundlib.h> /* for alsa interface   */

extern snd_seq_t *seq_handle;  /* sequencer handle */
extern int iportid;            /* input port */
extern int oportid;            /* output ports */

int init_midi_seq();    /* Initialize MIDI sequencer */
