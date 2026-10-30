#include <alsa/asoundlib.h> /* for alsa interface   */
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include "midircv.h"
#include "serialout.h"

extern void fromMIDItoSerial(snd_seq_event_t *ev);

int main()
{
    snd_seq_event_t *ev;
    struct pollfd *pfd;
    int npfd; /* poll_descriptors */

    if (init_midi_seq() == -1)
        exit(1);

    if (serial_init() == -1)
        exit(1);

    npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
    fprintf(stderr, "poll_descriptors=%d\n", npfd);
    pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

#if 0
    unsigned char noteon[3] = {0x90, 0x60, 0x7F};
    unsigned char noteoff[3] = {0x80, 0x60, 0x7F};

    while (1)
    {
        sendmessage(noteon);
        sleep(1);
        sendmessage(noteoff);
        sleep(1);
    }
#endif

    while (1) /* main loop */
        if (poll(pfd, npfd, 1000000) > 0)
        {
            do
            {
                snd_seq_event_input(seq_handle, &ev);
                fromMIDItoSerial(ev);
#if 0
                snd_seq_ev_set_source(ev, oportid);
                snd_seq_ev_set_subs(ev);
                snd_seq_event_output_direct(seq_handle, ev);
#endif
                snd_seq_free_event(ev);
            } while (snd_seq_event_input_pending(seq_handle, 0) > 0);

            waittrans();
        }

    close(fd);
}

void fromMIDItoSerial(snd_seq_event_t *ev)
{
    unsigned char message[3];
    switch (ev->type)
    {
    case SND_SEQ_EVENT_NOTEOFF: // 0x80
        message[0] = 0x80 + ev->data.control.channel;
        message[1] = ev->data.note.note;
        message[2] = ev->data.note.velocity;
        sendmessage_size(message, 3);
        break;

    case SND_SEQ_EVENT_NOTEON: // 0x90
        message[0] = 0x90 + ev->data.control.channel;
        message[1] = ev->data.note.note;
        message[2] = ev->data.note.velocity;
        sendmessage_size(message, 3);
        break;

    case SND_SEQ_EVENT_KEYPRESS: // 0xA0
        message[0] = 0xA0 + ev->data.control.channel;
        message[1] = ev->data.note.note;
        message[2] = ev->data.note.velocity;
        sendmessage_size(message, 3);
        break;

    case SND_SEQ_EVENT_CONTROLLER: // 0xB0
        message[0] = 0xB0 + ev->data.control.channel;
        message[1] = ev->data.control.param;
        message[2] = ev->data.control.value;
        sendmessage_size(message, 3);
        break;

    case SND_SEQ_EVENT_PGMCHANGE: // 0xC0
        message[0] = 0xC0 + ev->data.control.channel;
        message[1] = ev->data.control.value;
        sendmessage_size(message, 2);
        break;

    case SND_SEQ_EVENT_CHANPRESS: // 0xD0
        message[0] = 0xD0 + ev->data.control.channel;
        message[1] = ev->data.control.value;
        sendmessage_size(message, 2);
        break;

    case SND_SEQ_EVENT_PITCHBEND: // 0xE0
        int bend = ev->data.control.value + 0x2000;
        message[0] = 0xE0 + ev->data.control.channel;
        message[1] = bend & 0x7F;
        message[2] = (bend >> 7) & 0x7F;
        sendmessage_size(message, 3);
        break;

    case SND_SEQ_EVENT_SYSEX: // 0xF0
        unsigned char *p = (unsigned char *)ev->data.ext.ptr;
        sendmessage_size(p, ev->data.ext.len);
        waittrans();

#if 0
        for (int i = 0; i < ev->data.ext.len; i++, p++)
        {
            fprintf(stderr, "%02x ", *p);
        }
        fprintf(stderr, "\n");
#endif        
        break;
    }
}